#include "../../include/math_util.hpp"

#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/input/datamap.hpp>
#include <stardustxr/fusion/types/input/types/handinput.hpp>
#include <stardustxr/fusion/types/input/types/pointerinput.hpp>

#include "grabbable.hpp"

using namespace SKMath;
using namespace StardustXRFusion;

Grabbable::Grabbable(SKMath::vec3 origin, SKMath::quat orientation, StardustXRFusion::Field &field) :
		Spatial(Spatial::create(origin, orientation)),
		inputHandler(this, field, vec3_zero, quat_identity) {

	inputHandler.handHandlerMethod = std::bind(&Grabbable::handInput, this, std::placeholders::_1, std::placeholders::_2);
	// inputHandler.pointerHandlerMethod = std::bind(&Grabbable::pointerInput, this, std::placeholders::_1, std::placeholders::_2);

	this->field = &field;
	inputHandler.setSpatialParent(this);
}

void Grabbable::update() {
	xPinch.update();
	xPoint.update();
}

bool Grabbable::handInput(const StardustXRFusion::HandInput &hand, const StardustXRFusion::Datamap &datamap) {
	const SKMath::vec3 pinchPos = (hand.thumb().tip().pose.position + hand.index().tip().pose.position) * 0.5f;
	field->distance(&inputHandler, pinchPos, [&](float distance) { pinchDistance = distance; });
	if(pinchDistance > maxDistance && !xPinch.isActive())
		return false;
	const float pinchStrength = datamap.getFloat("pinchStrength");
	xPinch.input(pinchStrength > 0.9f);

	if(xPinch.isActive()) {
		pose_t pinchPose = {
			pinchPos,
			hand.palm.pose.orientation
		};
		if(xPinch.hasActiveChanged())
			pinchOffset = { -pinchPose.position, quat_inverse(pinchPose.orientation) };
		else {
			this->move(pinchPose.position);
			this->rotate(pinchPose.orientation * pinchOffset.orientation);
			this->move(pinchOffset.position);
		}
	}
	return xPinch.isActive();
}
// bool Grabbable::pointerInput(const StardustXRFusion::PointerInput &pointer, const StardustXRFusion::Datamap &datamap) {
// 	const SKMath::vec3 deepestPoint = pointer.origin + (pointer.direction * datamap.getFloat("deepestPointDistance"));
// 	field->distance(&inputHandler, deepestPoint, [&](float distance) {
// 		pointDistance = distance;
// 	});
// 	if(pointDistance > maxDistance && !xPoint.isActive())
// 		return false;
// 	const float select = datamap.getFloat("select");
// 	xPoint.input(select > 0.9f);

// 	matrix currentMat = matrix_trs(pointer.origin, quat_lookat(vec3_forward, -pointer.direction), vec3_one);
// 	// matrix diffMat = offsetMat * currentMat;
// 	matrix diffMat = currentMat;
// 	vec3 moveOffset;
// 	quat rotOffset;
// 	vec3 scaleOffset;
// 	matrix_decompose(diffMat, moveOffset, scaleOffset, rotOffset);
// 	if(xPoint.isActive() && !xPoint.hasActiveChanged()) {
// 		// this->rotateAround(pointer.origin, quat_lookat(vec3_forward, pointer.direction) * quat_inverse(startPointPose.orientation));
// 		// this->rotate(rotOffset);
// 		this->move(moveOffset);
// 	}
// 	matrix_inverse(currentMat, offsetMat);
// 	return xPinch.isActive();

// 	return false;
// }