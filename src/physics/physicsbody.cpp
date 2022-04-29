#include "physicsbody.hpp"

using namespace StardustXRFusion;

PhysicsBody::PhysicsBody() {}
PhysicsBody::~PhysicsBody() {}

void PhysicsBody::addImpulse(Vec3 impulse) {
	velocity = velocity + impulse;
}

void PhysicsBody::addForce(Vec3 force) {
	velocity = velocity + (force * lastDelta);
}

void PhysicsBody::step(float delta) {
	lastDelta = delta;

	position = position + (velocity * delta);
	dragStep(delta);
}

void PhysicsBody::dragStep(float delta) {
	float vel = glm::vec3(velocity).length();
	if(vel > drag * delta)
		velocity = velocity - (velocity/vel) * drag * delta;
	else
		velocity = Vec3::Zero;
}
