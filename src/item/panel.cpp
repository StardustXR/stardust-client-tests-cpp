#include "panel.hpp"
#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/fields/field.hpp>
#include <stardustxr/fusion/types/fields/spherefield.hpp>

using namespace SKMath;
using namespace StardustXRFusion;

PanelItemUI::PanelItemUI(PanelItem &item, uint32_t pixelWidth, uint32_t pixelHeight, float width, float thickness) :
Grabbable(item, BoxField(&item, vec3_zero, quat_identity, vec3{width, width * pixelHeight / pixelWidth, thickness}), 0.01f),
panel(item),
width(width),
thickness(thickness),
model(this, "../res/item/panelitem.glb", vec3_zero, quat_identity, vec3{width, width * pixelHeight / pixelWidth, thickness}) {
	boxField = static_cast<BoxField *>(&field);
	item.applySurfaceMaterial(model, 0);
	onStoppedGrabbing = [this]() {
		panel.triggerAccept();
	};
}

void PanelItemUI::update() {
	Grabbable::update();
	panel.getData([this](StardustXRFusion::PanelItem::Data data) {
		vec3 size = {width, width * data.height / data.width, thickness};
		model.setScale(size);
		boxField->setSize(size);
	});
}

void PanelItemUI::setCaptured(bool captured) {
	this->captured = captured;
	inputHandler.setEnabled(!captured);
	model.setEnabled(!captured);
}
