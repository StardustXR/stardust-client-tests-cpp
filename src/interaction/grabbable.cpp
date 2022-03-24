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

Grabbable::Grabbable(Spatial root, StardustXRFusion::Field &field, float maxDistance) :
		Spatial(root),
		inputHandler(nullptr, field, vec3_zero, quat_identity),
		grabSpace(Spatial::create(Root())),
		scrollSpace(Spatial::create(&grabSpace)) {

	inRangeAction.captureOnTrigger = false;
	inRangeAction.pointerActiveCondition = [&](const std::string, const PointerInput &, const Datamap &) { return true; };
	inRangeAction.handActiveCondition = [&](const std::string, const HandInput &, const Datamap &) { return true; };
	inputHandler.actions.push_back(&inRangeAction);

	grabAction.captureOnTrigger = true;
	grabAction.pointerActiveCondition = std::bind(&Grabbable::pointerGrabbingCondition, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	grabAction.handActiveCondition = std::bind(&Grabbable::handGrabbingCondition, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	inputHandler.actions.push_back(&grabAction);

	this->maxDistance = maxDistance;
	this->field = &field;

	setSpatialParent(&inputHandler);
}
Grabbable::Grabbable(SKMath::vec3 origin, SKMath::quat orientation, StardustXRFusion::Field &field, float maxDistance) :
		Grabbable(Spatial::create(nullptr, origin, orientation, vec3_one, true, true, false, true), field, maxDistance) {}

bool Grabbable::pointerGrabbingCondition(const std::string uuid, const PointerInput &pointer, const Datamap &datamap) {
	field->distance(&inputHandler, pointer.deepestPoint, [this, uuid](float distance) {
		grabbingInputDistances[uuid] = distance;
	});

	bool previouslyGrabbed = std::find(grabAction.activelyActing.begin(), grabAction.activelyActing.end(), uuid) != grabAction.activelyActing.end();
	if(!previouslyGrabbed && grabbingInputDistances[uuid] > maxDistance)
		return false;

	const float context = datamap.getFloat("context");
	return context > 0.9f && grabbingInputDistances.count(uuid) > 0 && grabbingInputDistances[uuid] < maxDistance;
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
		else
			grabbingInputDistances.erase(grabbingInputUUID);
		grabbingInputUUID = grabAction.startedActing.begin()->uuid;
		setSpatialParentInPlace(Root());
		setZoneable(false); // If anything is actively grabbing this, it shouldn't be zoneable
	} else if(std::find(grabAction.stoppedActing.begin(), grabAction.stoppedActing.end(), grabbingInputUUID) != grabAction.stoppedActing.end()) {
		grabbingInputDistances.erase(grabbingInputUUID);
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

void Grabbable::setField(Field *field) {
	this->field = field;
	inputHandler.setField(field);
}

//bool Grabbable::pointerInput(const std::string uuid, const StardustXRFusion::PointerInput &pointer, const StardustXRFusion::Datamap &datamap) {
//	field->distance(&inputHandler, pointer.deepestPoint, [&](float distance) {
//		pointDistance = distance;
//	});
//	if(pointDistance > maxDistance && !xInteract.isActive())
//		return false;
//	const float context = datamap.getFloat("context");
//	xInteract.input(context > 0.9f);

//	if(xInteract.isActive()) {
//		vec3 offset = vec3_zero;
//		vec2 scroll = datamap.getVec2("scroll");
//		if(scroll.y != 0.0f)
//			offset = pointer.direction * scroll.y * 0.01f;
//		matrix pointMat = matrix_trs(pointer.origin + offset, quat_lookat(vec3_zero, pointer.direction), vec3_one);
//		grab(pointMat);
//		startItemMat = startItemMat * matrix_trs(offset, quat_identity, vec3_one);
//	}
//	return xInteract.isActive();
//}
