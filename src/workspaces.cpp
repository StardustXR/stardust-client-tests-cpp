#include <cmath>

#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/fields/cylinderfield.hpp>
#include <stardustxr/fusion/types/model.hpp>
#include <stardustxr/fusion/types/spatial.hpp>
#include <stardustxr/fusion/types/zone.hpp>

using namespace StardustXRFusion;
using namespace SKMath;

double elapsedTime = 0;

int main(int, char *[]) {
	StardustXRFusion::Setup();

	Spatial root = Spatial::create(nullptr);
	Model cell(&root, "../res/workspaces/cell.glb");
	Model cell2(&root, "../res/workspaces/cell.glb", -vec3_up);
	Model divider1(&root, "../res/workspaces/divider.glb");
	Model divider2(&root, "../res/workspaces/divider.glb", -vec3_up);
	CylinderField field(&root, vec3_zero, quat_from_angles(90, 0, 0), 1, 1);
	Zone zone(&root, field);

	zone.onSpatialEnter = [&](Spatial &spatial) {
		zone.capture(spatial);
	};

	OnLogicStep([&](double delta, double) {
		elapsedTime += delta;
		root.setOrigin({0, (float) std::sin(elapsedTime * 3.1415) * 0.1f, 0});
	});
	StardustXRFusion::StallMainThread();
}
