#include <chrono>

#include <stardustxr/fusion/values/glm.hpp>
#include <stardustxr/fusion/fusion.hpp>
#include "interaction/slider.hpp"
#include "CLI11.hpp"


using namespace StardustXRFusion;


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

	Spatial root(Root(), Vec3::Forward * 0.5f, Quat::Identity);
	// root.setOrientation(quat_from_angles(180, 180, 0));

	Slider *eqSliders[sliderCount];
	for(uint i=0; i<sliderCount; ++i) {
		eqSliders[i] = new Slider(&root, sliderHeight, 0, 1);
		eqSliders[i]->setOrigin({i*sliderSpacing, sliderSpacing, 0});
		eqSliders[i]->setOrientation(glm::quat(glm::vec3(0, 0, 90)));
		eqSliders[i]->setSliderValue(0.5f);
	}

	Slider sliderSpacingSlider(&root, (sliderCount - 1)*sliderSpacing, 0.025f, 0.1f);
	sliderSpacingSlider.setSliderValue(sliderSpacing);

	Slider sliderHeightSlider(&root, (sliderCount - 1)*sliderSpacing, 0.1f, 1.0f);
	sliderHeightSlider.setOrigin({0, -sliderSpacing, 0});
	sliderHeightSlider.setSliderValue(sliderHeight);

	OnLogicStep([&](double, double) {
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
	StardustXRFusion::RunEventLoop();
}
