#pragma once

#include <atomic>

#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/input/inputhandler.hpp>
#include <stardustxr/fusion/types/fields/spherefield.hpp>

class ExclusivePinch {
public:
	explicit ExclusivePinch(SKMath::vec3 position);
	~ExclusivePinch();

	void update();

protected:
	std::atomic<bool> activity;
	std::atomic<bool> oldActivity;

	StardustXRFusion::SphereField field;
	StardustXRFusion::InputHandler inputHandler;
	bool inputEvent(const StardustXR::InputData *inputData);
};