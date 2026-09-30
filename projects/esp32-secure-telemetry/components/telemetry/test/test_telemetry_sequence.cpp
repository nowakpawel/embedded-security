#include "telemetry_sequence.h"
#include "unity.h"
#include <limits>

TEST_CASE(
    "duplicate sequence is detected", "[telemetry][sequence]") {
    telemetry::SequenceTracker tracker;

    const telemetry::SequenceResult first_result = tracker.observe(7);

    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(telemetry::SequenceStatus::first_message),
        static_cast<int>(first_result.status));

    const telemetry::SequenceResult duplicate_result = tracker.observe(7);

    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(telemetry::SequenceStatus::duplicate),
        static_cast<int>(duplicate_result.status));

    TEST_ASSERT_EQUAL_UINT32(8, duplicate_result.expected_sequence);

    TEST_ASSERT_EQUAL_UINT32(7, duplicate_result.received_sequence);
    TEST_ASSERT_EQUAL_UINT32(0, duplicate_result.missing_messages);
}

TEST_CASE(
    "sequence gap reports missing messages and advances tracker", "[telemetry][sequence]") {
    telemetry::SequenceTracker tracker;

    const telemetry::SequenceResult first_result = tracker.observe(10);

    TEST_ASSERT_EQUAL_INT(
    static_cast<int>(telemetry::SequenceStatus::first_message),
            static_cast<int>(first_result.status));

    const telemetry::SequenceResult gap_result = tracker.observe(14);

    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(telemetry::SequenceStatus::gap),
        static_cast<int>(gap_result.status));

    TEST_ASSERT_EQUAL_UINT32(11, gap_result.expected_sequence);
    TEST_ASSERT_EQUAL_UINT32(14, gap_result.received_sequence);
    TEST_ASSERT_EQUAL_UINT32(3, gap_result.missing_messages);

    const telemetry::SequenceResult next_result = tracker.observe(15);

    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(telemetry::SequenceStatus::in_order),
        static_cast<int>(next_result.status));
}

TEST_CASE(
    "out-of-order-sequence does not move tracker backwards", "[telemetry][sequence]") {
    telemetry::SequenceTracker tracker;

    const telemetry::SequenceResult first_result = tracker.observe(10);

    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(telemetry::SequenceStatus::first_message),
        static_cast<int>(first_result.status));

    const telemetry::SequenceResult out_of_order_result = tracker.observe(8);

    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(telemetry::SequenceStatus::out_of_order),
        static_cast<int>(out_of_order_result.status));

    TEST_ASSERT_EQUAL_UINT32(11, out_of_order_result.expected_sequence);
    TEST_ASSERT_EQUAL_UINT32(8, out_of_order_result.received_sequence);
    TEST_ASSERT_EQUAL_UINT32(0, out_of_order_result.missing_messages);

    const telemetry::SequenceResult next_result = tracker.observe(11);

    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(telemetry::SequenceStatus::in_order),
        static_cast<int>(next_result.status));
}

TEST_CASE(
    "sequence remains in order after uint32 wraparound", "[telemetry][sequence]") {
    constexpr std::uint32_t maximum_sequence = std::numeric_limits<std::uint32_t>::max();

    telemetry::SequenceTracker tracker;

    const telemetry::SequenceResult first_result = tracker.observe(maximum_sequence - 1U);

    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(telemetry::SequenceStatus::first_message),
        static_cast<int>(first_result.status));

    const telemetry::SequenceResult maximum_result = tracker.observe(maximum_sequence);

    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(telemetry::SequenceStatus::in_order),
        static_cast<int>(maximum_result.status));

    TEST_ASSERT_EQUAL_UINT32(maximum_sequence, maximum_result.expected_sequence);

    const telemetry::SequenceResult wrapped_result = tracker.observe(0);

    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(telemetry::SequenceStatus::in_order),
        static_cast<int>(wrapped_result.status));

    TEST_ASSERT_EQUAL_UINT32(0, wrapped_result.expected_sequence);
    TEST_ASSERT_EQUAL_UINT32(0, wrapped_result.received_sequence);

    const telemetry::SequenceResult next_result = tracker.observe(1);

    TEST_ASSERT_EQUAL_INT(
        static_cast<int>(telemetry::SequenceStatus::in_order),
        static_cast<int>(next_result.status));
}



