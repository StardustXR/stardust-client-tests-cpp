#include "environmentitem.hpp"
#include <stardustxr/fusion/types/fields/field.hpp>
#include <stardustxr/fusion/types/fields/spherefield.hpp>

using namespace StardustXRFusion;


EnvironmentItemUI::EnvironmentItemUI(StardustXRFusion::EnvironmentItem &item, StardustXRFusion::EnvironmentItem::Data data, float size) :
Grabbable(Vec3::Zero, Quat::Identity, SphereField(this, Vec3::Zero, size / 2), 0.05f),
environmentItem(item),
model(this, "res/item/skyball.glb", Vec3::Zero, Quat::Identity, Vec3::One * size),
sphereField(static_cast<SphereField *>(&field)) {
	model.setMaterialProperty(0, "diffuse", data.path);
}
