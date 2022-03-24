#include <sys/time.h>
#include <map>

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/fields/boxfield.hpp>
#include <stardustxr/fusion/types/input/inputactionhandler.hpp>

using namespace StardustXRFusion;
using namespace SKMath;

int main() {
	StardustXRFusion::Setup();

	timeval tv;
	std::map<std::string, timeval> checkTimes;

	BoxField box(StardustXRFusion::Root(), vec3_zero, quat_identity, vec3_one * 0.25f);
	InputActionHandler inputHandler(StardustXRFusion::Root(), box, vec3_zero, quat_identity);

	InputActionHandler::Action action(false);
	action.pointerActiveCondition = [&](const std::string uuid, const PointerInput &pointer, const Datamap &datamap){
		gettimeofday(&tv, nullptr);
		checkTimes[uuid] = tv;
		return pointer.distance < 0;
	};
	action.handActiveCondition = [&](const std::string uuid, const HandInput &hand, const Datamap &datamap){
		gettimeofday(&tv, nullptr);
		checkTimes[uuid] = tv;
		return hand.distance < 0;
	};
	inputHandler.actions.push_back(&action);

	StardustXRFusion::OnLogicStep([&](double, double) {
		inputHandler.update();
		for(InputActionHandler::InputMethod &input : action.startedActing) {
			timeval t2;
			gettimeofday(&t2, nullptr);
			timeval t1 = checkTimes[input.uuid];

			long msec[2] = {
				(t1.tv_sec * 1000) + (t1.tv_usec / 1000),
				(t2.tv_sec * 1000) + (t2.tv_usec / 1000),
			};

			printf("Time from checking input to update is %li ms\n", msec[1] - msec[0]);
		}
	});

	StardustXRFusion::StallMainThread();
}
