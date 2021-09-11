#pragma once

#include <stardustxr/fusion/types/input/inputhandler.hpp>
#include <stardustxr/fusion/types/fields/field.hpp>

class Button : public StardustXRFusion::InputHandler {
public:
	explicit Button(Spatial *space, StardustXRFusion::Field &field, SKMath::vec3 origin, SKMath::quat orientation);

	float indexCurlThreshold = 0.9;

protected:
	bool handInput(const StardustXRFusion::HandInput &hand, const StardustXRFusion::Datamap &datamap);
	bool pointerInput(const StardustXRFusion::PointerInput &pointer, const StardustXRFusion::Datamap &datamap);
};