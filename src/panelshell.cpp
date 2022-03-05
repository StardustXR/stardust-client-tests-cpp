#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "../include/math_util.hpp"

#include <linux/input-event-codes.h>

#include "interaction/xinteract.hpp"

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/drawable/model.hpp>
#include <stardustxr/fusion/types/fields/boxfield.hpp>
#include <stardustxr/fusion/types/input/inputhandler.hpp>
#include <stardustxr/fusion/types/input/types/pointerinput.hpp>
#include <stardustxr/fusion/types/input/types/handinput.hpp>
#include <stardustxr/fusion/types/items/acceptors/panel.hpp>
#include "interaction/inputactionhandler.hpp"

using namespace StardustXRFusion;
using namespace SKMath;

PanelItem *panel = nullptr;

int main(int, char *[]) {
	StardustXRFusion::Setup();
	Spatial root = Spatial::create(nullptr, vec3{0, -0.5f, -0.5f}, quat_identity, vec3_one, true, true, false, true);

	Model crt(&root, "../res/panelshell/crt.glb", vec3_zero, quat_from_angles(0, 180, 0));

	BoxField panelAcceptorField(&root, vec3{0, 0.23f, -0.08f}, quat_identity, vec3{0.471f, 0.46f, 0.168f});
	PanelItemAcceptor panelAcceptor(&root, panelAcceptorField, vec3_zero, quat_identity);

	Spatial screenCenter = Spatial::create(&root, vec3{0, 0.27f, -0.004927f});
	BoxField inputField(&screenCenter, vec3_zero, quat_identity, vec3{0.4f, 0.312254f, 0.022191f});
	InputActionHandler inputHandler(&screenCenter, inputField, vec3_zero, quat_identity);

	float maxDistance = 0.005f;
	inputHandler.inputActions.push_back(InputActionHandler::Action{
		false,
		[maxDistance](const std::string uuid, const PointerInput &pointer, const Datamap &datamap){
			return pointer.origin.z > 0 && pointer.distance < maxDistance;
		},
		[maxDistance](const std::string uuid, const HandInput &hand, const Datamap &datamap){
			const vec3 pinchPos = (hand.thumb().tip().pose.position + hand.index().tip().pose.position) * 0.5f;
			return pinchPos.z > 0 && hand.distance < maxDistance;
		}
	});
	InputActionHandler::Action *inRangeAction= inputHandler.inputActions.begin().base();

	float scrollMultiplier = 5;
//	inputHandler.pointerHandlerMethod = [&](const std::string uuid, const PointerInput &pointer, const Datamap &datamap) {
//			vec2 cursor;
//			cursor.x = map(pointer.deepestPoint.x,        -0.2f,         0.2f, 0, 800);
//			cursor.y = map(pointer.deepestPoint.y,  0.312254f/2, -0.312254f/2, 0, 600);
//			panel->setPointerPosition(cursor);

//			const float selectPressed = datamap.getFloat("select");
//			panel->setPointerButtonPressed(BTN_LEFT, selectPressed > 0.9f);

//			const vec2 scroll = datamap.getVec2("scroll");
//			panel->scrollPointerAxis(0, scroll.x * scrollMultiplier, scroll.y * scrollMultiplier, (int32_t) scroll.x, (int32_t) scroll.y);
//		}
//		return false;
//	};
//	inputHandler.handHandlerMethod = [&](const std::string uuid, const HandInput &hand, const Datamap &datamap) {
//		if(panel == nullptr || hand.distance > maxDistance)
//			return false;

//		const vec3 pinchPos = (hand.thumb().tip().pose.position + hand.index().tip().pose.position) * 0.5f;
//		if(pinchPos.z > 0 && hand.distance < maxDistance) {
//			vec2 cursor;
//			cursor.x = map(pinchPos.x,        -0.2f,         0.2f, 0, 800);
//			cursor.y = map(pinchPos.y,  0.312254f/2, -0.312254f/2, 0, 600);
//			panel->setPointerPosition(cursor);

//			const float pinchStrength = datamap.getFloat("pinchStrength");
//			panel->setPointerButtonPressed(BTN_LEFT, pinchStrength > 0.9f);
//		}
//		return false;
//	};

	StardustXRFusion::OnLogicStep([&](double delta, double) {
		inputHandler.update();
		if(!inRangeAction->activelyActing.empty()) {
			InputActionHandler::InputMethod *firstInputMethod = inRangeAction->activelyActing.begin().base();
		}
	});

	panelAcceptor.itemCapturedMethod = [&crt](PanelItem &panelItem, PanelItem::Data data) {
		if(panel && *panel != panelItem) {
			panel->setPose(pose_t{vec3{0, -0.25f, -0.4f}, quat_identity});
			panel->release();
		}
		panel = new PanelItem(panelItem);
		panel->applySurfaceMaterial(crt, 7);
		panel->resize(800, 600);
		panel->setPointerActive(true);
	};
	panelAcceptor.itemReleasedMethod = [](PanelItem &panel) {
	};

	StardustXRFusion::StallMainThread();
}
