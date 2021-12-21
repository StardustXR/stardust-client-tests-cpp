#pragma once

#include "../interaction/grabbable.hpp"
#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/fields/boxfield.hpp>
#include <stardustxr/fusion/types/items/panel.hpp>
#include <stardustxr/fusion/types/model.hpp>

class PanelItemUI : public Grabbable {
public:
	explicit PanelItemUI(StardustXRFusion::PanelItem &item, uint32_t width, uint32_t height, uint32_t density, float ppm = 0.0005f);

	void update();
	StardustXRFusion::PanelItem panel;

protected:
	uint32_t pixelWidth;
	uint32_t pixelHeight;
	float ppm;
	StardustXRFusion::Model model;
	StardustXRFusion::BoxField boxField;
};
