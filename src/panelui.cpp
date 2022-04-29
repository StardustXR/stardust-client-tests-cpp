#include "src/item/panel.hpp"
#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/values/glm.hpp>
#include <stardustxr/fusion/types/items/types/panel.hpp>
#include <stardustxr/fusion/types/drawable/model.hpp>
#include <memory>
#include <map>

using namespace StardustXRFusion;


std::map<PanelItem, std::unique_ptr<PanelItemUI>> panels;

int main(int argc, char *argv[]) {
	if(!StardustXRFusion::Setup())
		return 0;

	PanelItem::registerUIHandlers([&](PanelItem &item, PanelItem::Data data) {
		PanelItemUI *ui = new PanelItemUI(item, data.width, data.height, 0.05f, 0.01f);
		panels.emplace(item, ui);
		ui->setOrigin(Vec3::Forward * 0.4f);
	}, [&](PanelItem &item) {
		panels[item]->setCaptured(true);
	}, [&](PanelItem &item, PanelItem::Data data) {
		panels[item]->setCaptured(false);
	}, [&](PanelItem &item) {
		panels.erase(item);
	});

	OnLogicStep([&](double delta, double) {
		for(auto &panelPair : panels) {
			panelPair.second->update();
		}
	});
	
	StardustXRFusion::RunEventLoop();
}
