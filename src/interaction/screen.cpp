#include "include/math_util.hpp"

#include <stardustxr/fusion/values/glm.hpp>
#include <stardustxr/fusion/types/input/datamap.hpp>
#include <stardustxr/fusion/types/input/types/handinput.hpp>
#include <stardustxr/fusion/types/input/types/pointerinput.hpp>

//#include <math.h>
//#include <functional>

#include "screen.hpp"

using namespace StardustXRFusion;

Screen::Screen(Spatial &root, Vec3 origin, Quat orientation, std::string modelPath, float modelScale, StardustXRFusion::Field &field) :
		Spatial(&root, origin, orientation),
		model(this, modelPath, Vec3::Zero, Quat::Identity, Vec3::One * modelScale),
		field(field),
		inputHandler(nullptr, field, Vec3::Zero, Quat::Identity),
		domeModel(this, "res/touch_dome.glb", Vec3::Zero, Quat::Identity, Vec3::One) {

	inputHandler.handHandlerMethod = std::bind(&Screen::handInput, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	inputHandler.pointerHandlerMethod = std::bind(&Screen::pointerInput, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

void Screen::update() {
	xInteract.update();
}

bool Screen::handInput(const std::string uuid, const StardustXRFusion::HandInput &hand, const StardustXRFusion::Datamap &datamap) {
	if(hand.distance > maxDistance)
		return false;
	xInteract.input(false);
	const Vec3 pinchPos = (hand.thumb().tip().pose.position + hand.index().tip().pose.position) * 0.5f;
	setCursor(Vec2{pinchPos.x, pinchPos.y});
	return false;
}
bool Screen::pointerInput(const std::string uuid, const StardustXRFusion::PointerInput &pointer, const StardustXRFusion::Datamap &datamap) {
	if(pointer.distance > maxDistance)
		return false;
	xInteract.input(false);

	if(abs(pointer.deepestPoint.z) > abs(dimensions.z*2)) {
		domeModel.setScale(Vec3::Zero);
		return false;
	}

	domeModel.setScale(Vec3::One * (1.0f - clamp(map(pointer.distance, 0, maxDistance, 0, 1), 0, 1)));

	if(pointer.origin.z > 0 && pointer.distance < maxDistance)
		setCursor({pointer.deepestPoint.x, pointer.deepestPoint.y});
	return false;
}

void Screen::setCursor(Vec2 pos) {
	Vec2 cursor;
	cursor.x = clamp(pos.x, -dimensions.x/2, dimensions.x/2);
	cursor.y = clamp(pos.y, -dimensions.y/2, dimensions.y/2);
	domeModel.setOrigin({cursor.x, cursor.y, 0});
}
