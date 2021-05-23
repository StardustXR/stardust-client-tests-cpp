#include <chrono>
#include <iostream>
#include <thread>

#include <flatbuffers/flexbuffers.h>

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/model.hpp>
#include <stardustxr/fusion/types/fields/boxfield.hpp>
#include <stardustxr/fusion/types/input/inputhandler.hpp>

#include "physics/physicsbody.hpp"
#include "interaction/contactable.hpp"

using namespace StardustXRFusion;
using namespace SKMath;

float cubeSize = 0.08f;

int main(int, char *[]) {
	StardustXRFusion::Setup();

	BoxField objectField(vec3_zero, quat_identity, vec3_one * cubeSize);

	Contactable pushCube(vec3_zero, quat_identity, "../res/realitycomposer.glb", cubeSize, &objectField);

	LifeCycle()->onLogicStep([&](double delta, double) {
		pushCube.step(delta);
	});
	std::this_thread::sleep_for(std::chrono::seconds(3600));
}
