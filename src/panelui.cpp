#include "src/item/panel.hpp"
#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/items/types/panel.hpp>
#include <stardustxr/fusion/types/drawable/model.hpp>
#include <memory>
#include <map>

using namespace StardustXRFusion;
using namespace SKMath;

std::map<PanelItem, std::unique_ptr<PanelItemUI>> panels;

int main(int argc, char *argv[]) {
	StardustXRFusion::Setup();

	PanelItem::registerUIHandlers([&](PanelItem &item, PanelItem::Data data) {
		PanelItemUI *ui = new PanelItemUI(item, data.width, data.height, 0.15f, 0.01f);
		panels.emplace(item, ui);
		ui->setOrigin(vec3_forward * 0.4f);
	}, [&](PanelItem &item) {
		panels[item]->setEnabled(false);
	}, [&](PanelItem &item, PanelItem::Data data) {
		panels[item]->setEnabled(true);
	}, [&](PanelItem &item) {
		panels.erase(item);
	});

	OnLogicStep([&](double delta, double) {
		for(auto &panelPair : panels) {
			panelPair.second->update();
		}
	});
	
	StardustXRFusion::StallMainThread();
}
