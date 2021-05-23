#pragma once

#include <flatbuffers/flexbuffers.h>

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/spatial.hpp>
#include <stardustxr/fusion/types/model.hpp>
#include <stardustxr/fusion/types/fields/field.hpp>
#include <stardustxr/fusion/types/input/inputhandler.hpp>

#include "../physics/physicsbody.hpp"

class Screen : public StardustXRFusion::Spatial {
public:
	explicit Screen(SKMath::vec3 origin, SKMath::quat orientation, std::string modelPath, float modelScale, StardustXRFusion::Field &field);

	float attraction = 0.1f;
protected:
	bool inputEvent(const StardustXR::InputData *inputData);

	StardustXRFusion::Model model;
	StardustXRFusion::Field *field;
	StardustXRFusion::InputHandler inputHandler;

	StardustXRFusion::Model domeModel;
};
