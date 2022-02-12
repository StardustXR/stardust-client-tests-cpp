#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "../include/math_util.hpp"

#include <linux/input-event-codes.h>

#include "interaction/xinteract.hpp"

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/model.hpp>
#include <stardustxr/fusion/types/fields/boxfield.hpp>
#include <stardustxr/fusion/types/input/inputhandler.hpp>
#include <stardustxr/fusion/types/input/types/pointerinput.hpp>
#include <stardustxr/fusion/types/items/acceptors/panel.hpp>

using namespace StardustXRFusion;
using namespace SKMath;

PanelItem *panel = nullptr;

int main(int, char *[]) {
	StardustXRFusion::Setup();
	Spatial root = Spatial::create(nullptr, vec3_forward*0.5f, quat_identity, vec3_one, true, true, false);

	Model crt(&root, "../res/panelshell/crt.glb", vec3_zero, quat_from_angles(0, 180, 0));

	BoxField panelAcceptorField(&root, vec3{0, 0.23f, -0.08f}, quat_identity, vec3{0.471f, 0.46f, 0.168f});
	PanelItemAcceptor panelAcceptor(&root, panelAcceptorField, vec3_zero, quat_identity);

	Spatial screenCenter = Spatial::create(&root, vec3{0, 0.27f, -0.004927f});
	BoxField inputField(&screenCenter, vec3_zero, quat_identity, vec3{0.4f, 0.312254f, 0.022191f});
	InputHandler inputHandler(&screenCenter, inputField, vec3_zero, quat_identity);

	float scrollMultiplier = 5;
	float maxDistance = 0.005f;
	inputHandler.pointerHandlerMethod = [&](const std::string uuid, const PointerInput &pointer, const Datamap &datamap) {
		if(panel == nullptr || pointer.distance > maxDistance)
			return false;
		const SKMath::vec3 deepestPoint = pointer.origin + (pointer.direction * datamap.getFloat("deepestPointDistance"));
		if(pointer.origin.z > 0 && pointer.distance < maxDistance) {
			vec2 cursor;
			cursor.x = map(deepestPoint.x,        -0.2f,         0.2f, 0, 800);
			cursor.y = map(deepestPoint.y,  0.312254f/2, -0.312254f/2, 0, 600);
			panel->setPointerPosition(cursor);

			const float selectPressed = datamap.getFloat("select");
			panel->setPointerButtonPressed(BTN_LEFT, selectPressed > 0.9f);

			const vec2 scroll = datamap.getVec2("scroll");
			panel->scrollPointerAxis(0, scroll.x * scrollMultiplier, scroll.y * scrollMultiplier, (int32_t) scroll.x, (int32_t) scroll.y);
		}
		return false;
	};

	panelAcceptor.itemCapturedMethod = [&crt](PanelItem &panelItem, PanelItem::Data data) {
		if(panel && panel->getNodeName() != panelItem.getNodeName())
			panel->release();
		panel = new PanelItem(panelItem);
		panel->applySurfaceMaterial(crt, 7);
		panel->resize(800, 600);
		panel->setPointerActive(true);
	};
	panelAcceptor.itemReleasedMethod = [](PanelItem &panel) {
	};

	StardustXRFusion::StallMainThread();
}
