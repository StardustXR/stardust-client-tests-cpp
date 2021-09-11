#pragma once

#include "../interaction/grabbable.hpp"
#include <stardustxr/fusion/sk_math.hpp>

class Item : protected StardustXRFusion::Spatial {
public:
	explicit Item(SKMath::vec3 origin, SKMath::quat orientation);

	void update();

	void onAccepted();

	bool isGrabbing();
	bool grabbingChanged();
	StardustXRFusion::Field field = StardustXRFusion::Field(SKMath::vec3_zero, SKMath::quat_identity);

protected:
	Grabbable grabbable;
};