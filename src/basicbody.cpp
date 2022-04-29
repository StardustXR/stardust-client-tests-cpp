#include <chrono>
#include <iostream>
#include <thread>

#include <stardustxr/fusion/values/glm.hpp>
#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/drawable/model.hpp>

#include "physics/physicsbody.hpp"

using namespace StardustXRFusion;

int physicsRate = 60;
int64_t physicsStepMS = 1000 / physicsRate;
double delta = 1.0 / physicsRate;

int main(int, char *[]) {
	StardustXRFusion::Setup();
	perror("borken demo");
	return 1;

	// float cubeSize = 0.08;
	// Model objectModel(nullptr, "res/realitycomposer.glb", Vec3::Zero, Quat::Identity, Vec3::One * cubeSize);
	// PhysicsBody objectPhysics;

	// Vec3 boostVelocity = -Vec3::Right * 0.5f;

	// while(1) {
	// 	if(glm::length(objectPhysics.velocity) == 0) {
	// 		boostVelocity = -boostVelocity;
	// 		objectPhysics.addImpulse(boostVelocity);
	// 	}

	// 	objectPhysics.step(delta);
	// 	objectModel.setOrigin(objectPhysics.position);

	// 	std::this_thread::sleep_for(std::chrono::milliseconds(physicsStepMS));
	// }
}
