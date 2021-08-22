#pragma once

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/spatial.hpp>
#include <stardustxr/fusion/types/model.hpp>
#include <stardustxr/fusion/types/fields/field.hpp>
#include <stardustxr/fusion/types/input/inputhandler.hpp>

#include "xinteract.hpp"

class Screen : public StardustXRFusion::Spatial {
public:
	explicit Screen(SKMath::vec3 origin, SKMath::quat orientation, std::string modelPath, float modelScale, StardustXRFusion::Field &field);

	void update();

	float maxDistance = 0.0254f;
protected:
	XInteract xInteract;
	bool handInput(const StardustXRFusion::HandInput &hand, const StardustXRFusion::Datamap &datamap);
	bool pointerInput(const StardustXRFusion::PointerInput &pointer, const StardustXRFusion::Datamap &datamap);

	StardustXRFusion::Model model;
	StardustXRFusion::Field *field;
	StardustXRFusion::InputHandler inputHandler;

	SKMath::vec3 dimensions = {0.7f, 0.404f, 0.004f};

	void setCursor(SKMath::vec2 pos);

	StardustXRFusion::Model domeModel;
};
