#pragma once

#include <stardustxr/fusion/types/input/inputhandler.hpp>
#include <stardustxr/fusion/types/input/types/handinput.hpp>
#include <stardustxr/fusion/types/input/types/pointerinput.hpp>
#include <stardustxr/fusion/types/fields/field.hpp>

#include <memory>

using namespace StardustXRFusion;

class InputActionHandler : public InputHandler {
public:
	struct InputMethod {
		InputMethod(const std::string uuid, const Datamap datamap, PointerInput *pointer, HandInput *hand);
		InputMethod(const InputMethod &copyFrom);

		std::string uuid;
		Datamap datamap;
		std::unique_ptr<PointerInput> pointer;
		std::unique_ptr<HandInput> hand;

		void operator=(const InputMethod &other);
		bool operator<(const InputMethod &other);
		bool operator==(const std::string &uuid);
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

	explicit InputActionHandler(Spatial *parent, Field &field, SKMath::vec3 origin, SKMath::quat orientation);
	explicit InputActionHandler(Spatial *parent, SKMath::vec3 origin, SKMath::quat orientation);

	std::vector<Action> inputActions;
	void update();

protected:
	bool pointerInputEvent(const std::string uuid, const PointerInput &, const Datamap &);
	bool handInputEvent(const std::string uuid, const HandInput &, const Datamap &);
};
