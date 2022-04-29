#pragma once

#include <stardustxr/fusion/types/spatial/spatial.hpp>
#include <stardustxr/fusion/types/fields/field.hpp>
#include <stardustxr/fusion/types/input/inputactionhandler.hpp>
#include <stardustxr/fusion/types/input/actions/singleactor.hpp>

class Grabbable : public StardustXRFusion::Spatial {
public:
	Grabbable(Spatial root, const StardustXRFusion::Field field, float maxDistance = 0.005f);
	Grabbable(StardustXRFusion::Vec3 origin, StardustXRFusion::Quat orientation, const StardustXRFusion::Field field, float maxDistance = 0.005f);

	void update();

	std::function<void(void)> onStartedGrabbing = [](){};
	std::function<void(void)> onStoppedGrabbing = [](){};

	float maxDistance;

	StardustXRFusion::Field field;
	StardustXRFusion::InputActionHandler inputHandler;
	StardustXRFusion::InputActionHandler::Action inRangeAction;
	StardustXRFusion::SingleActorAction grabAction;
protected:
	bool pointerGrabbingCondition(const std::string uuid, const StardustXRFusion::PointerInput &pointer, const StardustXRFusion::Datamap &datamap);
	bool handGrabbingCondition(const std::string uuid, const StardustXRFusion::HandInput &hand, const StardustXRFusion::Datamap &datamap);

	StardustXRFusion::Spatial grabSpace;
	StardustXRFusion::Spatial scrollSpace;
	float scrollDistance = 0;
};
