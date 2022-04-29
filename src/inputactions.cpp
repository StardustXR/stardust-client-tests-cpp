#include <vector>

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/drawable/model.hpp>
#include <stardustxr/fusion/types/fields/boxfield.hpp>
#include <stardustxr/fusion/types/input/inputactionhandler.hpp>
#include <stardustxr/fusion/types/input/types/handinput.hpp>
#include <stardustxr/fusion/types/input/types/pointerinput.hpp>

using namespace StardustXRFusion;


int main(int, char *[]) {
	if(!StardustXRFusion::Setup())
		return 1;
	Spatial root(Root(), Vec3::Forward*0.5f, Quat::Identity, Vec3::One, true, true, false);

	float size = 0.1f;

	BoxField field(&root, Vec3::Zero, Quat::Identity, Vec3::One * size);
	InputActionHandler handler(&root, field, Vec3::Zero, Quat::Identity);

	InputActionHandler::Action action(false);
	action.pointerActiveCondition = [](const std::string uuid, const PointerInput &pointer, const Datamap &datamap){
		return datamap.getFloat("select") > 0.5f;
	};
	action.handActiveCondition = [](const std::string uuid, const HandInput &hand, const Datamap &datamap){
		return datamap.getFloat("pinchStrength") > 0.5f;
	};
	handler.actions.push_back(&action);

	OnLogicStep([&](double delta, double) {
		handler.update();
		for(InputActionHandler::InputMethod &inputMethod : action.startedActing) {
			printf("Input handler \"%s\" just got in range!\n", inputMethod.uuid.c_str());
		}
		fflush(stdout);
	});
	StardustXRFusion::RunEventLoop();
}
