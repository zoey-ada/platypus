#pragma once

class RigidBodyObject;

class Collision
{
public:
	Collision(RigidBodyObject* obj_a, RigidBodyObject* obj_b);
	virtual ~Collision() = default;

	virtual void resolve() = 0;
	virtual void resolveOverlap() = 0;
	virtual void resolveForces() = 0;

	virtual bool passesThreshold() = 0;

protected:
	RigidBodyObject* _obj_a {nullptr};
	RigidBodyObject* _obj_b {nullptr};

	float _ratio_a {0.0f};
	float _ratio_b {0.0f};
};

class RectangleRectangleCollision: public Collision
{
public:
	RectangleRectangleCollision(RigidBodyObject* obj_a, RigidBodyObject* obj_b, float overlap_x,
		float overlap_y);

	void resolve() override;
	void resolveOverlap() override;
	void resolveForces() override;

	bool passesThreshold() override;

private:
	float _overlap_x;
	float _overlap_y;
};
