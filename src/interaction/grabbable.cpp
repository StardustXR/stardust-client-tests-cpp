#include "../../include/math_util.hpp"

#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/input/datamap.hpp>
#include <stardustxr/fusion/types/input/types/handinput.hpp>
#include <stardustxr/fusion/types/input/types/pointerinput.hpp>
#include <stardustxr/fusion/types/spatial.hpp>

#include "grabbable.hpp"
#include "xinteract.hpp"

using namespace SKMath;
using namespace StardustXRFusion;

Grabbable::Grabbable(SKMath::vec3 origin, SKMath::quat orientation, StardustXRFusion::Field &field, float maxDistance) :
		Spatial(Spatial::create(nullptr, origin, orientation, vec3_one, true, true, false, true)),
//		spaceReference(nullptr),
		inputHandler(nullptr, field, vec3_zero, quat_identity) {

	inputHandler.handHandlerMethod = std::bind(&Grabbable::handInput, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	inputHandler.pointerHandlerMethod = std::bind(&Grabbable::pointerInput, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

	this->maxDistance = maxDistance;
	this->field = &field;

	setSpatialParent(&inputHandler);
}

void Grabbable::update() {
	if(xInteract.hasActiveChanged())
		setZoneable(!xInteract.isActive());
	xInteract.update();
}

void Grabbable::setField(Field *field) {
	this->field = field;
	inputHandler.setField(field);
}

bool Grabbable::isActive() {
	return xInteract.isActive();
}

bool Grabbable::activeChanged() {
	return xInteract.hasActiveChanged();
}

void Grabbable::grab(matrix grabMat) {
	if(xInteract.hasActiveChanged()) {
		setZoneable(!xInteract.isActive());
		setSpatialParentInPlace(&inputHandler);
		matrix_inverse(grabMat, startGrabMat);
		getTransform(&inputHandler, [this](vec3 pos, quat rot, vec3 scl) {
			startItemMat = matrix_trs(pos, rot, scl);
//			inputHandler.setPose({pos, rot});
		});
	} else {
		matrix itemMat = startItemMat * startGrabMat * grabMat;

		vec3 pos, scl;
		quat rot;
		matrix_decompose(itemMat, pos, scl, rot);
		setPose(pose_t{pos, rot});
	}
}

bool Grabbable::handInput(const std::string uuid, const StardustXRFusion::HandInput &hand, const StardustXRFusion::Datamap &datamap) {
	const SKMath::vec3 pinchPos = (hand.thumb().tip().pose.position + hand.index().tip().pose.position) * 0.5f;
	field->distance(&inputHandler, pinchPos, [&](float distance) { pinchDistance = distance; });
	if(pinchDistance > maxDistance && !xInteract.isActive())
		return false;
	const float pinchStrength = datamap.getFloat("pinchStrength");
	xInteract.input(pinchStrength > 0.9f);

	if(xInteract.isActive()) {
		matrix pinchMat = matrix_trs(pinchPos, hand.palm.pose.orientation, vec3_one);
		grab(pinchMat);
	}
	return xInteract.isActive();
}

bool Grabbable::pointerInput(const std::string uuid, const StardustXRFusion::PointerInput &pointer, const StardustXRFusion::Datamap &datamap) {
	const SKMath::vec3 deepestPoint = pointer.origin + (pointer.direction * datamap.getFloat("deepestPointDistance"));
	field->distance(&inputHandler, deepestPoint, [&](float distance) {
		pointDistance = distance;
	});
	if(pointDistance > maxDistance && !xInteract.isActive())
		return false;
	const float context = datamap.getFloat("context");
	xInteract.input(context > 0.9f);

	if(xInteract.isActive()) {
		matrix pointMat = matrix_trs(pointer.origin, quat_lookat(vec3_zero, pointer.direction), vec3_one);
		grab(pointMat);
		vec2 scroll = datamap.getVec2("scroll");
		if(scroll.y != 0.0f) {
			setOrigin(getOrigin() + (pointer.direction * scroll.y * 0.01f));
			startItemMat = matrix_trs(getOrigin(), getOrientation(), vec3_one);
			matrix_inverse(pointMat, startGrabMat);
		}
	}
	return xInteract.isActive();
}
