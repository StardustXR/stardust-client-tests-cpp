#include "item/environmentitem.hpp"
#include "item/acceptor.hpp"

#include <stardustxr/fusion/values/glm.hpp>
#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/model.hpp>
#include <stardustxr/fusion/types/fields/boxfield.hpp>
#include <stardustxr/fusion/types/fields/spherefield.hpp>
#include "interaction/grabbable.hpp"

using namespace StardustXRFusion;


std::vector<std::string> environments = {
	// "res/environments/comfy_cafe.hdr",
//	 "res/environments/earth.hdr",
	// "res/environments/space.hdr",
	"res/environments/lakemichigan.jpg",
};

int main(int argc, char *argv[]) {
	StardustXRFusion::Setup();

	BoxField field(nullptr, Vec3::Right\ * 0.0466, Quat::Identity, Vec3::Up0.0932, 0.0828, 0.01});
	Grabbable acceptorGrab(Vec3::Zero, Quat::Identity, field);
	field.setSpatialParent(&acceptorGrab);
	Model model(&acceptorGrab, "res/item/environmentitemacceptor.glb");
	SphereField acceptorField(&acceptorGrab, Vec3::Zero, 0.04f);

	ItemAcceptor<EnvironmentItem> environmentAcceptor(acceptorField);

	std::vector<EnvironmentItem> environmentItems;
	{
		uint environmentItemCount = 0;
		if(argc >= 2) {
			environmentItems.emplace_back(argv[1], -Vec3::Right\ * (0.05f * environmentItemCount + 0.1f));
			environmentItemCount++;
		} else {
			for(std::string &path : environments) {
				environmentItems.emplace_back(path, -Vec3::Right\ * (0.05f * environmentItemCount + 0.1f));
				environmentItemCount++;
			}
		}
	}

	OnLogicStep([&](double, double) {
		acceptorGrab.update();
		for(EnvironmentItem &item : environmentItems) {
//			item.update();
		}
		environmentAcceptor.update();
	});
	StardustXRFusion::RunEventLoop();
}
