#include <cinttypes>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "diagnostics.h"
#include "esp_log.h"

namespace {
    constexpr char TAG[] = "secure_telemetry";
}

extern "C" void app_main() {
        const diagnostics::Snapshot snapshot = diagnostics::collect();

        ESP_LOGI(
            TAG,
            "uptime = %" PRIu64
            " ms, free_heap = %" PRIu32
            " bytes, minimum_free_heap = %" PRIu32 " bytes",
            snapshot.uptime_ms,
            snapshot.free_heap_bytes,
            snapshot.free_heap_bytes);
}
