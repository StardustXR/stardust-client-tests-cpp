#pragma once

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/spatial.hpp>
#include <stardustxr/fusion/types/fields/field.hpp>
#include <stardustxr/fusion/types/input/inputhandler.hpp>

#include "xinteract.hpp"

class Grabbable : public StardustXRFusion::Spatial {
public:
	explicit Grabbable(SKMath::vec3 origin, SKMath::quat orientation, StardustXRFusion::Field &field, float maxDistance = 0.01f);

	void update();
	void setField(StardustXRFusion::Field *field);

	void resetStart();

	bool isActive();
	bool activeChanged();

	float maxDistance;

	StardustXRFusion::Spatial item;
protected:
	bool handInput(const StardustXRFusion::HandInput &hand, const StardustXRFusion::Datamap &datamap);
	bool pointerInput(const StardustXRFusion::PointerInput &pointer, const StardustXRFusion::Datamap &datamap);
	void grab(SKMath::matrix grabMat);

	XInteract xInteract;
	SKMath::matrix startItemMat;
	SKMath::matrix startGrabMat;
	float pinchDistance = 0.0f;
	float pointDistance = 0.0f;

	StardustXRFusion::Field *field;
	StardustXRFusion::InputHandler inputHandler;
};
