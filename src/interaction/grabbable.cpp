#include <math.h>
#include <functional>

#include "grabbable.hpp"

using namespace SKMath;
using namespace StardustXRFusion;

Grabbable::Grabbable(SKMath::vec3 origin, SKMath::quat orientation, std::string modelPath, float modelScale, StardustXRFusion::Field &field) :
		model(modelPath, origin, orientation, vec3_one * modelScale),
		inputHandler(nullptr, field, origin, orientation, std::bind(&Grabbable::inputEvent, this, std::placeholders::_1)) {
	this->field = &field;
//	model
}

void Grabbable::step(double delta) {
	PhysicsBody::step(delta);
	model.setOrigin(position);
	field->setOrigin(position);
	inputHandler.setOrigin(position);
}

bool Grabbable::inputEvent(const StardustXR::InputData *inputData) {
	if(inputData->input_type() == StardustXR::InputDataRaw_Hand) {
		flexbuffers::Map datamap = inputData->datamap_flexbuffer_root().AsMap();
		float pinchStrength = datamap["pinchStrength"].AsFloat();
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


		if(pinchStrength < 0.5f) {
			activeGrab = false;
			return false;
		} else if(inputData->distance() < 0.001f) {
			if(activeGrab) {
				grabPosition = pinchPos;
			}
			activeGrab = true;
		}

		if(!activeGrab) {
			addForce(vec3_normalize(pinchPos) * attraction / (inputData->distance()*inputData->distance()));
		} else {
			velocity = vec3_zero;
			position += grabPosition - pinchPos;
		}

		oldPosition = position;

//		vec3 targetPos;
//		if(activeGrab) {
//			targetPos = grabPosition;
//		} else {
//			float pinchDistance = inputData->distance();
//			vec3 pinchDirection = vec3_normalize(pinchPos);

//			float pinchAttraction = pinchDistance * pinchStrength;

//			float pinchMoveLimit = 0.01f / pinchAttraction*pinchAttraction;
//			float pinchMoveAmount = pinchDistance > pinchMoveLimit ? pinchMoveLimit : pinchDistance;

//			targetPos = pinchDirection * pinchMoveAmount;
//		}
//		model.move(targetPos);
//		field->move(targetPos);
//		inputHandler.move(targetPos);
		return true;
	} else
		return false;
}
