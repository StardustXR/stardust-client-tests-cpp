#include "../../include/math_util.hpp"

#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/input/datamap.hpp>
#include <stardustxr/fusion/types/input/types/handinput.hpp>
#include <stardustxr/fusion/types/input/types/pointerinput.hpp>
#include <stardustxr/fusion/types/spatial/spatial.hpp>

#include "grabbable.hpp"
#include "xinteract.hpp"

using namespace SKMath;
using namespace StardustXRFusion;

Grabbable::Grabbable(Spatial root, const Field field, float maxDistance) :
Spatial(root),
maxDistance(maxDistance),
field(field),
inputHandler(Root(), this->field, vec3_zero, quat_identity),
grabSpace(&inputHandler),
scrollSpace(&grabSpace) {

//	inRangeAction.captureOnTrigger = false;
//	inRangeAction.pointerActiveCondition = [&](const std::string, const PointerInput &, const Datamap &) { return true; };
//	inRangeAction.handActiveCondition = [&](const std::string, const HandInput &, const Datamap &) { return true; };
//	inputHandler.actions.push_back(&inRangeAction);

	grabAction.captureOnTrigger = true;
	grabAction.pointerActiveCondition = std::bind(&Grabbable::pointerGrabbingCondition, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	grabAction.handActiveCondition = std::bind(&Grabbable::handGrabbingCondition, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	inputHandler.actions.push_back(&grabAction);

	setSpatialParent(&inputHandler);
}
Grabbable::Grabbable(SKMath::vec3 origin, SKMath::quat orientation, const StardustXRFusion::Field field, float maxDistance) :
		Grabbable(Spatial(Root(), origin, orientation, vec3_one, true, true, false, true), field, maxDistance) {}

bool Grabbable::pointerGrabbingCondition(const std::string uuid, const PointerInput &pointer, const Datamap &datamap) {
	bool previouslyGrabbed = std::find(grabAction.activelyActing.begin(), grabAction.activelyActing.end(), uuid) != grabAction.activelyActing.end();
	if(!previouslyGrabbed && pointer.distance > maxDistance)
		return false;

	const float context = datamap.getFloat("context");
	return context > 0.9f;
}
bool Grabbable::handGrabbingCondition(const std::string uuid, const HandInput &hand, const Datamap &datamap) {
	bool previouslyGrabbed = std::find(grabAction.activelyActing.begin(), grabAction.activelyActing.end(), uuid) != grabAction.activelyActing.end();
	if(!previouslyGrabbed && hand.distance > maxDistance)
		return false;

//	bool previouslyInRange = std::find(inRangeAction->activelyActing.begin(), inRangeAction->activelyActing.end(), uuid) != inRangeAction->activelyActing.end();
	const float pinchStrength = datamap.getFloat("pinchStrength");
	const float grabStrength = datamap.getFloat("grabStrength");
	return /*previouslyInRange &&*/ pinchStrength > 0.9f && grabStrength < 0.1f;
}

void Grabbable::update() {
	inputHandler.update();
	if(grabAction.startedActing.size() > 0) {
		if(grabbingInputUUID == "")
			onStartedGrabbing();
		grabbingInputUUID = grabAction.startedActing.begin()->uuid;
		setSpatialParentInPlace(Root());
		setZoneable(false); // If anything is actively grabbing this, it shouldn't be zoneable
	} else if(std::find(grabAction.stoppedActing.begin(), grabAction.stoppedActing.end(), grabbingInputUUID) != grabAction.stoppedActing.end()) {
		grabbingInputUUID = "";
		setSpatialParentInPlace(Root());
		setZoneable(true);
		onStoppedGrabbing();
	}

	InputActionHandler::InputMethod *grabbingInput = std::find(grabAction.activelyActing.begin(), grabAction.activelyActing.end(), grabbingInputUUID).base();
	if(grabbingInput) {
		if(grabbingInput->hand.get()) {
			grabSpace.setPose(pose_t{
				(grabbingInput->hand->thumb().tip().pose.position +
				 grabbingInput->hand->index().tip().pose.position) * 0.5f,
				grabbingInput->hand->palm.pose.orientation
			});
		} else if(grabbingInput->pointer.get()) {
			grabSpace.setPose(pose_t{
				grabbingInput->pointer->origin,
				quat_lookat(vec3_zero, grabbingInput->pointer->direction)
			});
			if(grabbingInput->datamap.exists("scroll")) {
				vec2 scroll = grabbingInput->datamap.getVec2("scroll");
				scrollDistance += scroll.y * 0.05f;
				scrollSpace.setOrigin(vec3_forward * scrollDistance);
			}
		}
	}
	if(grabAction.startedActing.size() > 0)
		setSpatialParentInPlace(&scrollSpace);
}
