#include "src/item/environmentitem.hpp"
#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/items/environment.hpp>
#include <stardustxr/fusion/types/model.hpp>
#include <vector>

using namespace StardustXRFusion;
using namespace SKMath;

std::vector<EnvironmentItemUI> environmentItems;
std::vector<Model> models;

int main(int argc, char *argv[]) {
	StardustXRFusion::Setup();

	EnvironmentItem::registerUIHandler([&](bool create, EnvironmentItem &item) {
		if(create) {
			environmentItems.emplace_back(item);
			// models.emplace_back((--environmentItems.end()).base(), "../res/item/skyball.glb", vec3_zero, quat_identity, vec3_one * 0.07);
			// (--models.end())->setMaterialProperty(0, "diffuse", item.path);
		}
	});

	OnLogicStep([&](double delta, double) {
		for(EnvironmentItemUI &ui : environmentItems) {
			ui.update();
		}
	});
	
	StardustXRFusion::StallMainThread();
}
