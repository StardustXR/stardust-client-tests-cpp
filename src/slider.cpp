#include <chrono>
#include <iostream>
#include <thread>

#include "interaction/slider.hpp"

#include <stardustxr/fusion/fusion.hpp>

using namespace StardustXRFusion;
using namespace SKMath;

uint sliderCount = 50;
float oldSliderSpacing = 0.03f;
float sliderSpacing = 0.03f;
float oldSliderHeight = 0.03f;
float sliderHeight = 0.5f;

int main(int, char *[]) {
	StardustXRFusion::Setup();

	Spatial root = Spatial::create(vec3_forward * 0.5f, quat_identity);
	root.setOrientation(quat_from_angles(180, 180, 0));

	Slider *eqSliders[sliderCount];
	for(uint i=0; i<sliderCount; ++i) {
		eqSliders[i] = new Slider(sliderHeight, 0, 1);
		eqSliders[i]->setSpatialParent(&root);
		eqSliders[i]->setOrigin({i*sliderSpacing, sliderSpacing, 0});
		eqSliders[i]->setOrientation(quat_from_angles(90, 0, 0));
		eqSliders[i]->setSliderValue(0.5f);
	}

	Slider sliderSpacingSlider((sliderCount - 1)*sliderSpacing, 0.025f, 0.1f);
	sliderSpacingSlider.setSpatialParent(&root);
	sliderSpacingSlider.setSliderValue(sliderSpacing);

	Slider sliderHeightSlider((sliderCount - 1)*sliderSpacing, 0.1f, 1.0f);
	sliderHeightSlider.setSpatialParent(&root);
	sliderHeightSlider.setOrigin({0, -sliderSpacing, 0});
	sliderHeightSlider.setSliderValue(sliderHeight);

	LifeCycle()->onLogicStep([&](double, double) {
		sliderSpacing = sliderSpacingSlider.value;
		if(oldSliderSpacing != sliderSpacing) {
			for(uint i=0; i<sliderCount; ++i) {
				eqSliders[i]->setOrigin({i*sliderSpacing, sliderSpacing, 0});
			}

			sliderHeightSlider.setOrigin({0, -sliderSpacing, 0});
		}
		sliderHeight = sliderHeightSlider.value;
		if(oldSliderHeight != sliderHeight) {
			for(uint i=0; i<sliderCount; ++i) {
				eqSliders[i]->setSliderLength(sliderHeight);
			}
		}

		oldSliderSpacing = sliderSpacing;
		oldSliderHeight = sliderHeight;
	});

	std::this_thread::sleep_for(std::chrono::seconds(3600));
}
