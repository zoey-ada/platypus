#pragma once

#include <utilities/math/mathTypes.hpp>

class RigidBodyObject;

struct Manifold
{
	RigidBodyObject* obj_a {nullptr};
	RigidBodyObject* obj_b {nullptr};
	Vec3 normal {0.0f, 0.0f, 0.0f};
	float penetration {0.0f};
};

class Collision
{
public:
	Collision(Manifold collision_data);
	virtual ~Collision() = default;

	void resolve();

	void resolveOverlap();
	void resolveForces();
	bool collisionIsSignificant();

protected:
	RigidBodyObject* _obj_a {nullptr};
	RigidBodyObject* _obj_b {nullptr};
	Vec3 _normal {Vec3(0.0f, 0.0f, 0.0f)};
	float _penetration {0.0f};

	float _ratio_a {0.0f};
	float _ratio_b {0.0f};
};
