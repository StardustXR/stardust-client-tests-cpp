#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/items/environment.hpp>
#include <vector>

using namespace StardustXRFusion;
using namespace SKMath;

std::vector<std::string> environments = {
	 "../res/environments/comfy_cafe.hdr",
	 "../res/environments/earth.hdr",
	"../res/environments/space.hdr",
	 "../res/environments/lakemichigan.jpg",
};
std::vector<EnvironmentItem> environmentItems;

int main(int argc, char *argv[]) {
	StardustXRFusion::Setup();

	for(std::string &path : environments) {
		environmentItems.emplace_back(nullptr, path, vec3_zero, quat_identity);
	}
	
//	StardustXRFusion::StallMainThread();
}
