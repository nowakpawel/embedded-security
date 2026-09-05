#include <cinttypes>
#include <cstdint>

#include "diagnostics.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "telemetry_message.h"
#include "../components/telemetry/include/telemetry_message.h"

namespace {
    constexpr char TAG[] = "secure_telemetry";

    constexpr char DIAGNOSTICS_TASK_NAME[] = "diagnostics";
    constexpr char PROCESSING_TASK_NAME[] = "processing";

    constexpr std::uint32_t DIAGNOSTICS_TASK_STACK_SIZE = 4096;
    constexpr std::uint32_t PROCESSING_TASK_STACK_SIZE = 4096;

    constexpr UBaseType_t DIAGNOSTICS_TASK_PRIORITY = 5;
    constexpr UBaseType_t PROCESSING_TASK_PRIORITY = 6;

    constexpr UBaseType_t TELEMETRY_QUEUE_LENGTH = 5;

    constexpr TickType_t DIAGNOSTICS_INTERVAL = pdMS_TO_TICKS(5000);

    constexpr TickType_t BLINK_DURATION = pdMS_TO_TICKS(100);

    constexpr TickType_t QUEUE_SEND_TIMEOUT = pdMS_TO_TICKS(6000);
    constexpr TickType_t QUEUE_RECEIVE_TIMEOUT = pdMS_TO_TICKS(6000);

    constexpr gpio_num_t BLINK_GPIO = GPIO_NUM_2;

    constexpr std::uint32_t LED_ON = 1;
    constexpr std::uint32_t LED_OFF = 0;


    void configure_status_led() {
        gpio_config_t config{};
        config.pin_bit_mask = 1ULL << static_cast<std::uint32_t>(BLINK_GPIO);

        config.mode = GPIO_MODE_OUTPUT;
        config.pull_up_en = GPIO_PULLUP_DISABLE;
        config.pull_down_en = GPIO_PULLDOWN_DISABLE;
        config.intr_type = GPIO_INTR_DISABLE;

        ESP_ERROR_CHECK(gpio_config(&config));
        ESP_ERROR_CHECK(gpio_set_level(BLINK_GPIO, LED_OFF));
    }

    QueueHandle_t get_queue_from_paraeters(void* parameters) {
        return static_cast<QueueHandle_t>(parameters);
    }

    void diagnostics_task(void *parameters) {
        QueueHandle_t telemetry_queue = get_queue_from_paraeters(parameters);

        if (telemetry_queue == nullptr) {
            ESP_LOGE(TAG, "Diagnostics task received a null queue");
            vTaskDelete(nullptr);
            return;
        }

        TickType_t last_wake_time = xTaskGetTickCount();
        std::uint32_t sequence = 0;

        ESP_LOGI(TAG, "Diagnostics task started");

        while (true) {
            ESP_ERROR_CHECK(
                gpio_set_level(BLINK_GPIO, LED_ON));
            const diagnostics::Snapshot snapshot = diagnostics::collect();
            const auto minimum_free_stack = uxTaskGetStackHighWaterMark(nullptr);

            const telemetry::TelemetryMessage message{
                .uptime_ms =  snapshot.uptime_ms,
                .sequence = sequence,
                .free_heap_bytes = snapshot.free_heap_bytes,
                .minimum_free_heap_bytes = snapshot.minimum_free_heap_bytes,
                .minimum_free_stack_bytes = static_cast<std::uint32_t>(minimum_free_stack),
            };

            const BaseType_t send_result = xQueueSend(
                telemetry_queue,
                &message,
                QUEUE_SEND_TIMEOUT);

            if (send_result != pdPASS) {
                ESP_LOGW(TAG, "Telemetry queue full; dropped message: %%" PRIu32, message.sequence);
            }

            sequence++;

            vTaskDelay(BLINK_DURATION);

            ESP_ERROR_CHECK(
                gpio_set_level(BLINK_GPIO, LED_OFF));

            xTaskDelayUntil(
                &last_wake_time,
                DIAGNOSTICS_INTERVAL);
        }
    }

    void processing_task(void* parameters) {
        QueueHandle_t telemetry_queue = get_queue_from_paraeters(parameters);

        if (telemetry_queue == nullptr) {
            ESP_LOGE(TAG, "Processing task received a null queue");
            vTaskDelete(nullptr);
            return;
        }

        ESP_LOGI(TAG, "Processing task started");

        while (true) {
            telemetry::TelemetryMessage message{};

            const BaseType_t receive_result = xQueueReceive(
                telemetry_queue,
                &message,
                QUEUE_RECEIVE_TIMEOUT);

            if (receive_result != pdPASS) {
                ESP_LOGW(TAG, "No telemetry received before timeout");
                continue;
            }

            ESP_LOGI(TAG,
                "received_sequence = %" PRIu32
                ", uptime = %" PRIu64
                "ms, free_heap = %" PRIu32
                " bytes, minimum_free_heap = %" PRIu32
                " bytes, producer_minimum_free_stack = % " PRIu32
                " bytes",
                message.sequence,
                message.uptime_ms,
                message.free_heap_bytes,
                message.minimum_free_heap_bytes,
                message.minimum_free_stack_bytes);
        }
    }
}

extern "C" void app_main() {
    ESP_LOGI(TAG, "Starting Secure Telemetry");

    configure_status_led();

    QueueHandle_t telemetry_queue = xQueueCreate(
        TELEMETRY_QUEUE_LENGTH,
        sizeof(telemetry::TelemetryMessage));

    if (telemetry_queue == nullptr) {
        ESP_LOGE(TAG, "Failed to create telemetry queue");
        return;
    }

    ESP_LOGI(TAG, "Telemetry queue created: capacity = %u, item_size = %u bytes",
        static_cast<unsigned>(TELEMETRY_QUEUE_LENGTH),
        static_cast<unsigned>(sizeof(telemetry::TelemetryMessage)));

    TaskHandle_t processing_task_handle = nullptr;

    const BaseType_t processing_task_result = xTaskCreate(
        processing_task,
        PROCESSING_TASK_NAME,
        PROCESSING_TASK_STACK_SIZE,
        telemetry_queue,
        PROCESSING_TASK_PRIORITY,
        &processing_task_handle);

    if (processing_task_result != pdPASS) {
        ESP_LOGE(TAG, "Failed to create processing task");
        vQueueDelete(telemetry_queue);
        return;
    }

    const BaseType_t diagnostics_task_result = xTaskCreate(
        diagnostics_task,
        DIAGNOSTICS_TASK_NAME,
        DIAGNOSTICS_TASK_STACK_SIZE,
        telemetry_queue,
        DIAGNOSTICS_TASK_PRIORITY,
        &processing_task_handle);


    if (diagnostics_task_result != pdPASS) {
        ESP_LOGE(TAG, "Failed to create diagnostics task");
        vTaskDelete(processing_task_handle);
        vQueueDelete(telemetry_queue);
        return;
    }

    ESP_LOGI(TAG, "Diagnostics task created successfully");
}
