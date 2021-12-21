#include "panel.hpp"
#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/fields/field.hpp>
#include <stardustxr/fusion/types/fields/spherefield.hpp>

using namespace SKMath;

PanelItemUI::PanelItemUI(StardustXRFusion::PanelItem &item, uint32_t width, uint32_t height, uint32_t density, float ppm) :
Grabbable(vec3_zero, quat_identity, StardustXRFusion::Field::Empty(), 0.05f),
panel(item),
ppm(ppm),
model(this, "../res/item/panelitem.glb", vec3_zero, quat_identity, vec3{ppm * width / density, ppm * height / density, 0.01f}),
boxField(this, vec3_zero, quat_identity, vec3{ppm * width / density, ppm * height / density, 0.01f}) {
	setField(&boxField);
	item.applySurfaceMaterial(model, 0);
	item.setSpatialParent(this);
	inputHandler.actions["close"] = [this]() {
		panel.close();
	};
	inputHandler.actions["squarify"] = [this]() {
		uint32_t side = std::max(pixelWidth, pixelHeight);
		panel.resize(side, side);
	};
	inputHandler.updateActions();
}

void PanelItemUI::update() {
	Grabbable::update();
	panel.getData([this](uint32_t width, uint32_t height, uint32_t density) {
		pixelWidth = width;
		pixelHeight = height;
		model.setScale(vec3{ppm * width / density, ppm * height / density, 0.01f});
		boxField.setSize(vec3{ppm * width / density, ppm * height / density, 0.01f});
	});
}
