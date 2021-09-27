#pragma once

#include <flatbuffers/flexbuffers.h>

#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/model.hpp>
#include <stardustxr/fusion/types/spatial.hpp>
#include <stardustxr/fusion/types/fields/boxfield.hpp>
#include <stardustxr/fusion/types/input/datamap.hpp>
#include <stardustxr/fusion/types/input/inputhandler.hpp>

#include "xinteract.hpp"

class Slider : public StardustXRFusion::Spatial {
public:
	explicit Slider(Spatial *parent, float length, float minValue, float maxValue, float barThickness = 0.004f, float orbDiameter = 0.03f, SKMath::color color = SKMath::color{0,0,0.75f,1});
	~Slider();

	void update();
	bool isActive();
	bool activeChanged();

	float maxDistance = 0.0254f;

	float value = 0.0f;
	float minValue;
	float maxValue;

	void setSliderValue(float value);
	void setSliderLength(float length);

protected:
	float length;
	float barThickness;
	float orbDiameter;

	float orbPos = 0.0f;
	void setSliderPos(float pos);

	XInteract xInteract;
	XInteract xActive;
	float pinchDistance;
	float scroll;

	StardustXRFusion::Model base;
	StardustXRFusion::Model base_inv;
	StardustXRFusion::Model orb;
	StardustXRFusion::BoxField field;
	StardustXRFusion::InputHandler inputHandler;

	bool handInput(const StardustXRFusion::HandInput &hand, const StardustXRFusion::Datamap &datamap);
	bool pointerInput(const StardustXRFusion::PointerInput &pointer, const StardustXRFusion::Datamap &datamap);
};
