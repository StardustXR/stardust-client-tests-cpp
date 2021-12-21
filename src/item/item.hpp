#pragma once

#include "../interaction/grabbable.hpp"
#include <stardustxr/fusion/types/items/item.hpp>

class GrabbableItemUI : public Grabbable {
public:
	explicit GrabbableItemUI(StardustXRFusion::Item *item);

	bool isGrabbing();
	bool grabbingChanged();

protected:
	StardustXRFusion::Item *item;
};
