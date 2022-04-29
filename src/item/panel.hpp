#pragma once

#include <stardustxr/fusion/values/glm.hpp>
#include <stardustxr/fusion/types/fields/boxfield.hpp>
#include <stardustxr/fusion/types/items/types/panel.hpp>
#include <stardustxr/fusion/types/drawable/model.hpp>
#include "../interaction/grabbable.hpp"

class PanelItemUI : public Grabbable {
public:
	explicit PanelItemUI(StardustXRFusion::PanelItem &item, uint32_t pixelWidth, uint32_t pixelHeight, float width = 0.05f, float thickness = 0.005f);

	void update();
	StardustXRFusion::PanelItem panel;

	void setCaptured(bool captured);

protected:
	bool captured = false;
	uint32_t pixelWidth;
	uint32_t pixelHeight;
	float width;
	float thickness;
	StardustXRFusion::Model model;
	StardustXRFusion::BoxField *boxField;
};
