#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/drawable/model.hpp>

using namespace StardustXRFusion;

int main(int, char *[]) {
	StardustXRFusion::Setup();
	Model objectModel(nullptr, "res/axes.glb");
	StardustXRFusion::RunEventLoop();
}
