#include "item/environmentitem.hpp"
#include "item/acceptor.hpp"

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/model.hpp>
#include <stardustxr/fusion/types/fields/spherefield.hpp>
#include "interaction/grabbable.hpp"

using namespace StardustXRFusion;
using namespace SKMath;

int main(int, char *[]) {
	StardustXRFusion::Setup();

	SphereField acceptorField(nullptr, vec3_zero, 0.1f);
	ItemAcceptor<EnvironmentItem> environmentAcceptor(acceptorField);

	EnvironmentItem environment("../res/environments/comfy_cafe.hdr", -vec3_right * 0.1f);

	OnLogicStep([&](double, double) {
		environment.update();
		environmentAcceptor.update();
	});
	StardustXRFusion::StallMainThread();
}
