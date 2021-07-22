#include "slider.hpp"
#include "../../include/math_util.hpp"
#include <algorithm>

using namespace StardustXRFusion;
using namespace SKMath;

Slider::Slider(float length, float minValue, float maxValue) :
	Spatial(Spatial::create()),
	base("../res/slider/base.glb", vec3_zero, quat_identity, {length, 1, 1}),
	base_inv("../res/slider/base_inv.glb", {length, 0, 0}, quat_identity, {0, 1, 1}),
	orb("../res/slider/orb.glb"),
	field(vec3_right * (length / 2), quat_identity, {length, 0.004, 0.004}),
	handler(nullptr, field, vec3_zero, quat_identity, std::bind(&Slider::inputEvent, this, std::placeholders::_1)) {

	this->minValue = minValue;
	this->maxValue = maxValue;
	this->length = length;

	base.setSpatialParent(this);
	base_inv.setSpatialParent(this);
	orb.setSpatialParent(this);
	field.setSpatialParent(this);
	handler.setSpatialParent(this);
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

bool Slider::inputEvent(const StardustXR::InputData *inputData) {
	flexbuffers::Map datamap = inputData->datamap_flexbuffer_root().AsMap();
	float distance = inputData->distance();
	if(distance > maxDistance)
		return false;
	switch(inputData->input_type()) {
		case StardustXR::InputDataRaw_Hand: {
			const StardustXR::Hand *hand = inputData->input_as_Hand();
			const float pinchPos = (hand->finger_joints()->Get(2)->position().x() + hand->finger_joints()->Get(8)->position().x()) * 0.5f;
			float pinchStrength = datamap["pinchStrength"].AsFloat();
			bool move = pinchStrength > 0.9f;

			if(move)
				setSliderPos(pinchPos);

			return true;
		} break;
		case StardustXR::InputDataRaw_Pointer: {
			float select = datamap["select"].AsFloat();
			if(select < 0.9f) {
				movedBefore = false;
				return false;
			} else if(!movedBefore) {
				movedBefore = true;
			}
			flexbuffers::Vector scroll = datamap["scroll"].AsVector();
			float scrollY = scroll[1].AsFloat();
			const StardustXR::Pointer *pointer = inputData->input_as_Pointer();
			vec3 pointerDir = {
				pointer->direction()->x(),
				pointer->direction()->y(),
				pointer->direction()->z()
			};
			vec3 pointerOrigin = {
				pointer->origin()->x(),
				pointer->origin()->y(),
				pointer->origin()->z()
			};
			float deepestPointDistance = datamap["deepestPointDistance"].AsFloat();
			vec3 deepestPoint = pointerOrigin + (vec3_normalize(pointerDir) * deepestPointDistance);

			if(select > 0.9f)
				setSliderPos(deepestPoint.x);
			else
				setSliderPos(orbPos+scrollY);

			return true;
		} break;
		default: {
			movedBefore = false;
		} return false;
	}

	return false;
}
