#include <stardustxr/fusion/fusion.hpp>

#include <stardustxr/fusion/types/drawable/model.hpp>

using namespace StardustXRFusion;

int main() {
	Setup();

	Model dome(nullptr, "res/environments/DefaultEnvironment.glb");

	RunEventLoop();
}
