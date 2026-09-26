#include "telemetry_sequence.h"

namespace {
    constexpr std::uint32_t HALF_SEQUENCE_RANGE = std::uint32_t{1} << 31U;
}

namespace telemetry {
    SequenceResult SequenceTracker::observe(std::uint32_t received_sequence) {
        if (!has_previous_sequence_) {
            has_previous_sequence_ = true;
            previous_sequence_ = received_sequence;

            return {
                .status = SequenceStatus::first_message,
                .expected_sequence = received_sequence,
                .received_sequence  = received_sequence,
                .missing_messages = 0,
            };
        }

        const std::uint32_t expected_sequence = previous_sequence_ + 1U;
        const std::uint32_t sequence_distance = received_sequence - previous_sequence_;

        if (sequence_distance == 0U) {
            return {
                .status = SequenceStatus::duplicate,
                .expected_sequence = expected_sequence,
                .received_sequence = received_sequence,
                .missing_messages = 0,
            };
        }

        if (sequence_distance == 1U) {
            previous_sequence_ = received_sequence;

            return {
                .status = SequenceStatus::in_order,
                .expected_sequence = expected_sequence,
                .received_sequence = received_sequence,
                .missing_messages = 0,
            };
        }

        if (sequence_distance < HALF_SEQUENCE_RANGE) {
            previous_sequence_ = received_sequence;

            return {
                .status = SequenceStatus::gap,
                .expected_sequence = expected_sequence,
                .received_sequence = received_sequence,
                .missing_messages = sequence_distance - 1U,
            };
        }

        return {
            .status = SequenceStatus::out_of_order,
            .expected_sequence = expected_sequence,
            .received_sequence = received_sequence,
            .missing_messages = 0,
        };
    }

    const char* to_string(SequenceStatus status) {
        switch (status) {
            case SequenceStatus::first_message:
                return "first_message";
            case SequenceStatus::in_order:
                return "in_order";
            case SequenceStatus::duplicate:
                return "duplicate";
            case SequenceStatus::gap:
                return "gap";
            case SequenceStatus::out_of_order:
                return "out_of_order";
        }

        return "uknown_sequence_status";
    }
}