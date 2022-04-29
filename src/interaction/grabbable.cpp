#include <stardustxr/fusion/values/glm.hpp>
#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/input/datamap.hpp>
#include <stardustxr/fusion/types/input/types/handinput.hpp>
#include <stardustxr/fusion/types/input/types/pointerinput.hpp>

#include "grabbable.hpp"
#include "xinteract.hpp"

using namespace StardustXRFusion;

Grabbable::Grabbable(Spatial root, const Field field, float maxDistance) :
Spatial(root),
maxDistance(maxDistance),
field(field),
inputHandler(Root(), this->field, Vec3::Zero, Quat::Identity),
grabAction(true, true, &inRangeAction),
grabSpace(&inputHandler),
scrollSpace(&grabSpace) {

	inRangeAction.captureOnTrigger = false;
	inRangeAction.pointerActiveCondition = [&](const std::string, const PointerInput &pointer, const Datamap &) { return pointer.distance < maxDistance; };
	inRangeAction.handActiveCondition = [&](const std::string, const HandInput &hand, const Datamap &) { return hand.distance < maxDistance; };
	inputHandler.actions.push_back(&inRangeAction);

	grabAction.pointerActiveCondition = std::bind(&Grabbable::pointerGrabbingCondition, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	grabAction.handActiveCondition = std::bind(&Grabbable::handGrabbingCondition, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	inputHandler.actions.push_back(&grabAction);

	setSpatialParent(&inputHandler);
}
Grabbable::Grabbable(Vec3 origin, Quat orientation, const StardustXRFusion::Field field, float maxDistance) :
		Grabbable(Spatial(Root(), origin, orientation, Vec3::One, true, true, false, true), field, maxDistance) {}

bool Grabbable::pointerGrabbingCondition(const std::string uuid, const PointerInput &pointer, const Datamap &datamap) {
	const float select = datamap.getFloat("select");
	return select > 0.9f;
}
bool Grabbable::handGrabbingCondition(const std::string uuid, const HandInput &hand, const Datamap &datamap) {
	const float pinchStrength = datamap.getFloat("pinchStrength");
	const float grabStrength = datamap.getFloat("grabStrength");
	return pinchStrength > 0.9f && grabStrength < 0.1f;
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
			grabSpace.setPose(Pose(
				(grabAction.actor->hand->thumb().tip().pose.position +
				 grabAction.actor->hand->index().tip().pose.position) * 0.5f,
				 grabAction.actor->hand->palm.pose.rotation
			));
		} else if(grabAction.actor->pointer.get()) {
			grabSpace.setPose(Pose(
				grabAction.actor->pointer->origin,
				glm::quatLookAt(glm::vec3(grabAction.actor->pointer->direction), glm::vec3(Vec3::Up))
			));
			if(grabAction.actor->datamap.exists("scroll")) {
				Vec2 scroll = grabAction.actor->datamap.getVec2("scroll");
				scrollDistance += scroll.y * 0.05f;
				scrollSpace.setOrigin(Vec3::Forward * scrollDistance);
			}
		}
	}
	if(grabAction.actorStarted || grabAction.actorChanged)
		setSpatialParentInPlace(&scrollSpace);
}
