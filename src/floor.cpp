#include <stardustxr/fusion/values/glm.hpp>
#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/spatial/spatial.hpp>
#include <stardustxr/fusion/types/drawable/model.hpp>

//#include "interaction/slider.hpp"

using namespace StardustXRFusion;


#define SCALE 1000.0f
#define SLIDER_SPACING 0.02f
#define HEIGHT 1.65f

int main(int, char *[]) {
	StardustXRFusion::Setup();

	Spatial root(Root(), -Vec3::Up * HEIGHT);

	Model floor(&root, "res/floor/floor.glb", Vec3::Zero, Quat::Identity, Vec3::One * SCALE);
	floor.setMaterialProperty(0, "tex_scale", SCALE);

//	Slider r(&root, 0.1f, 0, 1, .002f, .015f, color{1, 0, 0, 1});
//	Slider g(&root, 0.1f, 0, 1, .002f, .015f, color{0, 1, 0, 1});
//	Slider b(&root, 0.1f, 0, 1, .002f, .015f, color{0, 0, 1, 1});

//	r.move(Vec3::Up * (HEIGHT+SLIDER_SPACING+SLIDER_SPACING));
//	g.move(Vec3::Up * (HEIGHT+SLIDER_SPACING));
//	b.move(Vec3::Up * (HEIGHT));

//	Color oldColor = color{0.0f, 0.0f, 0.0f, 1.0f};

//	OnLogicStep([&](double, double) {
//		r.update();
//		g.update();
//		b.update();
//		Color newColor = color{
//			r.value,
//			g.value,
//			b.value,
//			1.0f
//		};
//		if(newColor != oldColor)
//			floor.setMaterialProperty(0, "color", newColor);
//		oldColor = newColor;
//	});
	StardustXRFusion::RunEventLoop();
}
