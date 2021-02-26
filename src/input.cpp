#include <stardustxr/client/connector.hpp>
#include <stardustxr/client/messenger.hpp>
#include <stardustxr/client/stardust_scenegraph.hpp>

#include "flatbuffers/common_generated.h"
#include "flatbuffers/PointerInput_generated.h"
#include "flatbuffers/Input_generated.h"

#include <chrono>
#include <cmath>
#include <thread>
#include <string>
#include <iostream>
#include <unistd.h>

using namespace std;
using namespace StardustXR;

class MainNode : public StardustXR::ClientNode {
public:
	explicit MainNode(StardustXR::ClientMessenger *messenger) {
		this->messenger = messenger;
		STARDUSTXR_NODE_METHOD("inputEvent", &MainNode::inputEvent)
	}

	vector<uint8_t> inputEvent(flexbuffers::Reference data, bool) {
//		double delta = data.AsVector()[0].AsDouble();
//		time += delta;
		printf("\033[4;0f"); // Clear the whole screen
//		printf("Current time is %f with delta of %f", time, delta);
		printf("Input event\n");

		const InputData *inputMethod = GetInputData(data.AsBlob().data());

		printf("Input data:\n");
		printf("distance:    %f\n", inputMethod->distance());
		switch (inputMethod->input_type()) {
			case InputDataRaw_Pointer: {
				const Pointer *pointer = inputMethod->input_as_Pointer();
				printf("Pointer:\n");
				const vec3 *origin = pointer->origin();
				printf("origin:      %f, %f, %f\n", origin->x(), origin->y(), origin->z());
				const vec3 *direction = pointer->direction();
				printf("direction:   %f, %f, %f\n", direction->x(), direction->y(), direction->z());
			} break;
			default:
				break;
		}
//		printf("datamap")

		return vector<uint8_t>();
	}
private:
	double time = 0.0;
	StardustXR::ClientMessenger *messenger;
};

int main(int, char *argv[]) {
	printf("Client starting...\n");
	int readFD, writeFD;
	if (!StardustXR::ConnectClient("/tmp/stardust.sock", readFD, writeFD)) {
		perror("Client failed to connect to server");
		return 1;
	}

	StardustXR::ClientStardustScenegraph scenegraph;
	StardustXR::ClientMessenger messenger(readFD, writeFD, &scenegraph);
	messenger.startHandler();
	scenegraph.addNode("/main", new MainNode(&messenger));

	messenger.sendSignal("/model", "createFromFile", [&](flexbuffers::Builder &fbb) {
		fbb.Vector([&]() {
			fbb.String("iconTest");
			fbb.String(argv[1]);
			fbb.TypedVector([&]() {
				fbb.Float(0.0f);
				fbb.Float(-0.04f);
				fbb.Float(0.0f);
			});
			fbb.TypedVector([&]() {
				fbb.Float(0.0f);
				fbb.Float(0.0f);
				fbb.Float(0.0f);
				fbb.Float(1.0f);
			});
			fbb.TypedVector([&]() {
				fbb.Float(1.0f);
				fbb.Float(1.0f);
				fbb.Float(1.0f);
			});
		});
	});

	messenger.sendSignal("/field", "createBoxField", [&](flexbuffers::Builder &fbb) {
		fbb.Vector([&]() {
			fbb.String("iconTestField");
			fbb.TypedVector([&]() {
				fbb.Double(0.0);
				fbb.Double(0.0);
				fbb.Double(0.0);
			});
			fbb.TypedVector([&]() {
				fbb.Double(0.0);
				fbb.Double(0.0);
				fbb.Double(0.0);
				fbb.Double(1.0);
			});
			fbb.TypedVector([&]() {
				fbb.Double(0.08);
				fbb.Double(0.08);
				fbb.Double(0.08);
			});
		});
	});

	messenger.sendSignal("/input", "registerInputHandler", [&](flexbuffers::Builder &fbb) {
		fbb.Vector([&]() {
			fbb.String("iconTestHandler");
			fbb.String("/field/iconTestField");
			fbb.String("/main");
			fbb.String("inputEvent");
		});
	});

	std::this_thread::sleep_for(std::chrono::seconds(3600));

	return 0;
}
