#pragma once

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/spatial/spatial.hpp>
#include <stardustxr/fusion/types/fields/field.hpp>

#include "inputactionhandler.hpp"
#include "xinteract.hpp"

#include <memory>

class Grabbable : public StardustXRFusion::Spatial {
public:
	explicit Grabbable(Spatial root, StardustXRFusion::Field &field, float maxDistance = 0.005f);
	explicit Grabbable(SKMath::vec3 origin, SKMath::quat orientation, StardustXRFusion::Field &field, float maxDistance = 0.005f);

	void update();
	void setField(StardustXRFusion::Field *field);

	std::function<void(void)> onStartedGrabbing = [](){};
	std::function<void(void)> onStoppedGrabbing = [](){};

	float maxDistance;

	InputActionHandler inputHandler;
	InputActionHandler::Action *inRangeAction;
	InputActionHandler::Action *grabAction;
protected:
	bool pointerGrabbingCondition(const std::string uuid, const PointerInput &pointer, const Datamap &datamap);
	bool handGrabbingCondition(const std::string uuid, const HandInput &hand, const Datamap &datamap);

	std::string grabbingInputUUID;
	std::map<std::string, float> grabbingInputDistances;
	Spatial grabSpace;
	Spatial scrollSpace;
	float scrollDistance = 0;

	StardustXRFusion::Field *field;
};
