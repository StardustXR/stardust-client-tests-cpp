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

	inRangeAction.captureOnTrigger = false;
	inRangeAction.pointerActiveCondition = [&](const std::string, const PointerInput &pointer, const Datamap &) { return pointer.distance < maxDistance; };
	inRangeAction.handActiveCondition = [&](const std::string, const HandInput &hand, const Datamap &) { return hand.distance < maxDistance; };
	inputHandler.actions.push_back(&inRangeAction);

	grabAction.captureOnTrigger = true;
	grabAction.pointerActiveCondition = std::bind(&Grabbable::pointerGrabbingCondition, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	grabAction.handActiveCondition = std::bind(&Grabbable::handGrabbingCondition, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	inputHandler.actions.push_back(&grabAction);

	setSpatialParent(&inputHandler);
}
Grabbable::Grabbable(SKMath::vec3 origin, SKMath::quat orientation, const StardustXRFusion::Field field, float maxDistance) :
		Grabbable(Spatial(Root(), origin, orientation, vec3_one, true, true, false, true), field, maxDistance) {}

bool Grabbable::pointerGrabbingCondition(const std::string uuid, const PointerInput &pointer, const Datamap &datamap) {
	const float select = datamap.getFloat("select");
	return grabCondition(uuid, pointer.distance, select);
}
bool Grabbable::handGrabbingCondition(const std::string uuid, const HandInput &hand, const Datamap &datamap) {
	const float pinchStrength = datamap.getFloat("pinchStrength");
	const float grabStrength = datamap.getFloat("grabStrength");
	return grabCondition(uuid, hand.distance, pinchStrength > 0.9f && grabStrength < 0.1f);
}

bool Grabbable::grabCondition(const std::string uuid, float distance, bool gesture) {
	bool previouslyInRange = std::find(inRangeAction.activelyActing.begin(), inRangeAction.activelyActing.end(), uuid) != inRangeAction.activelyActing.end();
	bool previouslyGrabbed = std::find(grabAction.activelyActing.begin(), grabAction.activelyActing.end(), uuid) != grabAction.activelyActing.end();
	bool isImproperlyGrabbed = std::find(improperlyGrabbed.begin(), improperlyGrabbed.end(), uuid) != improperlyGrabbed.end();
	if(!gesture && isImproperlyGrabbed) {
		improperlyGrabbed.erase(std::remove(improperlyGrabbed.begin(), improperlyGrabbed.end(), uuid));
		return false;
	}
	if(!isImproperlyGrabbed && !previouslyInRange && !previouslyGrabbed && gesture) {
		improperlyGrabbed.push_back(uuid);
		return false;
	}

	return (previouslyInRange || previouslyGrabbed) && gesture && !isImproperlyGrabbed;
}

void Grabbable::update() {
	inputHandler.update();
	if(grabAction.actorStarted)
		onStartedGrabbing();
	if(grabAction.actorStopped)
		onStoppedGrabbing();
	if(grabAction.actorStarted || grabAction.actorChanged || grabAction.actorStopped) {
		setSpatialParentInPlace(Root());
		setZoneable(!grabAction.actorActing);
	}

	if(grabAction.actorActing) {
		if(grabAction.actor->hand.get()) {
			grabSpace.setPose(pose_t{
				(grabAction.actor->hand->thumb().tip().pose.position +
				 grabAction.actor->hand->index().tip().pose.position) * 0.5f,
				 grabAction.actor->hand->palm.pose.orientation
			});
		} else if(grabAction.actor->pointer.get()) {
			grabSpace.setPose(pose_t{
				grabAction.actor->pointer->origin,
				quat_lookat(vec3_zero, grabAction.actor->pointer->direction)
			});
			if(grabAction.actor->datamap.exists("scroll")) {
				vec2 scroll = grabAction.actor->datamap.getVec2("scroll");
				scrollDistance += scroll.y * 0.05f;
				scrollSpace.setOrigin(vec3_forward * scrollDistance);
			}
		}
	}
	if(grabAction.actorStarted || grabAction.actorChanged)
		setSpatialParentInPlace(&scrollSpace);
}
