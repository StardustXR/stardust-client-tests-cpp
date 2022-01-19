#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/model.hpp>
#include <stardustxr/fusion/types/fields/boxfield.hpp>
#include <stardustxr/fusion/types/input/inputhandler.hpp>
#include <stardustxr/fusion/types/input/types/pointerinput.hpp>
#include <stardustxr/fusion/types/items/types/panel.hpp>
#include "../include/math_util.hpp"

#include <linux/input-event-codes.h>

#include "interaction/xinteract.hpp"

using namespace StardustXRFusion;
using namespace SKMath;

int main() {
	Setup();

	uint32_t surfWidth, surfHeight;
	float centerWidth = 0.15f;
	Model center(Root(), "../res/item/scrollpanel.glb");
	BoxField box(nullptr, vec3_zero, quat_identity, vec3{0.15f, 0.15f, 0.0f});
	InputHandler handler(nullptr, box, vec3_forward * 0.005f, quat_identity);
	Model handles[2] = {
		Model(Root(), "../res/item/scrollpanelhandle.glb",  vec3_right * centerWidth / 2),
		Model(Root(), "../res/item/scrollpanelhandle.glb", -vec3_right * centerWidth / 2, quat_identity, vec3{-1, -1, 1}),
	};
	PanelItem *panel = nullptr;

	XInteract xInteract;
	float scrollMultiplier = 5;
	float maxDistance = 0.005f;
	handler.pointerHandlerMethod = [&](const PointerInput &pointer, const Datamap &datamap) {
		if(panel == nullptr || pointer.distance > maxDistance)
			return false;
		xInteract.input(false);
		const SKMath::vec3 deepestPoint = pointer.origin + (pointer.direction * datamap.getFloat("deepestPointDistance"));
		if(pointer.origin.z > 0 && pointer.distance < maxDistance) {
			vec2 cursor;
			cursor.x = map(deepestPoint.x, -centerWidth/2, centerWidth/2, 0, surfWidth);
			cursor.y = map(deepestPoint.y,          .15/2,        -.15/2, 0, surfHeight);
			panel->setPointerPosition(cursor);

			const float selectPressed = datamap.getFloat("select");
			panel->setPointerButtonPressed(BTN_LEFT, selectPressed > 0.9f);

			const vec2 scroll = datamap.getVec2("scroll");
			panel->scrollPointerAxis(0, scroll.x * scrollMultiplier, scroll.y * scrollMultiplier, (int32_t) scroll.x, (int32_t) scroll.y);

//			if(selectPressed > 0.9f) {
//				panel->touchDown(0, cursor.x, cursor.y);
//				panel->touchMove(0, cursor.x, cursor.y);
//			} else {
//				panel->touchUp(0);
//			}
		}
		return false;
	};

	PanelItem::registerUIHandlers([&](PanelItem &panelItem, PanelItem::Data data) {
		if(panel != nullptr) {
			delete panel;
		}
		panel = new PanelItem(panelItem);
		panel->applySurfaceMaterial(center, 0);
	}, [&](PanelItem &panelItem) {
	});

	OnLogicStep([&](double, double) {
		if(panel != nullptr) {
			panel->getData([&](PanelItem::Data data) {
				panel->setPointerActive(true);
				surfWidth  = data.width;
				surfHeight = data.height;
				float scale = (float)data.width / (float)data.height;
				centerWidth = 0.15f * scale;

				center.setScale(vec3{scale, 1, 1});
				box.setSize(vec3{centerWidth, 0.15f, 0.0f});

				handles[0].setOrigin( vec3_right * centerWidth / 2);
				handles[1].setOrigin(-vec3_right * centerWidth / 2);
			});
		}
	});

	StallMainThread();
}
