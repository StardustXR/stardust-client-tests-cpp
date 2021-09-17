#include <stardustxr/fusion/types/fields/boxfield.hpp>

#include "interaction/screen.hpp"

using namespace StardustXRFusion;
using namespace SKMath;

int main(int, char *[]) {
	StardustXRFusion::Setup();

	BoxField screenField(vec3_forward * 0.5f, quat_identity, {0.7f, 0.4f, 0.01f});

	Screen discordScreen(vec3_forward * 0.5f, quat_identity, "../res/discord_panel.glb", 1.0f, screenField);
	LifeCycle()->onLogicStep([&](double, double) {
		discordScreen.update();
	});
	StardustXRFusion::ShutdownAfterInput();
}
