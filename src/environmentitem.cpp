#include <chrono>
#include <iostream>
#include <thread>

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/model.hpp>
#include <stardustxr/fusion/types/fields/spherefield.hpp>
#include "interaction/grabbable.hpp"

using namespace StardustXRFusion;
using namespace SKMath;

int main(int, char *[]) {
	StardustXRFusion::Setup();

	float size = 0.07f;
	SphereField sphereField(vec3_zero, size / 2);
	Model skyball("../res/item/skyball.glb", vec3_zero, quat_identity, vec3_one * size);
	Grabbable root(-vec3_right * 0.1f, quat_identity, sphereField);
	sphereField.setSpatialParent(&root);
	skyball.setSpatialParent(&root);

	LifeCycle()->onLogicStep([&](double, double) {
		root.update();
	});

	if(skyball)
		skyball.setMaterialProperty(0, "diffuse", "../res/environments/comfy_cafe.hdr");
	else
		return 0;
	std::this_thread::sleep_for(std::chrono::seconds(3600));
}
