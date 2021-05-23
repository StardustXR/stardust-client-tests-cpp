#include <math.h>
#include <functional>

#include "contactable.hpp"

using namespace SKMath;
using namespace StardustXRFusion;

Contactable::Contactable(SKMath::vec3 origin, SKMath::quat orientation, std::string modelPath, float modelScale, StardustXRFusion::Field &field) :
		model(modelPath, origin, orientation, vec3_one * modelScale),
		inputHandler(nullptr, field, origin, orientation, std::bind(&Contactable::inputEvent, this, std::placeholders::_1)) {
	this->field = &field;
}

void Contactable::step(double delta) {
	PhysicsBody::step(delta);
	model.setOrigin(position);
	field->setOrigin(position);
	inputHandler.setOrigin(position);
}

bool Contactable::inputEvent(const StardustXR::InputData *inputData) {
	if(inputData->input_type() == StardustXR::InputDataRaw_Hand) {
		flexbuffers::Map datamap = inputData->datamap_flexbuffer_root().AsMap();
		float pinchStrength = datamap["pinchStrength"].AsFloat();
		const StardustXR::Hand *hand = inputData->input_as_Hand();

		vec3 fingerPositions[24];
		for(int i=0; i<24; ++i) {
			fingerPositions[i] = {
				hand->finger_joints()->Get(i)->position().x(),
				-hand->finger_joints()->Get(i)->position().y(),
				-hand->finger_joints()->Get(i)->position().z()
			};
		}
		return true;
	} else
		return false;
}
