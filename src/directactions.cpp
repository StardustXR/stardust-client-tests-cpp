#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/input/inputactions.hpp>
#include <stardustxr/fusion/types/input/inputhandler.hpp>
#include <stardustxr/fusion/types/spatial.hpp>

#include <iostream>
#include <string>

using namespace StardustXRFusion;
using namespace SKMath;

std::map<std::string, InputActions> inputs = {};
Spatial *origin;

void updateInputHandlers() {
	inputs.clear();
	InputHandler::getInputHandlers(origin, true, [&](std::vector<InputActions> &basics) {
		printf("\033[2J\033[0;0HInput Handlers:\n");
		for(InputActions &basic : basics) {
			std::string uuid = basic.uuid;
			vec3 pos = basic.position;
			inputs.insert(std::pair(uuid, basic));
			basic.getActions([uuid, pos](std::vector<std::string> &actions) {
				printf("    %s: (%f, %f, %f)\n", uuid.c_str(), pos.x, pos.y, pos.z);
				for(std::string &action : actions) {
					printf("        %s\n", action.c_str());
				}
			});
		}
	});
}

int main(int, char *[]) {
	StardustXRFusion::Setup();
	origin = new Spatial(Spatial::create(nullptr));
	updateInputHandlers();

	std::string uuid;
	std::string action;
	while(std::getline(std::cin, uuid)) {
		if(uuid == "update") {
			updateInputHandlers();
			continue;
		}

		if(inputs.find(uuid) != inputs.end()) {
			std::getline(std::cin, action);
			inputs.at(uuid).runAction(action);
			printf("Running action \"%s\" on input handler with UUID \"%s\"\n", action.c_str(), uuid.c_str());
		}
	}

	StardustXRFusion::StallMainThread();
}
