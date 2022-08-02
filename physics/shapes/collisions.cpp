#include "collisions.hpp"

#include "../rigidBodyObject.hpp"

const float collision_threshold = 0.000001f;

Collision::Collision(Manifold manifold)
	: _obj_a(manifold.obj_a),
	  _obj_b(manifold.obj_b),
	  _normal(manifold.normal),
	  _penetration(manifold.penetration)
{
	float total_mass = this->_obj_a->getMass() + this->_obj_b->getMass();
	this->_ratio_a = this->_obj_a->getMass() / total_mass;
	this->_ratio_b = this->_obj_b->getMass() / total_mass;
}

void Collision::resolve()
{
	if (!this->collisionIsSignificant() || !this->_obj_a->isSolid() || !this->_obj_b->isSolid())
	{
		return;
	}

	this->resolveForces();
	this->resolveOverlap();

	// trigger collision event
}

void Collision::resolveForces()
{
	float inverse_mass_a = this->_obj_a->getInverseMass();
	float inverse_mass_b = this->_obj_b->getInverseMass();
	auto velocity_a = this->_obj_a->getLinearVelocity();
	auto velocity_b = this->_obj_b->getLinearVelocity();

	auto relative_velocity = velocity_b - velocity_a;
	float velocity_along_normal = relative_velocity.dotProduct(this->_normal);

	if (velocity_along_normal >= 0)
		return;

	float restitution = std::min(this->_obj_a->getRestitution(), this->_obj_b->getRestitution());

	float impulse_scalar = -(1.0f + restitution) * velocity_along_normal;
	impulse_scalar /= (inverse_mass_a + inverse_mass_b);

	auto impulse = this->_normal * impulse_scalar;
	this->_obj_a->setLinearVelocity(this->_obj_a->getLinearVelocity() - impulse * inverse_mass_a);
	this->_obj_b->setLinearVelocity(this->_obj_b->getLinearVelocity() + impulse * inverse_mass_b);
}

void Collision::resolveOverlap()
{
	auto correction = this->_normal * std::max(this->_penetration, 0.0f);
	this->_obj_a->adjustPosition(correction * -this->_ratio_a);
	this->_obj_b->adjustPosition(correction * this->_ratio_b);
}

bool Collision::collisionIsSignificant()
{
	return this->_penetration > collision_threshold;
}
