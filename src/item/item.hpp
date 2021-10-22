#pragma once

#include "../interaction/grabbable.hpp"
#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/fields/field.hpp>

class Item : public Grabbable {
public:
	explicit Item(SKMath::vec3 origin, SKMath::quat orientation);
	virtual ~Item() {}

	void update();

	virtual void onAccepted(Spatial &acceptor);

	bool isGrabbing();
	bool grabbingChanged();

	void distance (Spatial *space, SKMath::vec3 point, std::function<void (float)> callback);
};
