#include "button.hpp"
#include <stardustxr/fusion/types/spatial/spatial.hpp>

using namespace StardustXRFusion;


Button::Button(Spatial *space, StardustXRFusion::Field &field, Vec3 origin, Quat orientation) :
StardustXRFusion::InputHandler(space, field, origin, orientation) {

	handHandlerMethod = std::bind(&Button::handInput, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	pointerHandlerMethod = std::bind(&Button::pointerInput, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

bool Button::handInput(const std::string uuid, const StardustXRFusion::HandInput &hand, const StardustXRFusion::Datamap &datamap) {
	// if(xInteract.)
	// xInteract.input(bool active)
	return false;
}

bool Button::pointerInput(const std::string uuid, const StardustXRFusion::PointerInput &pointer, const StardustXRFusion::Datamap &datamap) {
	return false;
}
