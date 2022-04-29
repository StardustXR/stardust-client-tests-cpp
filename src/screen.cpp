#include <stardustxr/fusion/values/glm.hpp>
#include <stardustxr/fusion/types/fields/boxfield.hpp>

#include "interaction/screen.hpp"

using namespace StardustXRFusion;


int main(int, char *[]) {
	StardustXRFusion::Setup();

	BoxField screenField(Root(), Vec3::Forward * 0.5f, Quat::Identity, {0.7f, 0.4f, 0.01f});

	Screen discordScreen(*Root(), Vec3::Forward * 0.5f, Quat::Identity, "res/discord_panel.glb", 1.0f, screenField);
	OnLogicStep([&](double, double) {
		discordScreen.update();
	});
	StardustXRFusion::RunEventLoop();
}
