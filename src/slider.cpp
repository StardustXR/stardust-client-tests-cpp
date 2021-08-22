#include <chrono>
#include <iostream>
#include <thread>

#include "interaction/slider.hpp"
#include "CLI11.hpp"

#include <stardustxr/fusion/fusion.hpp>

using namespace StardustXRFusion;
using namespace SKMath;

uint sliderCount = 50;
float sliderSpacing = 0.03f;
float sliderHeight = 0.5f;

int parseArgs(int argc, const char* const argv[]) {
	CLI::App app("Stardust XR Slider Demo");
	app.add_option<uint>("-n", sliderCount, "Number of sliders to create");
	try {
		(app).parse((argc), (argv));
	} catch(const CLI::ParseError &e) {
		return (app).exit(e);
	}
	return -1;
}

int main(int argc, const char* const argv[]) {
	int parse_result = parseArgs(argc, argv);
	if (parse_result != -1) return parse_result;

	StardustXRFusion::Setup();

	Spatial root = Spatial::create(vec3_forward * 0.5f, quat_identity);
	// root.setOrientation(quat_from_angles(180, 180, 0));

	Slider *eqSliders[sliderCount];
	for(uint i=0; i<sliderCount; ++i) {
		eqSliders[i] = new Slider(sliderHeight, 0, 1);
		eqSliders[i]->setSpatialParent(&root);
		eqSliders[i]->setOrigin({i*sliderSpacing, sliderSpacing, 0});
		eqSliders[i]->setOrientation(quat_from_angles(0, 0, 90));
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
		sliderHeightSlider.update();
		sliderSpacingSlider.update();
		for(uint i=0; i<sliderCount; ++i) {
			eqSliders[i]->update();
		}
		if(sliderSpacingSlider.isActive()) {
			const float sliderSpacing = sliderSpacingSlider.value;
			for(uint i=0; i<sliderCount; ++i) {
				eqSliders[i]->setOrigin({i*sliderSpacing, sliderSpacing, 0});
			}

			sliderHeightSlider.setOrigin({0, -sliderSpacing, 0});
		}
		if(sliderHeightSlider.isActive()) {
			for(uint i=0; i<sliderCount; ++i) {
				eqSliders[i]->setSliderLength(sliderHeightSlider.value);
			}
		}
	});

	std::this_thread::sleep_for(std::chrono::seconds(3600));
}
