#include "environmentitem.hpp"
#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/fields/field.hpp>
#include <stardustxr/fusion/types/fields/spherefield.hpp>

#include "acceptor.hpp"

using namespace SKMath;

EnvironmentItem::EnvironmentItem(std::string environment, vec3 origin, quat orientation, float size) :
Item(origin, orientation),
field(&grabbable.item, vec3_zero, size / 2),
model(&grabbable.item, "../res/item/skyball.glb", vec3_zero, quat_identity, vec3_one * size) {
	grabbable.setField(&field);

	ItemAcceptor<EnvironmentItem>::addItem(this);

	model.setMaterialProperty(0, "diffuse", environment);
}
