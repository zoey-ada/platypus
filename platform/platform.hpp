#pragma once

enum class Platform {
	Windows,
	Unknown
};

[[nodiscard]] Platform detectPlatform();
