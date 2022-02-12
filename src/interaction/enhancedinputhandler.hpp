#pragma once

#include <stardustxr/fusion/types/input/inputhandler.hpp>
#include <stardustxr/fusion/types/input/types/handinput.hpp>
#include <stardustxr/fusion/types/input/types/pointerinput.hpp>
#include <stardustxr/fusion/types/fields/field.hpp>

using namespace StardustXRFusion;

class EnhancedInputHandler : public InputHandler {
public:
	struct InputMethod {
		std::string uuid;
		Datamap datamap;

		bool operator<(const InputMethod& other) {
			return uuid < other.uuid;
		}
	};

	struct PointerInputMethod : InputMethod {
		PointerInput pointer;
	};
	struct HandInputMethod : InputMethod {
		HandInput hand;
	};

	struct Action {
		bool captureOnTrigger = true;
		std::function<bool(const std::string uuid, const PointerInput &, const Datamap &)> pointerActiveCondition = [](const std::string uuid, const PointerInput &pointer, const Datamap &datamap){return false;};
		std::function<bool(const std::string uuid, const HandInput &,    const Datamap &)> handActiveCondition    = [](const std::string uuid, const HandInput &hand,       const Datamap &datamap){return false;};

		std::vector<InputMethod> startedActing;
		std::vector<InputMethod> activelyActing;
		std::vector<InputMethod> stoppedActing;

		std::vector<InputMethod> queuedActiveInputs;
	};

	explicit EnhancedInputHandler(Spatial *parent, Field &field, SKMath::vec3 origin, SKMath::quat orientation);
	explicit EnhancedInputHandler(Spatial *parent, SKMath::vec3 origin, SKMath::quat orientation);

	std::vector<Action> actions;
	void update();

protected:
	bool pointerInputEvent(const std::string uuid, const PointerInput &, const Datamap &);
	bool handInputEvent(const std::string uuid, const HandInput &, const Datamap &);
};
