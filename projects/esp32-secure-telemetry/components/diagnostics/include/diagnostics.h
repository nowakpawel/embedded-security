#pragma once

#include<cstdint>

namespace diagnostics {
	struct Snapshot {
		std::uint64_t uptime_ms;
		std::uint32_t free_heap_bytes;
		std::uint32_t minimum_free_heap_bytes;
	};

	Snapshot collect();
}
