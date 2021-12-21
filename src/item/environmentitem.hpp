#pragma once

#include "../interaction/grabbable.hpp"
#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/fields/spherefield.hpp>
#include <stardustxr/fusion/types/items/environment.hpp>
#include <stardustxr/fusion/types/model.hpp>

class EnvironmentItemUI : public Grabbable {
public:
	explicit EnvironmentItemUI(StardustXRFusion::EnvironmentItem &item, float size = 0.07f);

protected:
	StardustXRFusion::EnvironmentItem environmentItem;
	StardustXRFusion::Model model;
	StardustXRFusion::SphereField sphereField;
};
