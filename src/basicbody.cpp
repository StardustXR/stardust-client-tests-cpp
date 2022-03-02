#include <chrono>
#include <iostream>
#include <thread>

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/drawable/model.hpp>

#include "physics/physicsbody.hpp"

using namespace StardustXRFusion;
using namespace SKMath;

int physicsRate = 60;
int64_t physicsStepMS = 1000 / physicsRate;
double delta = 1.0 / physicsRate;

int main(int, char *[]) {
	StardustXRFusion::Setup();

	float cubeSize = 0.08;
	Model objectModel(nullptr, "../res/realitycomposer.glb", vec3_zero, quat_identity, vec3_one * cubeSize);
	PhysicsBody objectPhysics;

	vec3 boostVelocity = -vec3_right * 0.5f;

	while(1) {
		if(vec3_magnitude_sq(objectPhysics.velocity) == 0) {
			boostVelocity = -boostVelocity;
			objectPhysics.addImpulse(boostVelocity);
		}

		objectPhysics.step(delta);
		objectModel.setOrigin(objectPhysics.position);

		std::this_thread::sleep_for(std::chrono::milliseconds(physicsStepMS));
	}
}
