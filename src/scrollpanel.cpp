#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/drawable/model.hpp>
#include <stardustxr/fusion/types/fields/boxfield.hpp>
#include <stardustxr/fusion/types/input/inputhandler.hpp>
#include <stardustxr/fusion/types/input/types/pointerinput.hpp>
#include <stardustxr/fusion/types/items/types/panel.hpp>
#include "../include/math_util.hpp"

#include <linux/input-event-codes.h>
#include <xkbcommon/xkbcommon.h>
#include <xkbcommon/xkbcommon-x11.h>

#include "interaction/xinteract.hpp"

using namespace StardustXRFusion;


int main() {
	Setup();

	xkb_keymap *keymap;
	{
		struct xkb_context *xkb_ctx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
		struct xkb_rule_names names = {};
		keymap = xkb_keymap_new_from_names(xkb_ctx, &names, XKB_KEYMAP_COMPILE_NO_FLAGS);
	}
	std::string keymapString = xkb_keymap_get_as_string(keymap, XKB_KEYMAP_FORMAT_TEXT_V1);

	uint32_t surfWidth, surfHeight;
	float centerWidth = 0.15f;
	Model center(Root(), "res/item/scrollpanel.glb");
	BoxField box(nullptr, Vec3::Zero, Quat::Identity, Vec3{0.15f, 0.15f, 0.0f});
	InputHandler handler(nullptr, box, Vec3::Forward * 0.005f, Quat::Identity);
	Model handles[2] = {
		Model(Root(), "res/item/scrollpanelhandle.glb",  Vec3::Right * centerWidth / 2),
		Model(Root(), "res/item/scrollpanelhandle.glb", -Vec3::Right * centerWidth / 2, Quat::Identity, Vec3{-1, -1, 1}),
	};
	PanelItem *panel = nullptr;

	XInteract xInteract;
	float scrollMultiplier = 5;
	float maxDistance = 0.005f;
	handler.pointerHandlerMethod = [&](const std::string uuid, const PointerInput &pointer, const Datamap &datamap) {
		if(panel == nullptr || pointer.distance > maxDistance)
			return false;
		panel->setKeyboardActive(pointer.distance < maxDistance);
		xInteract.input(false);
		const Vec3 deepestPoint = pointer.origin + (pointer.direction * datamap.getFloat("deepestPointDistance"));
		if(pointer.origin.z > 0 && pointer.distance < maxDistance) {
			Vec2 cursor;
			cursor.x = map(deepestPoint.x, -centerWidth/2, centerWidth/2, 0, surfWidth);
			cursor.y = map(deepestPoint.y,          .15/2,        -.15/2, 0, surfHeight);
			panel->setPointerPosition(cursor);

			const float selectPressed = datamap.getFloat("select");
			panel->setPointerButtonPressed(BTN_LEFT, selectPressed > 0.9f);

			const Vec2 scroll = datamap.getVec2("scroll");
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
		panel->setKeymap(keymapString);
	}, [&](PanelItem &item) {

	}, [&](PanelItem &item, PanelItem::Data data) {

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

				center.setScale(Vec3{scale, 1, 1});
				box.setSize(Vec3{centerWidth, 0.15f, 0.0f});

				handles[0].setOrigin( Vec3::Right * centerWidth / 2);
				handles[1].setOrigin(-Vec3::Right * centerWidth / 2);
			});
		}
	});

	RunEventLoop();
}
