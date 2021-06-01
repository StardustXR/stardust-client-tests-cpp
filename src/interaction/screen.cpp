#include "../../include/math_util.hpp"

//#include <math.h>
//#include <functional>

#include "screen.hpp"

using namespace SKMath;
using namespace StardustXRFusion;

Screen::Screen(SKMath::vec3 origin, SKMath::quat orientation, std::string modelPath, float modelScale, StardustXRFusion::Field &field) :
		Spatial(Spatial::create(origin, orientation)),
		model(modelPath, vec3_zero, quat_identity, vec3_one * modelScale),
		inputHandler(nullptr, field, vec3_zero, quat_identity, std::bind(&Screen::inputEvent, this, std::placeholders::_1)),
		domeModel("../res/touch_dome.glb", vec3_zero, quat_identity, vec3_one) {
	this->field = &field;
	model.setSpatialParent(this);
	inputHandler.setSpatialParent(this);
	domeModel.setSpatialParent(this);
}

bool Screen::inputEvent(const StardustXR::InputData *inputData) {
	flexbuffers::Map datamap = inputData->datamap_flexbuffer_root().AsMap();
	float distance = inputData->distance();
	switch(inputData->input_type()) {
		case StardustXR::InputDataRaw_Hand: {
			const StardustXR::Hand *hand = inputData->input_as_Hand();

			const vec3 indexTipPos = {
				hand->finger_joints()->Get(8)->position().x(),
				-hand->finger_joints()->Get(8)->position().y(),
				-hand->finger_joints()->Get(8)->position().z()
			};
			const vec3 thumbTipPos = {
				hand->finger_joints()->Get(2)->position().x(),
				-hand->finger_joints()->Get(2)->position().y(),
				-hand->finger_joints()->Get(2)->position().z()
			};
			const vec3 pinchPos = (indexTipPos + thumbTipPos) * 0.5f;

			setCursor({pinchPos.x, pinchPos.y});
		} return true;
		case StardustXR::InputDataRaw_Pointer: {
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

			if(abs(deepestPoint.z) > abs(dimensions.z*2)) {
				domeModel.setScale(vec3_zero);
				return false;
			}

			domeModel.setScale(vec3_one * (1.0f - clamp(map(distance, 0, maxDistance, 0, 1), 0, 1)));

			if(pointerOrigin.z > 0 && inputData->distance() < maxDistance)
				setCursor({deepestPoint.x, -deepestPoint.y});
			else
				return false;
		} return true;
		default: return false;
	}
}

void Screen::setCursor(SKMath::vec2 pos) {
	vec2 cursor;
	cursor.x = clamp(pos.x, -dimensions.x/2, dimensions.x/2);
	cursor.y = clamp(pos.y, -dimensions.y/2, dimensions.y/2);
	domeModel.setOrigin({cursor.x, cursor.y, 0});
}
