#include "directXMathTypes.hpp"

Mat4x4 matrixPerspectiveFovLH(const float& fovAngleY, const float& aspectRatio, const float& nearZ,
	const float& farZ)
{
	return Mat4x4(dx::XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ));
}

Mat4x4 matrixOrthographicOffCenterLH(const float& view_left, const float& view_right,
	const float& view_bottom, const float& view_top, const float& near_z, const float& far_z)
{
	return Mat4x4(dx::XMMatrixOrthographicOffCenterLH(view_left, view_right, view_bottom, view_top,
		near_z, far_z));
}
