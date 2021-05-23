#pragma once

#include <stardustxr/fusion/types/spatial.hpp>

class PhysicsBody {
public:
	explicit PhysicsBody();

	StardustXRFusion::Spatial *space = nullptr;

	SKMath::vec3 position = SKMath::vec3_zero;
	SKMath::vec3 velocity = SKMath::vec3_zero;

	float drag = 0.4f;

	void addImpulse(SKMath::vec3 impulse);
	void addForce(SKMath::vec3 force);

	virtual void step(double delta);

	double lastDelta = 0;

protected:
	virtual void dragStep(double delta);
};
