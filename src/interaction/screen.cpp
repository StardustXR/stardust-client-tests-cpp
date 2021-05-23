#include <math.h>
#include <functional>

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
	if(inputData->input_type() == StardustXR::InputDataRaw_Hand) {
//		float pinchStrength = datamap["pinchStrength"].AsFloat();
//		const StardustXR::Hand *hand = inputData->input_as_Hand();

//		vec3 fingerPositions[24];
//		for(int i=0; i<24; ++i) {
//			fingerPositions[i] = {
//				hand->finger_joints()->Get(i)->position().x(),
//				-hand->finger_joints()->Get(i)->position().y(),
//				-hand->finger_joints()->Get(i)->position().z()
//			};
//		}
		return true;
	} else if(inputData->input_type() == StardustXR::InputDataRaw_Pointer) {
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

		float multiplier = pointerOrigin.z / pointerDir.z;
		vec3 pointerPoint = pointerOrigin - (pointerDir * multiplier);
		pointerPoint.y = -pointerPoint.y;

		domeModel.setOrigin(pointerPoint + (vec3_forward * 0.02));
		return true;
	} else {
		return false;
	}
}
