#include <chrono>
#include <iostream>
#include <thread>

#include <flatbuffers/flexbuffers.h>

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/model.hpp>
#include <stardustxr/fusion/types/fields/boxfield.hpp>
#include <stardustxr/fusion/types/input/inputhandler.hpp>

#include "interaction/screen.hpp"

using namespace StardustXRFusion;
using namespace SKMath;

int main(int, char *[]) {
	StardustXRFusion::Setup();

	BoxField screenField(vec3_zero, quat_identity, {0.7f, 0.4f, 0.01f});

	Screen discordScreen(vec3_zero, quat_from_angles(0, 0, 0), "../res/discord_panel.glb", 1.0f, screenField);

	std::this_thread::sleep_for(std::chrono::seconds(3600));
}
