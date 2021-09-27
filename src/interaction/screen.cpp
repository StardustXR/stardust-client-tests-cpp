#include "../../include/math_util.hpp"

#include <stardustxr/fusion/types/input/datamap.hpp>
#include <stardustxr/fusion/types/input/types/handinput.hpp>
#include <stardustxr/fusion/types/input/types/pointerinput.hpp>

//#include <math.h>
//#include <functional>

#include "screen.hpp"

using namespace SKMath;
using namespace StardustXRFusion;

Screen::Screen(SKMath::vec3 origin, SKMath::quat orientation, std::string modelPath, float modelScale, StardustXRFusion::Field &field) :
		Spatial(Spatial::create(nullptr, origin, orientation)),
		model(this, modelPath, vec3_zero, quat_identity, vec3_one * modelScale),
		inputHandler(nullptr, field, vec3_zero, quat_identity),
		domeModel(this, "../res/touch_dome.glb", vec3_zero, quat_identity, vec3_one) {

	inputHandler.handHandlerMethod = std::bind(&Screen::handInput, this, std::placeholders::_1, std::placeholders::_2);
	inputHandler.pointerHandlerMethod = std::bind(&Screen::pointerInput, this, std::placeholders::_1, std::placeholders::_2);

	this->field = &field;
	model.setSpatialParent(this);
	inputHandler.setSpatialParent(this);
	domeModel.setSpatialParent(this);
}

void Screen::update() {
	xInteract.update();
}

bool Screen::handInput(const StardustXRFusion::HandInput &hand, const StardustXRFusion::Datamap &datamap) {
	if(hand.distance > maxDistance)
		return false;
	xInteract.input(false);
	const SKMath::vec3 pinchPos = (hand.thumb().tip().pose.position + hand.index().tip().pose.position) * 0.5f;
	setCursor(SKMath::vec2{pinchPos.x, pinchPos.y});
	return false;
}
bool Screen::pointerInput(const StardustXRFusion::PointerInput &pointer, const StardustXRFusion::Datamap &datamap) {
	if(pointer.distance > maxDistance)
		return false;
	xInteract.input(false);
	const SKMath::vec3 deepestPoint = pointer.origin + (pointer.direction * datamap.getFloat("deepestPointDistance"));

	if(abs(deepestPoint.z) > abs(dimensions.z*2)) {
		domeModel.setScale(vec3_zero);
		return false;
	}

	domeModel.setScale(vec3_one * (1.0f - clamp(map(pointer.distance, 0, maxDistance, 0, 1), 0, 1)));

	if(pointer.origin.z > 0 && pointer.distance < maxDistance)
		setCursor({deepestPoint.x, deepestPoint.y});
	return false;
}

void Screen::setCursor(SKMath::vec2 pos) {
	vec2 cursor;
	cursor.x = clamp(pos.x, -dimensions.x/2, dimensions.x/2);
	cursor.y = clamp(pos.y, -dimensions.y/2, dimensions.y/2);
	domeModel.setOrigin({cursor.x, cursor.y, 0});
}
