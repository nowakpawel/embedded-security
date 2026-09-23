#include "telemetry_message.h"
#include "telemetry_validator.h"
#include "unity.h"

namespace {
    telemetry::TelemetryMessage create_valid_message() {
        return telemetry::TelemetryMessage{
            .uptime_ms = 1000,
            .sequence = 1,
            .free_heap_bytes = 800,
            .minimum_free_heap_bytes = 400,
            .minimum_free_stack_bytes = 512,
            .total_heap_bytes = 1000,
        };
    }

    void assert_validation_result(const telemetry::ValidationResult expected,
        const telemetry::TelemetryMessage& message) {
        const telemetry::ValidationResult actual = telemetry::validate(message);

        TEST_ASSERT_EQUAL_INT(
            static_cast<int>(expected),
            static_cast<int>(actual));

    }
}

TEST_CASE("Valid telemetry message passes validation", "[telemetry][validation]") {
    const telemetry::TelemetryMessage message = create_valid_message();
    assert_validation_result(telemetry::ValidationResult::valid, message);
}

TEST_CASE("zero total heap is rejected", "[telemetry][validator]") {
    telemetry::TelemetryMessage message = create_valid_message();
    message.total_heap_bytes = 0;

    assert_validation_result(telemetry::ValidationResult::total_heap_is_zero, message);
}

TEST_CASE("zero free heap is rejected", "[telemetry][validator]") {
    telemetry::TelemetryMessage message = create_valid_message();
    message.free_heap_bytes = 0;

    assert_validation_result(telemetry::ValidationResult::free_heap_is_zero, message);
}

TEST_CASE("zero minimum free heap is rejected", "[telemetry][validator]") {
    telemetry::TelemetryMessage message = create_valid_message();
    message.minimum_free_heap_bytes = 0;

    assert_validation_result(telemetry::ValidationResult::minimum_free_heap_is_zero, message);
}

TEST_CASE("zero minimum free stack is rejected", "[telemetry][validator]") {
    telemetry::TelemetryMessage message = create_valid_message();
    message.minimum_free_stack_bytes = 0;

    assert_validation_result(telemetry::ValidationResult::minimum_free_stack_is_zero, message);
}

TEST_CASE("free heap exceeding total heap is rejected", "[telemetry][validator]") {
    telemetry::TelemetryMessage message = create_valid_message();
    message.free_heap_bytes = message.total_heap_bytes + 1;

    assert_validation_result(telemetry::ValidationResult::free_heap_exceeds_total_heap, message);
}

TEST_CASE("minimum free heap exceeding current free heap is rejected", "[telemetry][validator]") {
    telemetry::TelemetryMessage message = create_valid_message();
    message.minimum_free_heap_bytes = message.free_heap_bytes + 1;

    assert_validation_result(telemetry::ValidationResult::minimum_free_heap_exceeds_current_heap, message);
}

TEST_CASE("minimum free heap equal to current free heap is valid", "[telemetry][validator]") {
    telemetry::TelemetryMessage message = create_valid_message();
    message.minimum_free_heap_bytes = message.free_heap_bytes;

    assert_validation_result(telemetry::ValidationResult::valid, message);
}

TEST_CASE("validation results have readable descriptions", "[telemetry][validator]") {
    TEST_ASSERT_EQUAL_STRING(
        "free_heap_exceeds_total_heap",
        telemetry::to_string(telemetry::ValidationResult::free_heap_exceeds_total_heap));

    TEST_ASSERT_EQUAL_STRING("minimum_free_heap_exceeds_current_heap",
        telemetry::to_string(telemetry::ValidationResult::minimum_free_heap_exceeds_current_heap));
}


