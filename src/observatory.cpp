#include <stardustxr/fusion/fusion.hpp>

#include <stardustxr/fusion/types/model.hpp>

using namespace StardustXRFusion;

int main() {
	Setup();

	Model dome(nullptr, "../res/environments/DefaultEnvironment.glb");

	StallMainThread();
}
