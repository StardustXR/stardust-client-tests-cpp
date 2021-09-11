#include "item.hpp"

using namespace SKMath;

Item::Item(vec3 origin, quat orientation) :
grabbable(origin, orientation, field) {
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