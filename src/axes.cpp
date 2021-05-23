#include <chrono>
#include <iostream>
#include <thread>

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/model.hpp>

using namespace StardustXRFusion;

int main(int, char *[]) {
	StardustXRFusion::Setup();

	Model objectModel("../res/axes.glb");

	std::this_thread::sleep_for(std::chrono::seconds(3600));
}
