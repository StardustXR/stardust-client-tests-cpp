#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "../include/math_util.hpp"

#include <linux/input-event-codes.h>

#include "interaction/xinteract.hpp"

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/data/receiver.hpp>
#include <stardustxr/fusion/types/drawable/model.hpp>
#include <stardustxr/fusion/types/fields/boxfield.hpp>
#include <stardustxr/fusion/types/input/inputactionhandler.hpp>
#include <stardustxr/fusion/types/input/actions/singleactor.hpp>
#include <stardustxr/fusion/types/input/types/pointerinput.hpp>
#include <stardustxr/fusion/types/input/types/handinput.hpp>
#include <stardustxr/fusion/types/items/acceptors/panel.hpp>

using namespace StardustXRFusion;
using namespace SKMath;

PanelItem *panel = nullptr;
float maxDistance = 0.005f;
float scrollMultiplier = 5;

int main(int, char *[]) {
	StardustXRFusion::Setup();
	Spatial root(Root(), vec3_zero, quat_identity, vec3_one, true, true, false, true);

	Model crt(&root, "../res/panelshell/crt.glb");

	BoxField panelAcceptorField(&root, vec3{0, 0.23f, 0.08f}, quat_identity, vec3{0.471f, 0.46f, 0.168f});
	PanelItemAcceptor panelAcceptor(&root, panelAcceptorField, vec3_zero, quat_identity);

	Spatial screenCenter(&root, vec3{0, 0.27f, -0.004927f});
	BoxField inputField(&screenCenter, vec3_zero, quat_identity, vec3{0.4f, 0.312254f, 0.022191f});
	InputActionHandler inputHandler(&screenCenter, inputField, vec3_zero, quat_identity);
	PulseReceiver keyReceiver(&screenCenter, inputField);

	keyReceiver.setMask([&](flexbuffers::Builder &fbb) {
		fbb.String("type", "keyboard");
	});

	keyReceiver.onDataReceived = [&](std::string senderUUID, flexbuffers::Reference ref) {
		if(panel && ref.IsMap()) {
			flexbuffers::Map map = ref.AsMap();

			if(map["keymap"].IsString())
				panel->setKeymap(map["keymap"].AsString().str());
			if(map["pressed"].IsUInt())
				panel->setKeyState(map["pressed"].AsUInt32(), true);
			if(map["released"].IsUInt())
				panel->setKeyState(map["released"].AsUInt32(), false);

			if(map["modifiers"].IsMap()) {
				flexbuffers::Map modMap = map["modifiers"].AsMap();

				panel->setKeyModStates(
					modMap["depressed"].AsUInt32(),
					modMap["latched"].AsUInt32(),
					modMap["locked"].AsUInt32(),
					modMap["group"].AsUInt32());
			}
		}
	};

	SingleActorAction hoverAction(false);
	hoverAction.pointerActiveCondition = [](const std::string uuid, const PointerInput &pointer, const Datamap &datamap){
		return panel && pointer.origin.z < 0 && pointer.distance < maxDistance;
	};
	hoverAction.handActiveCondition = [](const std::string uuid, const HandInput &hand, const Datamap &datamap){
		const vec3 pinchPos = (hand.thumb().tip().pose.position + hand.index().tip().pose.position) * 0.5f;
		return panel && pinchPos.z < 0 && hand.distance < maxDistance;
	};
	inputHandler.actions.push_back(&hoverAction);

	SingleActorAction leftClick(true, true, &hoverAction);
	leftClick.pointerActiveCondition = [](const std::string uuid, const PointerInput &pointer, const Datamap &datamap){
		return panel && (datamap.getFloat("select") > 0.9f || datamap.getFloat("left") > 0.9f);
	};
	leftClick.handActiveCondition = [](const std::string uuid, const HandInput &hand, const Datamap &datamap){
		return panel && datamap.getFloat("pinchStrength") > 0.9f;
	};
	inputHandler.actions.push_back(&leftClick);

	StardustXRFusion::OnLogicStep([&](double delta, double) {
		inputHandler.update();
		if(!panel)
			return;
		if(hoverAction.actorActing) {
			vec2 cursorUnit = vec2_zero;
			vec2 cursorPixel = vec2_zero;

			PointerInput *pointer = hoverAction.actor->pointer.get();
			HandInput *hand = hoverAction.actor->hand.get();
			if(pointer) {
				cursorUnit.x = pointer->deepestPoint.x;
				cursorUnit.y = pointer->deepestPoint.y;

				Datamap *datamap = &hoverAction.actor->datamap;
				const vec2 scroll = datamap->getVec2("scroll");
				if(scroll.x != 0 && scroll.y != 0)
					panel->scrollPointerAxis(0, scroll.x * scrollMultiplier, scroll.y * scrollMultiplier, (int32_t) scroll.x, (int32_t) scroll.y);
			}
			if(hand) {
				const vec3 pinchPos = (hand->thumb().tip().pose.position + hand->index().tip().pose.position) * 0.5f;
				cursorUnit.x = pinchPos.x;
				cursorUnit.y = pinchPos.y;
			}

			cursorPixel.x = mapClamped(cursorUnit.x,         0.2f,        -0.2f, 0, 800);
			cursorPixel.y = mapClamped(cursorUnit.y,  0.312254f/2, -0.312254f/2, 0, 600);
			panel->setPointerPosition(cursorPixel);
		}
		if(leftClick.actorStarted)
			panel->setPointerButtonPressed(BTN_LEFT, true);
		if(leftClick.actorStopped)
			panel->setPointerButtonPressed(BTN_LEFT, false);
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
		panel->setKeyboardActive(true);
	};
	panelAcceptor.itemReleasedMethod = [&](PanelItem &releasedPanel) {
		if(panel && *panel == releasedPanel)
			panel = nullptr;
	};

	StardustXRFusion::RunEventLoop();
}
