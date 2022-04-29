#pragma once

#include <stardustxr/fusion/values/glm.hpp>
#include <stardustxr/fusion/types/spatial/spatial.hpp>

class PhysicsBody {
public:
	explicit PhysicsBody();
	virtual ~PhysicsBody();

	StardustXRFusion::Spatial *space = nullptr;

	StardustXRFusion::Vec3 position = StardustXRFusion::Vec3::Zero;
	StardustXRFusion::Vec3 velocity = StardustXRFusion::Vec3::Zero;

	float drag = 0.4f;

	void addImpulse(StardustXRFusion::Vec3 impulse);
	void addForce(StardustXRFusion::Vec3 force);

	virtual void step(float delta);

	float lastDelta = 0;

protected:
	virtual void dragStep(float delta);
};
