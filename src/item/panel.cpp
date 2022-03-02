#include "panel.hpp"
#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/fields/field.hpp>
#include <stardustxr/fusion/types/fields/spherefield.hpp>

using namespace SKMath;

PanelItemUI::PanelItemUI(StardustXRFusion::PanelItem &item, uint32_t pixelWidth, uint32_t pixelHeight, float width, float thickness) :
Grabbable(vec3_zero, quat_identity, StardustXRFusion::Field::Empty(), 0.1f),
panel(item),
width(width),
thickness(thickness),
model(this, "../res/item/panelitem.glb", vec3_zero, quat_identity, vec3{width, width * pixelHeight / pixelWidth, thickness}),
boxField(this, vec3_zero, quat_identity, vec3{width, width * pixelHeight / pixelWidth, thickness}) {
	setField(&boxField);
	item.applySurfaceMaterial(model, 0);
	item.setSpatialParent(this);
	inputHandler.actions["close"] = [this]() {
		panel.close();
	};
	inputHandler.updateActions();
}

void PanelItemUI::update() {
	Grabbable::update();
	if(xInteract.hasActiveChanged()) {
		if(!xInteract.isActive())
			panel.triggerAccept();
//		else
//			panel.release();
	}
	panel.getData([this](StardustXRFusion::PanelItem::Data data) {
		pixelWidth = data.width;
		pixelHeight = data.height;
		model.setScale(  vec3{width, width * pixelHeight / pixelWidth, thickness});
		boxField.setSize(vec3{width, width * pixelHeight / pixelWidth, thickness});
	});
}
