#pragma once

#ifdef _WIN32
	#include "directXMathTypes.hpp"
#else

#endif

Mat4x4 matrixPerspectiveFovLH(const float& FovAngleY, const float& AspectRatio, const float& NearZ,
	const float& FarZ);

Mat4x4 matrixOrthographicOffCenterLH(const float& view_left, const float& view_right,
	const float& view_bottom, const float& view_top, const float& near_z, const float& far_z);