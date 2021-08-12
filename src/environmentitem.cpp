#include <chrono>
#include <iostream>
#include <stardustxr/fusion/sk_math.hpp>
#include <thread>

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/model.hpp>

using namespace StardustXRFusion;
using namespace SKMath;

int main(int, char *[]) {
	StardustXRFusion::Setup();

	Model skyball("../res/item/skyball.glb", -vec3_right * 0.2f, quat_identity, vec3_one * 0.07f);
	if(skyball)
		skyball.setMaterialProperty(0, "diffuse", "../res/environments/comfy_cafe.hdr");
	std::this_thread::sleep_for(std::chrono::seconds(3600));
}
