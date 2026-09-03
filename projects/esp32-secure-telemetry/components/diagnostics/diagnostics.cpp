#include "diagnostics.h"
#include "esp_heap_caps.h"
#include "esp_timer.h"

namespace diagnostics {
	Snapshot collect() {
		const auto uptime_us = esp_timer_get_time();

		Snapshot snapshot {
			.uptime_ms = static_cast<std::uint64_t>(uptime_us / 1000),
			.free_heap_bytes = 
				static_cast<std::uint32_t>(heap_caps_get_free_size(MALLOC_CAP_8BIT)),
			.minimum_free_heap_bytes = 
				static_cast<std::uint32_t>(heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT)),
		};

		return snapshot;
	}
}
