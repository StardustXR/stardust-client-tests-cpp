#include "physicsbody.hpp"

using namespace SKMath;

PhysicsBody::PhysicsBody() {}

void PhysicsBody::addImpulse(SKMath::vec3 impulse) {
	velocity += impulse;
}

void PhysicsBody::addForce(SKMath::vec3 force) {
	velocity += force * lastDelta;
}

void PhysicsBody::step(double delta) {
	lastDelta = delta;

	position += velocity * delta;
	dragStep(delta);
}

void PhysicsBody::dragStep(double delta) {
	float vel_sq = vec3_magnitude_sq(velocity);
	if(vel_sq > drag * drag * delta * delta)
		velocity -= (velocity/sqrtf(vel_sq)) * drag * delta;
	else
		velocity = vec3_zero;
}
