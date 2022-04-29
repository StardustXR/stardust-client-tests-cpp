#pragma once

#include <stardustxr/fusion/values/glm.hpp>
#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/spatial/spatial.hpp>
#include <stardustxr/fusion/types/drawable/model.hpp>
#include <stardustxr/fusion/types/fields/field.hpp>
#include <stardustxr/fusion/types/input/inputhandler.hpp>

#include "xinteract.hpp"

class Screen : public StardustXRFusion::Spatial {
public:
	explicit Screen(Spatial &root, StardustXRFusion::Vec3 origin, StardustXRFusion::Quat orientation, std::string modelPath, float modelScale, StardustXRFusion::Field &field);

	void update();

	float maxDistance = 0.0254f;
protected:
	XInteract xInteract;
	bool handInput(const std::string uuid, const StardustXRFusion::HandInput &hand, const StardustXRFusion::Datamap &datamap);
	bool pointerInput(const std::string uuid, const StardustXRFusion::PointerInput &pointer, const StardustXRFusion::Datamap &datamap);

	StardustXRFusion::Model model;
	StardustXRFusion::Field field;
	StardustXRFusion::InputHandler inputHandler;

	StardustXRFusion::Vec3 dimensions = {0.7f, 0.404f, 0.004f};

	void setCursor(StardustXRFusion::Vec2 pos);

	StardustXRFusion::Model domeModel;
};
