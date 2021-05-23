#pragma once

#include <flatbuffers/flexbuffers.h>

#include <stardustxr/fusion/types/model.hpp>
#include <stardustxr/fusion/types/spatial.hpp>
#include <stardustxr/fusion/types/fields/boxfield.hpp>
#include <stardustxr/fusion/types/input/inputhandler.hpp>

class Slider : public StardustXRFusion::Spatial {
public:
	explicit Slider(float length, float minValue, float maxValue);
	~Slider();

	float value = 0.0f;
	float minValue;
	float maxValue;

	void setSliderValue(float value);

protected:
	float length;

	float orbPos = 0.0f;

	void setSliderPos(float pos);

	StardustXRFusion::Model base;
	StardustXRFusion::Model base_inv;
	StardustXRFusion::Model orb;
	StardustXRFusion::BoxField field;
	StardustXRFusion::InputHandler handler;

	bool inputEvent(const StardustXR::InputData *inputData);
};
