#pragma once

#include "item.hpp"
#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/fields/spherefield.hpp>
#include <stardustxr/fusion/types/model.hpp>

class EnvironmentItem : public Item {
public:
	explicit EnvironmentItem(std::string environment, SKMath::vec3 origin, SKMath::quat orientation = SKMath::quat_identity, float size = 0.07);
	virtual ~EnvironmentItem() {}

	void onAccepted(Spatial &acceptor);

protected:
	std::string environment;
	StardustXRFusion::Model model;
	StardustXRFusion::SphereField sphereField;
};