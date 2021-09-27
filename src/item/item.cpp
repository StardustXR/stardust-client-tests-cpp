#include "item.hpp"
#include <stardustxr/fusion/types/spatial.hpp>

using namespace SKMath;

Item::Item(vec3 origin, quat orientation) :
StardustXRFusion::Spatial(nullptr),
grabbable(origin, orientation, field),
field(this, SKMath::vec3_zero, SKMath::quat_identity) {
	this->setSpatialParent(&grabbable);
}

void Item::update() {
	grabbable.update();
}

void Item::onAccepted() {
	grabbable.resetStart();
}


bool Item::isGrabbing() {
	return grabbable.isActive();
}
bool Item::grabbingChanged() {
	return grabbable.activeChanged();
}