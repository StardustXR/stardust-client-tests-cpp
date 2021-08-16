#include "slider.hpp"
#include "../../include/math_util.hpp"
#include <algorithm>
#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/input/datamap.hpp>
#include <stardustxr/fusion/types/input/types/handinput.hpp>
#include <stardustxr/fusion/types/input/types/pointerinput.hpp>

using namespace StardustXRFusion;
using namespace SKMath;

Slider::Slider(float length, float minValue, float maxValue, SKMath::color color) :
	Spatial(Spatial::create()),
	base("../res/slider/base.glb", vec3_zero, quat_from_angles(0, 180, 0), {0, 1, 1}),
	base_inv("../res/slider/base.glb", {length, 0, 0}, quat_identity, {length, 1, 1}),
	orb("../res/slider/orb.glb"),
	field(vec3_right * (length / 2), quat_identity, {length, 0.004, 0.004}),
	inputHandler(nullptr, field, vec3_zero, quat_identity) {
	
	inputHandler.handHandlerMethod = std::bind(&Slider::handInput, this, std::placeholders::_1, std::placeholders::_2);
	inputHandler.pointerHandlerMethod = std::bind(&Slider::pointerInput, this, std::placeholders::_1, std::placeholders::_2);

	base.setMaterialProperty(0, "color", color);

	this->minValue = minValue;
	this->maxValue = maxValue;
	this->length = length;

	base.setSpatialParent(this);
	base_inv.setSpatialParent(this);
	orb.setSpatialParent(this);
	field.setSpatialParent(this);
	inputHandler.setSpatialParent(this);
}

Slider::~Slider() {}

void Slider::setSliderValue(float value) {
	float sliderPos = map(value, minValue, maxValue, 0, length);
	setSliderPos(sliderPos);
}

void Slider::setSliderPos(float pos) {
	orbPos = clamp(pos, 0, length);
	orb.setOrigin({orbPos, 0, 0});
	base.setScale({orbPos, 1, 1});
	base_inv.setScale({length - orbPos, 1, 1});

	value = map(orbPos, 0, length, minValue, maxValue);
}

void Slider::setSliderLength(float length) {
	this->length = length;
	
	field.setOrigin(vec3_right * (length / 2));
	field.setSize({length, 0.004, 0.004});

	base_inv.setOrigin({length, 0, 0});
	setSliderValue(value);
}


bool Slider::handInput(const StardustXRFusion::HandInput &hand, const StardustXRFusion::Datamap &datamap) {
	if(hand.distance > maxDistance)
		return false;
	const SKMath::vec3 pinchPos = (hand.thumb().tip().pose.position + hand.index().tip().pose.position) * 0.5f;
	const float pinchStrength = datamap.getFloat("pinchStrength");
	const bool move = pinchStrength > 0.9f;
	if(move)
		setSliderPos(pinchPos.x);
	return move;
}
bool Slider::pointerInput(const StardustXRFusion::PointerInput &pointer, const StardustXRFusion::Datamap &datamap) {
	if(pointer.distance > maxDistance)
		return false;
	const float select = datamap.getFloat("select");
	const SKMath::vec2 scroll = datamap.getVec2("scroll");
	const SKMath::vec3 deepestPoint = pointer.origin + (pointer.direction * datamap.getFloat("deepestPointDistance"));
	
	if(select > 0.9f) {
		setSliderPos(deepestPoint.x);
		return true;
	} else if(scroll.y != 0.0f) {
		setSliderPos(orbPos+scroll.y);
		return true;
	}
	return false;
}

// bool Slider::inputEvent(const StardustXR::InputData *inputData) {
// 		case StardustXR::InputDataRaw_Pointer: {
// 			float select = datamap["select"].AsFloat();
// 			if(select < 0.9f) {
// 				movedBefore = false;
// 				return false;
// 			} else if(!movedBefore) {
// 				movedBefore = true;
// 			}
// 			flexbuffers::Vector scroll = datamap["scroll"].AsVector();
// 			float scrollY = scroll[1].AsFloat();
// 			const StardustXR::Pointer *pointer = inputData->input_as_Pointer();
// 			vec3 pointerDir = {
// 				pointer->direction()->x(),
// 				pointer->direction()->y(),
// 				pointer->direction()->z()
// 			};
// 			vec3 pointerOrigin = {
// 				pointer->origin()->x(),
// 				pointer->origin()->y(),
// 				pointer->origin()->z()
// 			};
// 			float deepestPointDistance = datamap["deepestPointDistance"].AsFloat();
// 			vec3 deepestPoint = pointerOrigin + (vec3_normalize(pointerDir) * deepestPointDistance);


// 			return true;
// 		} break;
// 		default: {
// 			movedBefore = false;
// 		} return false;
// 	}

// 	return false;
// }
