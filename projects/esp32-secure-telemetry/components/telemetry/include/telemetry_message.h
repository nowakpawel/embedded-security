#pragma once

#include <cstdint>
#include <type_traits>

namespace telemetry {
    struct TelemetryMessage {
        std::uint64_t uptime_ms;
        std::uint32_t sequence;
        std::uint32_t free_heap_bytes;
        std::uint32_t minimum_free_heap_bytes;
        std::uint32_t minimum_free_stack_bytes;
    };

    static_assert(
        std::is_trivially_copyable_v<TelemetryMessage>,
        "TelemetryMessage must be safe to copy through a FreeRTOS queue");
}