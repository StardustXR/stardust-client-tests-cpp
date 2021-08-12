#include "exclusivepinch.hpp"

using namespace StardustXRFusion;
using namespace SKMath;

ExclusivePinch::ExclusivePinch(SKMath::vec3 position) :
	Spatial(Spatial::create(position)),
	field(vec3_zero, 0.5f),
	inputHandler(nullptr, field, vec3_zero, quat_identity, std::bind(&ExclusivePinch::inputEvent, this, std::placeholders::_1)) {
	
	

}