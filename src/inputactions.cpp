#include <vector>

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/drawable/model.hpp>
#include <stardustxr/fusion/types/fields/boxfield.hpp>
#include <stardustxr/fusion/types/input/inputactionhandler.hpp>
#include <stardustxr/fusion/types/input/types/handinput.hpp>
#include <stardustxr/fusion/types/input/types/pointerinput.hpp>

using namespace StardustXRFusion;
using namespace SKMath;

int main(int, char *[]) {
	if(!StardustXRFusion::Setup())
		return 1;
	Spatial root = Spatial::create(nullptr, vec3_forward*0.5f, quat_identity, vec3_one, true, true, false);

	float size = 0.1f;

	BoxField field(&root, vec3_zero, quat_identity, vec3_one * size);
	InputActionHandler handler(&root, field, vec3_zero, quat_identity);
	handler.actions.push_back(InputActionHandler::Action{
		false,
		[](const std::string uuid, const PointerInput &pointer, const Datamap &datamap){
			return datamap.getFloat("select") > 0.5f;
		},
		[](const std::string uuid, const HandInput &hand, const Datamap &datamap){
			return datamap.getFloat("pinchStrength") > 0.5f;
		}
	});
	InputActionHandler::Action *action = &handler.actions[0];

	OnLogicStep([&](double delta, double) {
		handler.update();
		for(InputActionHandler::InputMethod &inputMethod : action->startedActing) {
			printf("Input handler \"%s\" just got in range!\n", inputMethod.uuid.c_str());
		}
		fflush(stdout);
	});
	StardustXRFusion::StallMainThread();
}
