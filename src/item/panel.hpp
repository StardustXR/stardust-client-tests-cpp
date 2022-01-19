#pragma once

#include "../interaction/grabbable.hpp"
#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/fields/boxfield.hpp>
#include <stardustxr/fusion/types/items/types/panel.hpp>
#include <stardustxr/fusion/types/model.hpp>

class PanelItemUI : public Grabbable {
public:
	explicit PanelItemUI(StardustXRFusion::PanelItem &item, uint32_t pixelWidth, uint32_t pixelHeight, float width = 0.05f, float thickness = 0.005f);

	void update();
	StardustXRFusion::PanelItem panel;

protected:
	uint32_t pixelWidth;
	uint32_t pixelHeight;
	float width;
	float thickness;
	StardustXRFusion::Model model;
	StardustXRFusion::BoxField boxField;
};
