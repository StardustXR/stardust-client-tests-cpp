#include <stardustxr/fusion/values/glm.hpp>
#include "include/math_util.hpp"
#include "slider.hpp"

#include <algorithm>
#include <stardustxr/fusion/types/input/datamap.hpp>
#include <stardustxr/fusion/types/input/types/handinput.hpp>
#include <stardustxr/fusion/types/input/types/pointerinput.hpp>

using namespace StardustXRFusion;

Slider::Slider(Spatial *parent, float length, float minValue, float maxValue, float barThickness, float orbDiameter, Color color) :
	base(this, "res/slider/base.glb", Vec3::Zero, glm::quat(glm::vec3(0, 180, 0)), {0, barThickness, barThickness}),
	base_inv(this, "res/slider/base.glb", {length, 0, 0}, Quat::Identity, {length, barThickness, barThickness}),
	orb(this, "res/slider/orb.glb", Vec3::Zero, Quat::Identity, Vec3::One * orbDiameter),
	field(this, Vec3::Right * (length / 2), Quat::Identity, {length, barThickness, barThickness}),
	inputHandler(this, field, Vec3::Zero, Quat::Identity) {
	
	inputHandler.handHandlerMethod = std::bind(&Slider::handInput, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	inputHandler.pointerHandlerMethod = std::bind(&Slider::pointerInput, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

	base.setMaterialProperty(0, "color", color);

	this->minValue = minValue;
	this->maxValue = maxValue;
	this->length = length;
	this->barThickness = barThickness;
	this->orbDiameter = orbDiameter;
}

Slider::~Slider() {}

void Slider::update() {
	xInteract.update();
	xActive.update();
}

bool Slider::isActive() {
	return xActive.isActive();
}

bool Slider::activeChanged() {
	return xActive.hasActiveChanged();
}

void Slider::setSliderValue(float value) {
	float sliderPos = map(value, minValue, maxValue, 0, length);
	setSliderPos(sliderPos);
}

void Slider::setSliderPos(float pos) {
	orbPos = clamp(pos, 0, length);
	orb.setOrigin({orbPos, 0, 0});
	base.setScale({orbPos, barThickness, barThickness});
	base_inv.setScale({length - orbPos, barThickness, barThickness});

	value = map(orbPos, 0, length, minValue, maxValue);
}

void Slider::setSliderLength(float length) {
	this->length = length;
	
	field.setOrigin(Vec3::Right * (length / 2));
	field.setSize({length, barThickness, barThickness});

	base_inv.setOrigin({length, 0, 0});
	setSliderValue(value);
}

bool Slider::handInput(const std::string uuid, const StardustXRFusion::HandInput &hand, const StardustXRFusion::Datamap &datamap) {
	const Vec3 pinchPos = (hand.thumb().tip().pose.position + hand.index().tip().pose.position) * 0.5f;
	field.distance(&inputHandler, pinchPos, [&](float distance) { pinchDistance = distance; });
	if(!xInteract.isActive() && pinchDistance > maxDistance)
		return false;
	const float pinchStrength = datamap.getFloat("pinchStrength");
	xInteract.input(pinchStrength > 0.9f);
	xActive.input(xInteract.isActive());
	if(xInteract.isActive())
		setSliderPos(pinchPos.x);
	return xInteract.isActive();
}
bool Slider::pointerInput(const std::string uuid, const StardustXRFusion::PointerInput &pointer, const StardustXRFusion::Datamap &datamap) {
	if(!xInteract.isActive() && pointer.distance > maxDistance)
		return false;
	const float select = datamap.getFloat("select");
	const Vec2 scroll = datamap.getVec2("scroll");
	xInteract.input(select > 0.9f);
	xActive.input(xInteract.isActive() || scroll.y != 0);
	
	if(xInteract.isActive()) {
		setSliderPos(pointer.deepestPoint.x);
		return true;
	} else if(scroll.y != 0.0f) {
		this->scroll = scroll.y;
		setSliderValue(value+map(scroll.y, 0, 1000, 0, maxValue-minValue));
		return true;
	}
	return false;
}
