#include <cinttypes>

#include "diagnostics.h"
#include "esp_log.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/gpio.h"

namespace {
    constexpr char TAG[] = "secure_telemetry";
    constexpr char DIAGNOSTICS_TASK_NAME[] = "diagnostics";

    constexpr std::uint32_t DIAGNOSTIC_TASK_STACK_SIZE = 4096;
    constexpr UBaseType_t DIAGNOSTIC_TASK_PRIORITY = 5;

    constexpr gpio_num_t BLINK_GPIO = GPIO_NUM_2;

    constexpr std::uint32_t LED_ON = 1;
    constexpr std::uint32_t LED_OFF = 0;

    constexpr TickType_t BLINK_DURATION = pdMS_TO_TICKS(100);

    constexpr TickType_t DIAGNOSTICS_INTERVAL = pdMS_TO_TICKS(5000);

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

    void diagnostics_task(void *parameters) {
        // In current version we do not pass any parameters to the task.
        // Explicit use of void prevents compiler warning.
        (void) parameters;

        TickType_t last_wake_time = xTaskGetTickCount();

        ESP_LOGI(TAG, "Diagnostics task started");

        while (true) {
            ESP_ERROR_CHECK(
                gpio_set_level(BLINK_GPIO, LED_ON));
            const diagnostics::Snapshot snapshot = diagnostics::collect();
            const UBaseType_t stack_high_water_mark = uxTaskGetStackHighWaterMark(nullptr);

            ESP_LOGI(
                TAG,
                "uptime = %" PRIu64
                " ms, free_heap = %" PRIu32
                " bytes, minimum_free_heap = %" PRIu32 " bytes",
                snapshot.uptime_ms,
                snapshot.free_heap_bytes,
                snapshot.minimum_free_heap_bytes,
                static_cast<unsigned>(stack_high_water_mark));

            vTaskDelay(BLINK_DURATION);

            ESP_ERROR_CHECK(
                gpio_set_level(BLINK_GPIO, LED_OFF));

            xTaskDelayUntil(
                &last_wake_time,
                DIAGNOSTICS_INTERVAL);
        }
    }
}

extern "C" void app_main() {
    ESP_LOGI(TAG, "Starting Secure Telemetry");

    configure_status_led();

    const BaseType_t telemetry_task_result = xTaskCreate(
        diagnostics_task,
        DIAGNOSTICS_TASK_NAME,
        DIAGNOSTIC_TASK_STACK_SIZE,
        nullptr,
        DIAGNOSTIC_TASK_PRIORITY,
        nullptr);

    if (telemetry_task_result != pdPASS) {
        ESP_LOGE(TAG, "Failed to create diagnostics task");
        return;
    }

    ESP_LOGI(TAG, "Diagnostics task created successfully");
}
