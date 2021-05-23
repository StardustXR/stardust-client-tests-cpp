#pragma once

#include <flatbuffers/flexbuffers.h>

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/model.hpp>
#include <stardustxr/fusion/types/fields/boxfield.hpp>
#include <stardustxr/fusion/types/input/inputhandler.hpp>

#include "../physics/physicsbody.hpp"

class Grabbable : public PhysicsBody {
public:
	explicit Grabbable(SKMath::vec3 origin, SKMath::quat orientation, std::string modelPath, float modelScale, StardustXRFusion::Field &field);

	float attraction = 0.1f;

	void step(double delta);
protected:
	bool inputEvent(const StardustXR::InputData *inputData);

	bool activeGrab = false;
	SKMath::vec3 grabPosition = SKMath::vec3_zero;
	SKMath::vec3 oldPosition = SKMath::vec3_zero;

	StardustXRFusion::Model model;
	StardustXRFusion::Field *field;
	StardustXRFusion::InputHandler inputHandler;
};
