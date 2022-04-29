#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/values/glm.hpp>
#include <stardustxr/fusion/types/items/types/environment.hpp>
#include <vector>

using namespace StardustXRFusion;


std::vector<std::string> environments = {
	 "res/environments/comfy_cafe.hdr",
	 "res/environments/earth.hdr",
	"res/environments/space.hdr",
	 "res/environments/lakemichigan.jpg",
};
std::vector<EnvironmentItem> environmentItems;

int main(int argc, char *argv[]) {
	StardustXRFusion::Setup();

	for(std::string &path : environments) {
		environmentItems.emplace_back(nullptr, path, Vec3::Zero, Quat::Identity);
	}
	
	StardustXRFusion::RunEventLoop();
}
