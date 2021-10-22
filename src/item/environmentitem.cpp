#include "environmentitem.hpp"
#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/fields/field.hpp>
#include <stardustxr/fusion/types/fields/spherefield.hpp>

#include "acceptor.hpp"

using namespace SKMath;

EnvironmentItem::EnvironmentItem(std::string environment, vec3 origin, quat orientation, float size) :
Item(origin, orientation),
model(this, "../res/item/skyball.glb", vec3_zero, quat_identity, vec3_one * size),
sphereField(this, vec3_zero, size / 2) {
	setField(&sphereField);
	ItemAcceptor<EnvironmentItem>::addItem(this);
	this->environment = environment;
	model.setMaterialProperty(0, "diffuse", environment);

	inputHandler.actions["setSkytex"] = [environment] {
		StardustXRFusion::SetSkytex(environment);
	};
	inputHandler.actions["setSkylight"] = [environment] {
		StardustXRFusion::SetSkylight(environment);
	};
	inputHandler.updateActions();
}

void EnvironmentItem::onAccepted(Spatial &acceptor) {
	StardustXRFusion::SetSkytex(environment);
	Item::onAccepted(acceptor);
}
