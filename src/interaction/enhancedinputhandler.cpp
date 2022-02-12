#include "enhancedinputhandler.hpp"

EnhancedInputHandler::EnhancedInputHandler(Spatial *parent, Field &field, SKMath::vec3 origin, SKMath::quat orientation) :
	InputHandler(parent, field, origin, orientation) {

}

EnhancedInputHandler::EnhancedInputHandler(Spatial *parent, SKMath::vec3 origin, SKMath::quat orientation) :
	InputHandler(parent, origin, orientation) {

}

void EnhancedInputHandler::update() {
	for(Action &action : actions) {
		std::sort(action.queuedActiveInputs.begin(), action.queuedActiveInputs.end(), [](InputMethod &a, InputMethod &b) {
			return a.uuid < b.uuid;
		});

		std::set_difference(action.queuedActiveInputs.begin(), action.queuedActiveInputs.end(),
							action.activelyActing.begin(), action.activelyActing.end(),
							std::inserter(action.startedActing, action.startedActing.begin()));
		std::set_difference(action.activelyActing.begin(), action.activelyActing.end(),
							action.queuedActiveInputs.begin(), action.queuedActiveInputs.end(),
							std::inserter(action.stoppedActing, action.stoppedActing.begin()));

		action.activelyActing = action.queuedActiveInputs;
		action.queuedActiveInputs.clear();
	}
}

bool EnhancedInputHandler::pointerInputEvent(const std::string uuid, const PointerInput &pointer, const Datamap &datamap) {
	bool capture = false;
	for(Action &action : actions) {
		bool acted = action.pointerActiveCondition(uuid, pointer, datamap);
		if(acted) {
			if(action.captureOnTrigger)
				capture = true;

			action.activelyActing.push_back(PointerInputMethod{{uuid, datamap}, pointer});
		}
	}
	return capture;
}

bool EnhancedInputHandler::handInputEvent(const std::string uuid, const HandInput &hand, const Datamap &datamap) {
	bool capture = false;
	for(Action &action : actions) {
		bool acted = action.handActiveCondition(uuid, hand, datamap);
		if(acted) {
			if(action.captureOnTrigger)
				capture = true;

			action.activelyActing.push_back(HandInputMethod{{uuid, datamap}, hand});
		}
	}
	return capture;
}
