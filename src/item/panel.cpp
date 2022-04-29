#include <stardustxr/fusion/values/glm.hpp>
#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/fields/field.hpp>
#include <stardustxr/fusion/types/fields/spherefield.hpp>

#include "panel.hpp"

using namespace StardustXRFusion;

PanelItemUI::PanelItemUI(PanelItem &item, uint32_t pixelWidth, uint32_t pixelHeight, float width, float thickness) :
Grabbable(item, BoxField(&item, Vec3::Zero, Quat::Identity, Vec3(width, width * pixelHeight / pixelWidth, thickness)), 0.01f),
panel(item),
width(width),
thickness(thickness),
model(this, "res/item/panelitem.glb", Vec3::Zero, Quat::Identity, Vec3::Zero) {
	boxField = static_cast<BoxField *>(&field);
	item.applySurfaceMaterial(model, 0);
	onStoppedGrabbing = [this]() {
		panel.triggerAccept();
	};
}

void PanelItemUI::update() {
	Grabbable::update();
	panel.getData([this](StardustXRFusion::PanelItem::Data data) {
		if(data.width == 0 || data.height == 0) return;
		Vec3 size = {width, width * data.height / data.width, thickness};
		model.setScale(size);
		boxField->setSize(size);
	});
}

void PanelItemUI::setCaptured(bool captured) {
	this->captured = captured;
	inputHandler.setEnabled(!captured);
	model.setEnabled(!captured);
}
