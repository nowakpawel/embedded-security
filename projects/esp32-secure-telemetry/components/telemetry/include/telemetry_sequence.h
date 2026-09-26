#pragma once

#include <cstdint>

namespace telemetry {
    enum class SequenceStatus {
        first_message,
        in_order,
        duplicate,
        gap,
        out_of_order
    };

    struct SequenceResult {
        SequenceStatus status;
        std::uint32_t expected_sequence;
        std::uint32_t received_sequence;
        std::uint32_t missing_messages;
    };

    class SequenceTracker {
    public:
        [[nodiscard]] SequenceResult observe(std::uint32_t received_sequence);

    private:
        bool has_previous_sequence_{false};
        std::uint32_t previous_sequence_{0};
    };

    [[nodiscard]] const char* to_string(SequenceStatus status);
}
