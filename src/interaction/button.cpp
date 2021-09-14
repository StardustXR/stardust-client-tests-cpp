#include <stardustxr/fusion/types/spatial.hpp>
#include "button.hpp"

using namespace StardustXRFusion;
using namespace SKMath;

Button::Button(Spatial *space, StardustXRFusion::Field &field, SKMath::vec3 origin, SKMath::quat orientation) : 
StardustXRFusion::InputHandler(space, field, origin, orientation) {

	handHandlerMethod = std::bind(&Button::handInput, this, std::placeholders::_1, std::placeholders::_2);
	pointerHandlerMethod = std::bind(&Button::pointerInput, this, std::placeholders::_1, std::placeholders::_2);
}

bool Button::handInput(const StardustXRFusion::HandInput &hand, const StardustXRFusion::Datamap &datamap) {
	// if(xInteract.)
	// xInteract.input(bool active)
}

bool Button::pointerInput(const StardustXRFusion::PointerInput &pointer, const StardustXRFusion::Datamap &datamap) {
	
}
