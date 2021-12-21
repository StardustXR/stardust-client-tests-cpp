#include "item.hpp"
#include <stardustxr/fusion/types/fields/field.hpp>
#include <stardustxr/fusion/types/spatial.hpp>

using namespace SKMath;

GrabbableItemUI::GrabbableItemUI(StardustXRFusion::Item *item) :
Grabbable(vec3_zero, quat_identity, StardustXRFusion::Field::Empty(), 0.05f),
item(item) {
	item->setSpatialParent(this);
	inputHandler.setEnabled(false);
	item->getTransform([this](vec3 pos, quat rot, vec3 scl) {
		this->setPose({pos, rot});
		inputHandler.setEnabled(true);
	});
}

bool GrabbableItemUI::isGrabbing() {
	return Grabbable::isActive();
}
bool GrabbableItemUI::grabbingChanged() {
	return Grabbable::activeChanged();
}
