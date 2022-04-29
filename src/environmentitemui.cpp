#include <stardustxr/fusion/values/glm.hpp>
#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/items/types/environment.hpp>
#include <stardustxr/fusion/types/drawable/model.hpp>
#include "src/item/environmentitem.hpp"
#include <vector>

using namespace StardustXRFusion;


std::vector<EnvironmentItemUI> environmentItems;
std::vector<Model> models;

int main(int argc, char *argv[]) {
	StardustXRFusion::Setup();

	EnvironmentItem::registerUIHandlers([&](EnvironmentItem &item, EnvironmentItem::Data data) {
		environmentItems.emplace_back(item, data);
		// models.emplace_back((--environmentItems.end()).base(), "res/item/skyball.glb", Vec3::Zero, Quat::Identity, Vec3::One * 0.07);
		// (--models.end())->setMaterialProperty(0, "diffuse", item.path);
	}, [&](EnvironmentItem &item) {
		environmentItems.erase(std::remove_if(
			environmentItems.begin(),
			environmentItems.end(),
			[&item](EnvironmentItemUI &listEnvUI) {
				return listEnvUI.getNodeName() == item.getNodeName();
			}),
			environmentItems.end()
		);
	});

	OnLogicStep([&](double delta, double) {
		for(EnvironmentItemUI &ui : environmentItems) {
			ui.update();
		}
	});
	
	StardustXRFusion::RunEventLoop();
}
