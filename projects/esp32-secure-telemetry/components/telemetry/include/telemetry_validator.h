#pragma once

#include "telemetry_message.h"

namespace telemetry {
    enum class ValidationResult {
        valid,
        total_heap_is_zero,
        free_heap_is_zero,
        minimum_free_heap_is_zero,
        free_heap_exceeds_total_heap,
        minimum_free_heap_exceeds_current_heap,
        minimum_free_stack_is_zero,
    };

    [[nodiscard]] ValidationResult validate(const TelemetryMessage& message);
    [[nodiscard]] const char* to_string(ValidationResult result);
}
