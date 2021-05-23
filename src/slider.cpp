#include <chrono>
#include <iostream>
#include <thread>

#include "interaction/slider.hpp"

#include <stardustxr/fusion/fusion.hpp>

using namespace StardustXRFusion;
using namespace SKMath;

int main(int, char *[]) {
	StardustXRFusion::Setup();

	Slider slider(0.5, 0, 1);
	slider.setSliderValue(0.5f);

	std::this_thread::sleep_for(std::chrono::seconds(3600));
}
