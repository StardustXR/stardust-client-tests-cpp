#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/model.hpp>
#include <stardustxr/fusion/types/items/panel.hpp>

using namespace StardustXRFusion;
using namespace SKMath;

int main() {
	Setup();

	float centerWidth = 0.15f;
	Model center(Root(), "../res/item/scrollpanel.glb");
	Model handles[2] = {
		Model(Root(), "../res/item/scrollpanelhandle.glb",  vec3_right * centerWidth / 2),
		Model(Root(), "../res/item/scrollpanelhandle.glb", -vec3_right * centerWidth / 2, quat_identity, vec3{-1, -1, 1}),
	};
	PanelItem *panel = nullptr;

	PanelItem::registerUIHandler([&](bool active, PanelItem &panelItem, uint32_t, uint32_t) {
		if(active) {
			if(panel != nullptr) {
				delete panel;
			}
			panel = new PanelItem(panelItem);
			panel->applySurfaceMaterial(center, 0);
		}
	});

	OnLogicStep([&](double, double) {
		if(panel != nullptr) {
			panel->getData([&center, &centerWidth, &handles](uint32_t width, uint32_t height) {
				float scale = (float)width / (float)height;
				centerWidth = 0.15f * scale;
				center.setScale(vec3{scale, 1, 1});
				handles[0].setOrigin( vec3_right * centerWidth / 2);
				handles[1].setOrigin(-vec3_right * centerWidth / 2);
			});
		}
	});

	StallMainThread();
}
