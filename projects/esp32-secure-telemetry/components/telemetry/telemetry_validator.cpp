#include "telemetry_validator.h"
#include "include/telemetry_validator.h"

namespace telemetry {
    ValidationResult validate(const TelemetryMessage& message) {
        if (message.total_heap_bytes == 0) {
            return ValidationResult::total_heap_is_zero;
        }

        if (message.free_heap_bytes == 0) {
            return ValidationResult::free_heap_is_zero;
        }

        if (message.minimum_free_heap_bytes == 0) {
            return ValidationResult::minimum_free_heap_is_zero;
        }

        if (message.free_heap_bytes > message.total_heap_bytes) {
            return ValidationResult::free_heap_exceeds_total_heap;
        }

        if (message.minimum_free_heap_bytes > message.free_heap_bytes) {
            return ValidationResult::minimum_free_heap_exceeds_current_heap;
        }

        if (message.minimum_free_stack_bytes == 0) {
            return ValidationResult::minimum_free_stack_is_zero;
        }

        return ValidationResult::valid;
    }

    const char* to_string(const ValidationResult result) {
        switch (result) {
            case ValidationResult::valid:
                return "valid";
            case ValidationResult::total_heap_is_zero:
                return "total_heap_is_zero";
            case ValidationResult::free_heap_is_zero:
                return "free_heap_is_zero";
            case ValidationResult::minimum_free_heap_is_zero:
                return "minimum_free_heap_is_zero";
            case ValidationResult::free_heap_exceeds_total_heap:
                return "free_heap_exceeds_total_heap";
            case ValidationResult::minimum_free_heap_exceeds_current_heap:
                return "free_heap_exceeds_total_heap";
            case ValidationResult::minimum_free_stack_is_zero:
                return "minimum_free_stack_is_zero";
        }

        return "unknown_validation_result";
    }
}
