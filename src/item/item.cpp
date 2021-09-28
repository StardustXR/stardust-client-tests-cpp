#include "item.hpp"
#include <stardustxr/fusion/types/fields/field.hpp>
#include <stardustxr/fusion/types/spatial.hpp>

using namespace SKMath;

Item::Item(vec3 origin, quat orientation) :
Grabbable(origin, orientation, StardustXRFusion::Field::Empty()) {}

void Item::update() {
	Grabbable::update();
}
void Item::onAccepted(Spatial &acceptor) {
	printf("Item accepted!\n");
	setSpatialParent(&acceptor);
	setPose(pose_t { vec3_zero, quat_identity });
}

bool Item::isGrabbing() {
	return Grabbable::isActive();
}
bool Item::grabbingChanged() {
	return Grabbable::activeChanged();
}

void Item::distance(Spatial *space, SKMath::vec3 point, std::function<void (float)> callback) {
	field->distance(space, point, callback);
}
