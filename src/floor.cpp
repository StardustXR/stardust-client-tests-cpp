#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/spatial/spatial.hpp>

#include "interaction/slider.hpp"

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/drawable/model.hpp>

using namespace StardustXRFusion;
using namespace SKMath;

#define SCALE 1000.0f
#define SLIDER_SPACING 0.02f
#define HEIGHT 1.65f

int main(int, char *[]) {
	StardustXRFusion::Setup();

	Spatial root(Root(), -vec3_up * HEIGHT);

	Model floor(&root, "../res/floor/floor.glb", vec3_zero, quat_identity, vec3_one * SCALE);
	floor.setMaterialProperty(0, "tex_scale", SCALE);

//	Slider r(&root, 0.1f, 0, 1, .002f, .015f, color{1, 0, 0, 1});
//	Slider g(&root, 0.1f, 0, 1, .002f, .015f, color{0, 1, 0, 1});
//	Slider b(&root, 0.1f, 0, 1, .002f, .015f, color{0, 0, 1, 1});

//	r.move(vec3_up * (HEIGHT+SLIDER_SPACING+SLIDER_SPACING));
//	g.move(vec3_up * (HEIGHT+SLIDER_SPACING));
//	b.move(vec3_up * (HEIGHT));

//	color oldColor = color{0.0f, 0.0f, 0.0f, 1.0f};

//	OnLogicStep([&](double, double) {
//		r.update();
//		g.update();
//		b.update();
//		color newColor = color{
//			r.value,
//			g.value,
//			b.value,
//			1.0f
//		};
//		if(newColor != oldColor)
//			floor.setMaterialProperty(0, "color", newColor);
//		oldColor = newColor;
//	});
	StardustXRFusion::StallMainThread();
}
