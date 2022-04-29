#pragma once

#include <stardustxr/fusion/values/glm.hpp>
#include <stardustxr/fusion/types/fields/spherefield.hpp>
#include <stardustxr/fusion/types/items/types/environment.hpp>
#include <stardustxr/fusion/types/drawable/model.hpp>
#include "../interaction/grabbable.hpp"

class EnvironmentItemUI : public Grabbable {
public:
	explicit EnvironmentItemUI(StardustXRFusion::EnvironmentItem &item, StardustXRFusion::EnvironmentItem::Data data, float size = 0.07f);

protected:
	StardustXRFusion::EnvironmentItem environmentItem;
	StardustXRFusion::Model model;
	StardustXRFusion::SphereField *sphereField;
};
