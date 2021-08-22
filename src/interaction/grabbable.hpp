#pragma once

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/spatial.hpp>
#include <stardustxr/fusion/types/fields/field.hpp>
#include <stardustxr/fusion/types/input/inputhandler.hpp>

#include "xinteract.hpp"

class Grabbable : public StardustXRFusion::Spatial {
public:
	explicit Grabbable(SKMath::vec3 origin, SKMath::quat orientation, StardustXRFusion::Field &field);

	void update();

	float maxDistance = 0.01f;
protected:
	bool handInput(const StardustXRFusion::HandInput &hand, const StardustXRFusion::Datamap &datamap);
	bool pointerInput(const StardustXRFusion::PointerInput &pointer, const StardustXRFusion::Datamap &datamap);

	XInteract xPinch;
	SKMath::pose_t pinchOffset;
	float pinchDistance = 0.0f;

	XInteract xPoint;
	SKMath::matrix offsetMat;
	float pointDistance = 0.0f;

	StardustXRFusion::Field *field;
	StardustXRFusion::InputHandler inputHandler;
};
