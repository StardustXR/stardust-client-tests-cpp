#pragma once

#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/input/inputhandler.hpp>
#include <stardustxr/fusion/types/fields/spherefield.hpp>
#include <stardustxr/fusion/types/spatial/spatial.hpp>

class XInteract {
public:
	explicit XInteract();

	void update();
	void input(bool active);

	bool isActive();
	bool hasActiveChanged();

	bool isInteractable();
	bool hasInteractableChanged();

protected:
	bool active = false;
	bool oldActive = false;
	bool activeFirstFrame = false;
	bool activeChanged = false;

	bool interactable = false;
	bool oldInteractable = false;
	bool interactableChanged = false;

	bool inputActivity = false;
	bool oldInputActivity = false;
};
