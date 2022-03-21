#include "environmentitem.hpp"
#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/fields/field.hpp>
#include <stardustxr/fusion/types/fields/spherefield.hpp>

using namespace SKMath;

EnvironmentItemUI::EnvironmentItemUI(StardustXRFusion::EnvironmentItem &item, StardustXRFusion::EnvironmentItem::Data data, float size) :
Grabbable(vec3_zero, quat_identity, StardustXRFusion::Field::Empty(), 0.05f),
environmentItem(item),
model(this, "../res/item/skyball.glb", vec3_zero, quat_identity, vec3_one * size),
sphereField(this, vec3_zero, size / 2) {
	setField(&sphereField);
	model.setMaterialProperty(0, "diffuse", data.path);
}
