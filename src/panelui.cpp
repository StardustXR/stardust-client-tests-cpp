#include "src/item/panel.hpp"
#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/items/panel.hpp>
#include <stardustxr/fusion/types/model.hpp>
#include <memory>
#include <vector>

using namespace StardustXRFusion;
using namespace SKMath;

std::vector<std::unique_ptr<PanelItemUI>> panels;

int main(int argc, char *argv[]) {
	StardustXRFusion::Setup();

	PanelItem::registerUIHandler([&](bool create, PanelItem &item, uint32_t width, uint32_t height) {
		if(create) {
			panels.emplace_back(new PanelItemUI(item, width, height));
		} else {
			panels.erase(std::remove_if(
				panels.begin(),
				panels.end(),
				[&item](std::unique_ptr<PanelItemUI> &listSurface) {
					return listSurface->panel.getNodeName() == item.getNodeName();
				}),
				panels.end()
			);
		}
	});

	OnLogicStep([&](double delta, double) {
		for(std::unique_ptr<PanelItemUI> &ui : panels) {
			ui->update();
		}
	});
	
	StardustXRFusion::StallMainThread();
}
