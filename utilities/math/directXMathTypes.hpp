#pragma once

#include <algorithm>
#include <array>
#include <memory>

#include <DirectXMath.h>

namespace dx = DirectX;

class Vec2;
class Vec3;
class Vec4;
class Mat4x4;
class Quaternion;
class Plane;
class Frustum;

class Vec2: public dx::XMFLOAT2
{
public:
	inline explicit Vec2(const float _x = 1.0f, const float _y = 1.0f);
	inline Vec2(const Vec2& copy) = default;

	virtual ~Vec2() = default;

	[[nodiscard]] inline float length() const;
	[[nodiscard]] inline Vec2 normalize() const;
	[[nodiscard]] inline float dotProduct(const Vec2& b) const;
	[[nodiscard]] inline Vec2 crossProduct(const Vec2& b) const;

	[[nodiscard]] inline Vec2 operator-(const Vec2& rhs) const;
	[[nodiscard]] inline Vec2 operator+(const Vec2& rhs) const;
	[[nodiscard]] inline Vec2 operator*(const float& rhs) const;
	[[nodiscard]] inline Vec2 operator/(const float& rhs) const;

	[[nodiscard]] inline dx::XMVECTOR load() const { return dx::XMLoadFloat2(this); }

private:
	inline void store(const dx::XMVECTOR& v) { dx::XMStoreFloat2(this, v); }
};

class Vec3: public dx::XMFLOAT3
{
	friend class Quaternion;
	friend class Mat4x4;

public:
	inline explicit Vec3(const float _x = 1.0f, const float _y = 1.0f, const float _z = 1.0f);

	inline Vec3(const Vec3& copy) = default;
	inline explicit Vec3(const Vec4& copy);

	virtual ~Vec3() = default;

	[[nodiscard]] inline float length() const;
	[[nodiscard]] inline Vec3 normalize() const;
	[[nodiscard]] inline float dotProduct(const Vec3& b) const;
	[[nodiscard]] inline Vec3 crossProduct(const Vec3& b) const;

	[[nodiscard]] inline Vec3 operator-(const Vec3& rhs) const;
	[[nodiscard]] inline Vec3 operator+(const Vec3& rhs) const;
	[[nodiscard]] inline Vec3 operator*(const float& rhs) const;
	[[nodiscard]] inline Vec3 operator/(const float& rhs) const;

	[[nodiscard]] inline dx::XMVECTOR load() const { return dx::XMLoadFloat3(this); }

private:
	inline void store(const dx::XMVECTOR& v) { dx::XMStoreFloat3(this, v); }
};

class Vec4: public dx::XMFLOAT4
{
	friend class Mat4x4;

public:
	inline Vec4(const Vec4& copy) = default;
	inline explicit Vec4(const float _x = 1.0f, const float _y = 1.0f, const float _z = 1.0f,
		const float _w = 1.0f);

	inline explicit Vec4(const Vec3& copy);

	virtual ~Vec4() = default;

	[[nodiscard]] inline float length() const;
	[[nodiscard]] inline Vec4 normalize() const;
	[[nodiscard]] inline float dotProduct(const Vec4& b) const;

	[[nodiscard]] inline dx::XMVECTOR load() const { return dx::XMLoadFloat4(this); }

private:
	inline void store(const dx::XMVECTOR& v) { dx::XMStoreFloat4(this, v); }
};

class Mat4x4: public dx::XMFLOAT4X4
{
public:
	inline Mat4x4();
	inline Mat4x4(const Mat4x4& copy) = default;
	inline explicit Mat4x4(const dx::XMFLOAT4X4& matrix);
	inline explicit Mat4x4(const dx::XMMATRIX& matrix);

	virtual ~Mat4x4() = default;

	inline void setPosition(const Vec3& pos);
	inline void setPosition(const Vec4& pos);
	[[nodiscard]] inline Vec3 getPosition() const;

	[[nodiscard]] inline Vec4 transform(const Vec4& v) const;
	[[nodiscard]] inline Vec3 transform(const Vec3& v) const;

	[[nodiscard]] inline Mat4x4 inverse() const;

	inline void translate(const Vec3& offset);
	inline void translate(const float _x, const float _y, const float _z);

	inline void scale(const Vec3& scale);
	inline void scale(const float _x, const float _y, const float _z);

	inline void rotateX(const float radians);
	inline void rotateY(const float radians);
	inline void rotateZ(const float radians);

	inline void rotateAboutAxis(const Vec3& axis, const float radians);

	inline void buildRotationYawPitchRoll(const float yawRadians, const float pitchRadians,
		const float rollRadians);
	inline void buildRotationQuaternion(const Quaternion& q);
	inline void buildRotationLookAt(const Vec3& eye, const Vec3& at, const Vec3& up);

	[[nodiscard]] inline Mat4x4 transpose() const;

	[[nodiscard]] inline Mat4x4 operator*(const Mat4x4& rhs) const;

	[[nodiscard]] inline dx::XMMATRIX load() const { return dx::XMLoadFloat4x4(this); }

	[[nodiscard]] static inline Mat4x4* identity() { return new (std::nothrow) Mat4x4(); }

private:
	inline void store(const dx::XMMATRIX& _m) { dx::XMStoreFloat4x4(this, _m); }
};

class Quaternion: public dx::XMFLOAT4
{
public:
	inline Quaternion();
	inline Quaternion(const Quaternion& copy) = default;

	virtual ~Quaternion() = default;

	inline void normalize();
	inline void slerp(const Quaternion& begin, const Quaternion& end, const float percent);
	inline void getAxisAngle(Vec3& axis, float& angle) const;

	inline void buildRotationYawPitchRoll(const float yawRadians, const float pitchRadians,
		const float rollRadians);
	inline void buildAxisAngle(const Vec3& axis, const float radians);
	inline void build(const Mat4x4& matrix);

	[[nodiscard]] inline Quaternion operator*(const Quaternion& rhs) const;

	[[nodiscard]] inline dx::XMVECTOR load() const { return dx::XMLoadFloat4(this); }

private:
	inline void store(const dx::XMVECTOR& q) { dx::XMStoreFloat4(this, q); }
};

class Plane: public dx::XMFLOAT4
{
public:
	inline Plane();
	inline explicit Plane(const Vec3& p0, const Vec3& p1, const Vec3& p2);

	virtual ~Plane() = default;

	inline void initialize(const Vec3& p0, const Vec3& p1, const Vec3& p2);

	inline void normalize();
	[[nodiscard]] inline bool contains(const Vec3& point, const float radius) const;
	[[nodiscard]] inline bool contains(const Vec3& point) const;

	[[nodiscard]] inline dx::XMVECTOR load() const { return dx::XMLoadFloat4(this); }

private:
	inline void store(const dx::XMVECTOR& p) { dx::XMStoreFloat4(this, p); }
};

const float default_field_of_view =
	dx::XM_PI / 4.0f;                      // default field of view is 90 degrees (PI/4 rads)
const float default_aspect = 1.0f / 1.0f;  // default aspect ratio is 1:1
const float default_near_dist = 1.0f;      // default near plane is 1m away from the camera
const float default_far_dist = 1000.0f;    // default far plane is 1km away from the camera

class Frustum
{
public:
	enum class Side
	{
		Near = 0,
		Far,
		Top,
		Right,
		Bottom,
		Left,
		NumPlanes
	};

	inline Frustum();
	inline explicit Frustum(const float fieldOfView, const float aspect, const float nearDist,
		const float farDist);

	virtual ~Frustum() = default;

	inline void initialize(const float fieldOfView, const float aspect, const float nearDist,
		const float farDist);

	[[nodiscard]] inline bool contains(const Vec3& point) const;
	[[nodiscard]] inline bool contains(const Vec3& point, float radius) const;

	[[nodiscard]] inline const Plane& getSide(Side side) const;

	inline void setFieldOfView(float fieldOfView);
	inline void setAspect(float aspect);
	inline void setNearDistance(float nearClipDist);
	inline void setFarDistance(float farClipDist);

	inline void render();

	[[nodiscard]] inline float getFieldOfView() const { return this->_fieldOfView; }
	[[nodiscard]] inline float getAspect() const { return this->_aspect; }
	[[nodiscard]] inline float getNearDistance() const { return this->_nearDist; }
	[[nodiscard]] inline float getFarDistance() const { return this->_farDist; }

private:
	std::array<Plane, static_cast<unsigned int>(Side::NumPlanes)> _planes;
	std::array<Vec3, 4> _nearClip;
	std::array<Vec3, 4> _farClip;

	float _fieldOfView {default_field_of_view};
	float _aspect {default_aspect};
	float _nearDist {default_near_dist};
	float _farDist {default_far_dist};
};

// Vec2
inline Vec2::Vec2(const float _x, const float _y): dx::XMFLOAT2(_x, _y)
{}

inline float Vec2::length() const
{
	float length = 0.0f;
	dx::XMStoreFloat(&length, dx::XMVector2Length(load()));
	return length;
}

inline Vec2 Vec2::normalize() const
{
	Vec2 norm;
	norm.store(dx::XMVector2Normalize(load()));
	return norm;
}

inline float Vec2::dotProduct(const Vec2& b) const
{
	float dot = 0.0f;
	dx::XMStoreFloat(&dot, dx::XMVector2Dot(load(), b.load()));
	return dot;
}

inline Vec2 Vec2::crossProduct(const Vec2& b) const
{
	Vec2 cross;
	cross.store(dx::XMVector2Cross(load(), b.load()));
	return cross;
}

inline Vec2 Vec2::operator-(const Vec2& rhs) const
{
	return Vec2(this->x - rhs.x, this->y - rhs.y);
}

inline Vec2 Vec2::operator+(const Vec2& rhs) const
{
	return Vec2(this->x + rhs.x, this->y + rhs.y);
}

inline Vec2 Vec2::operator*(const float& rhs) const
{
	return Vec2(this->x * rhs, this->y * rhs);
}

inline Vec2 Vec2::operator/(const float& rhs) const
{
	return Vec2(this->x / rhs, this->y / rhs);
}

// Vec3
inline Vec3::Vec3(const float _x, const float _y, const float _z): dx::XMFLOAT3(_x, _y, _z)
{}

inline Vec3::Vec3(const Vec4& copy): dx::XMFLOAT3(copy.x, copy.y, copy.z)
{}

inline float Vec3::length() const
{
	float length = 0.0f;
	dx::XMStoreFloat(&length, dx::XMVector3Length(load()));
	return length;
}

inline Vec3 Vec3::normalize() const
{
	Vec3 vec;
	vec.store(dx::XMVector3Normalize(load()));
	return vec;
}

inline float Vec3::dotProduct(const Vec3& b) const
{
	float dot = 0.0f;
	dx::XMStoreFloat(&dot, dx::XMVector3Dot(load(), b.load()));
	return dot;
}

inline Vec3 Vec3::crossProduct(const Vec3& b) const
{
	Vec3 vec;
	vec.store(dx::XMVector3Cross(load(), b.load()));
	return vec;
}

inline Vec3 Vec3::operator-(const Vec3& rhs) const
{
	return Vec3(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
}

inline Vec3 Vec3::operator+(const Vec3& rhs) const
{
	return Vec3(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
}

inline Vec3 Vec3::operator*(const float& rhs) const
{
	return Vec3(this->x * rhs, this->y * rhs, this->z * rhs);
}

inline Vec3 Vec3::operator/(const float& rhs) const
{
	return Vec3(this->x / rhs, this->y / rhs, this->z / rhs);
}

// Vec4
inline Vec4::Vec4(const float _x, const float _y, const float _z, const float _w)
	: dx::XMFLOAT4(_x, _y, _z, _w)
{}

inline Vec4::Vec4(const Vec3& copy): Vec4(copy.x, copy.y, copy.z, 1.0f)
{}

inline float Vec4::length() const
{
	float length = 0.0f;
	dx::XMStoreFloat(&length, dx::XMVector4Length(load()));
	return length;
}

inline Vec4 Vec4::normalize() const
{
	Vec4 vec;
	XMStoreFloat4(&vec, dx::XMVector4Normalize(load()));
	return vec;
}

inline float Vec4::dotProduct(const Vec4& b) const
{
	float dot = 0.0f;
	dx::XMStoreFloat(&dot, dx::XMVector4Dot(load(), b.load()));
	return dot;
}

// Mat4x4
inline Mat4x4::Mat4x4(): dx::XMFLOAT4X4()
{
	store(dx::XMMatrixIdentity());
}

inline Mat4x4::Mat4x4(const dx::XMFLOAT4X4& matrix): dx::XMFLOAT4X4(matrix)
{}

inline Mat4x4::Mat4x4(const dx::XMMATRIX& matrix): dx::XMFLOAT4X4()
{
	this->store(matrix);
}

inline void Mat4x4::setPosition(const Vec3& pos)
{
	m[3][0] = pos.x;
	m[3][1] = pos.y;
	m[3][2] = pos.z;
	m[3][3] = 1.0f;
}

inline void Mat4x4::setPosition(const Vec4& pos)
{
	m[3][0] = pos.x;
	m[3][1] = pos.y;
	m[3][2] = pos.z;
	m[3][3] = pos.w;
}

inline Vec3 Mat4x4::getPosition() const
{
	return Vec3(m[3][0], m[3][1], m[3][2]);
}

inline Vec4 Mat4x4::transform(const Vec4& v) const
{
	Vec4 trans;
	trans.store(XMVector4Transform(v.load(), load()));
	return trans;
}

inline Vec3 Mat4x4::transform(const Vec3& v) const
{
	Vec4 temp(v);
	Vec4 trans;
	trans.store(XMVector4Transform(temp.load(), load()));
	return Vec3(trans);
}

inline Mat4x4 Mat4x4::inverse() const
{
	Mat4x4 invert;
	invert.store(XMMatrixInverse(nullptr, load()));
	return invert;
}

inline Mat4x4 Mat4x4::transpose() const
{
	Mat4x4 transposed;
	transposed.store(XMMatrixTranspose(this->load()));
	return transposed;
}

inline void Mat4x4::translate(const Vec3& offset)
{
	store(dx::XMMatrixMultiply(load(), dx::XMMatrixTranslationFromVector(offset.load())));
}

inline void Mat4x4::translate(const float _x, const float _y, const float _z)
{
	store(dx::XMMatrixMultiply(load(), dx::XMMatrixTranslation(_x, _y, _z)));
}

inline void Mat4x4::scale(const Vec3& scale)
{
	store(dx::XMMatrixMultiply(load(), dx::XMMatrixScalingFromVector(scale.load())));
}

inline void Mat4x4::scale(const float _x, const float _y, const float _z)
{
	store(dx::XMMatrixMultiply(load(), dx::XMMatrixScaling(_x, _y, _z)));
}

inline void Mat4x4::rotateX(const float radians)
{
	store(dx::XMMatrixMultiply(load(), dx::XMMatrixRotationX(radians)));
}

inline void Mat4x4::rotateY(const float radians)
{
	store(dx::XMMatrixMultiply(load(), dx::XMMatrixRotationY(radians)));
}

inline void Mat4x4::rotateZ(const float radians)
{
	store(dx::XMMatrixMultiply(load(), dx::XMMatrixRotationZ(radians)));
}

inline void Mat4x4::rotateAboutAxis(const Vec3& axis, const float radians)
{
	store(dx::XMMatrixMultiply(load(), dx::XMMatrixRotationAxis(axis.load(), radians)));
}

inline void Mat4x4::buildRotationYawPitchRoll(const float yawRadians, const float pitchRadians,
	const float rollRadians)
{
	store(dx::XMMatrixRotationRollPitchYaw(pitchRadians, yawRadians, rollRadians));
}

inline void Mat4x4::buildRotationQuaternion(const Quaternion& q)
{
	store(dx::XMMatrixRotationQuaternion(q.load()));
}

inline void Mat4x4::buildRotationLookAt(const Vec3& eye, const Vec3& at, const Vec3& up)
{
	store(dx::XMMatrixLookAtRH(eye.load(), at.load(), up.load()));
}

inline Mat4x4 Mat4x4::operator*(const Mat4x4& rhs) const
{
	Mat4x4 mult(dx::XMMatrixMultiply(load(), rhs.load()));
	return mult;
}

// Quaternion
inline Quaternion::Quaternion(): XMFLOAT4()
{
	store(dx::XMQuaternionIdentity());
}

inline void Quaternion::normalize()
{
	store(dx::XMQuaternionNormalize(load()));
}

inline void Quaternion::slerp(const Quaternion& begin, const Quaternion& end, const float percent)
{
	// performs spherical linear interpolation between begin and end
	// Note: set percent = [0.0f, 1.0f]
	store(dx::XMQuaternionSlerp(begin.load(), end.load(), percent));
}

inline void Quaternion::getAxisAngle(Vec3& axis, float& angle) const
{
	auto loadedAxis = axis.load();
	dx::XMQuaternionToAxisAngle(&loadedAxis, &angle, load());
	axis.store(loadedAxis);
}

inline void Quaternion::buildRotationYawPitchRoll(const float yawRadians, const float pitchRadians,
	const float rollRadians)
{
	store(dx::XMQuaternionRotationRollPitchYaw(pitchRadians, yawRadians, rollRadians));
}

inline void Quaternion::buildAxisAngle(const Vec3& axis, const float radians)
{
	store(dx::XMQuaternionRotationAxis(axis.load(), radians));
}

inline void Quaternion::build(const Mat4x4& matrix)
{
	store(dx::XMQuaternionRotationMatrix(matrix.load()));
}

inline Quaternion Quaternion::operator*(const Quaternion& rhs) const
{
	// for rotations, this is exactly like concatenating matrices
	// the new quaternion represents rotation A followed by rotation B
	Quaternion mul;
	mul.store(dx::XMQuaternionMultiply(load(), rhs.load()));
	return mul;
}

// Plane
inline Plane::Plane(): dx::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)
{}

inline Plane::Plane(const Vec3& p0, const Vec3& p1, const Vec3& p2): dx::XMFLOAT4()
{
	initialize(p0, p1, p2);
}

inline void Plane::initialize(const Vec3& p0, const Vec3& p1, const Vec3& p2)
{
	store(dx::XMPlaneFromPoints(p0.load(), p1.load(), p2.load()));
	normalize();
}

inline void Plane::normalize()
{
	store(dx::XMPlaneNormalize(load()));
}

inline bool Plane::contains(const Vec3& point) const
{
	// inside the plane is defined as the direction the normal is facing
	float distance = 0.0f;
	dx::XMStoreFloat(&distance, dx::XMPlaneDotCoord(load(), point.load()));
	return (distance >= 0.0f);
}

inline bool Plane::contains(const Vec3& point, const float radius) const
{
	float distance = 0.0f;
	dx::XMStoreFloat(&distance, dx::XMPlaneDotCoord(load(), point.load()));

	// if this distance is < -radius, we are outside
	return (distance >= -radius);
}

// Frustum
inline Frustum::Frustum()
{
	_fieldOfView = default_field_of_view;
	_aspect = default_aspect;
	_nearDist = default_near_dist;
	_farDist = default_far_dist;

	initialize(_fieldOfView, _aspect, _nearDist, _farDist);
}

inline Frustum::Frustum(const float fieldOfView, const float aspect, const float nearDist,
	const float farDist)
{
	initialize(fieldOfView, aspect, nearDist, farDist);
}

inline void Frustum::initialize(const float fieldOfView, const float aspect, const float nearDist,
	const float farDist)
{
	_fieldOfView = fieldOfView;
	_aspect = aspect;
	_nearDist = nearDist;
	_farDist = farDist;

	const double tanFovOver2 = tan(static_cast<double>(_fieldOfView) / 2.0);
	Vec3 nearRight(static_cast<float>(_nearDist * tanFovOver2 * _aspect), 0.0f, 0.0f);
	Vec3 farRight(static_cast<float>(_farDist * tanFovOver2 * _aspect), 0.0f, 0.0f);
	Vec3 nearUp(0.0f, static_cast<float>(_nearDist * tanFovOver2), 0.0f);
	Vec3 farUp(0.0f, static_cast<float>(_farDist * tanFovOver2), 0.0f);

	// points start in the upper left and go around clockwise
	_nearClip[0] = Vec3(0.0f, 0.0f, _nearDist) - nearRight + nearUp;
	_nearClip[1] = Vec3(0.0f, 0.0f, _nearDist) + nearRight + nearUp;
	_nearClip[2] = Vec3(0.0f, 0.0f, _nearDist) + nearRight - nearUp;
	_nearClip[3] = Vec3(0.0f, 0.0f, _nearDist) - nearRight - nearUp;

	_farClip[0] = Vec3(0.0f, 0.0f, _farDist) - farRight + farUp;
	_farClip[1] = Vec3(0.0f, 0.0f, _farDist) + farRight + farUp;
	_farClip[2] = Vec3(0.0f, 0.0f, _farDist) + farRight - farUp;
	_farClip[3] = Vec3(0.0f, 0.0f, _farDist) - farRight - farUp;

	// now that we have all 8 points, we can create the 6 planes
	Vec3 origin(0.0f, 0.0f, 0.0f);
	_planes[static_cast<unsigned int>(Side::Near)].initialize(_nearClip[2], _nearClip[1],
		_nearClip[0]);
	_planes[static_cast<unsigned int>(Side::Far)].initialize(_farClip[0], _farClip[1], _farClip[2]);
	_planes[static_cast<unsigned int>(Side::Right)].initialize(_farClip[2], _farClip[1], origin);
	_planes[static_cast<unsigned int>(Side::Top)].initialize(_farClip[1], _farClip[0], origin);
	_planes[static_cast<unsigned int>(Side::Left)].initialize(_farClip[0], _farClip[3], origin);
	_planes[static_cast<unsigned int>(Side::Bottom)].initialize(_farClip[3], _farClip[2], origin);
}

inline bool Frustum::contains(const Vec3& point) const
{
	return std::ranges::all_of(_planes.begin(), _planes.end(),
		[&](const Plane& plane) { return plane.contains(point); });
}

inline bool Frustum::contains(const Vec3& point, float radius) const
{
	return std::ranges::all_of(_planes.begin(), _planes.end(),
		[&](const Plane& plane) { return plane.contains(point, radius); });
}

inline const Plane& Frustum::getSide(Side side) const
{
	auto index = static_cast<unsigned int>(side);
	return _planes.at(index);
}

inline void Frustum::setFieldOfView(float fieldOfView)
{
	_fieldOfView = fieldOfView;
	initialize(_fieldOfView, _aspect, _nearDist, _farDist);
}

inline void Frustum::setAspect(float aspect)
{
	_aspect = aspect;
	initialize(_fieldOfView, _aspect, _nearDist, _farDist);
}

inline void Frustum::setNearDistance(float nearClipDist)
{
	_nearDist = nearClipDist;
	initialize(_fieldOfView, _aspect, _nearDist, _farDist);
}

inline void Frustum::setFarDistance(float farClipDist)
{
	_farDist = farClipDist;
	initialize(_fieldOfView, _aspect, _nearDist, _farDist);
}

inline void Frustum::render()
{}
