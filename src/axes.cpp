#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/model.hpp>

using namespace StardustXRFusion;

int main(int, char *[]) {
	StardustXRFusion::Setup();
	Model objectModel("../res/axes.glb");
	StardustXRFusion::ShutdownAfterInput();
}
