#include <stardustxr/client/connector.hpp>
#include <stardustxr/client/messenger.hpp>
#include <stardustxr/client/stardust_scenegraph.hpp>

#include "debug.hpp"
#include "fusion.hpp"

#include <chrono>
#include <cmath>
#include <thread>
#include <string>
#include <iostream>
#include <unistd.h>

using namespace std;
using namespace SKMath;
using namespace StardustXR;
using namespace StardustFusion;

class MainNode : public StardustXR::ClientNode {
public:
	explicit MainNode(StardustXR::ClientMessenger *messenger) {
		this->messenger = messenger;
		STARDUSTXR_NODE_METHOD("inputEvent", &MainNode::inputEvent)
		STARDUSTXR_NODE_METHOD("logicStep", &MainNode::logicStep)
	}

	std::vector<uint8_t> logicStep(flexbuffers::Reference data, bool returnValue) {
		delta = data.AsVector()[0].AsDouble();
		time += delta;
		return std::vector<uint8_t>();
	}

	vector<uint8_t> inputEvent(flexbuffers::Reference data, bool) {
//		double delta = data.AsVector()[0].AsDouble();
		printf("\033[4;0f"); // Clear the whole screen
		printf("Time: %fsec\n", time);
		printf("Delta: %fms/%ifps\n", delta*1000, (int) std::floor(1.0/delta));

		const InputData *inputMethod = GetInputData(data.AsBlob().data());

		PrintInputData(inputMethod);

//		if(inputMethod->input_type() == InputDataRaw_Pointer && datamap["select"].AsFloat() > 0.0f) {
//			messenger->sendSignal("/model/iconTest", "setScale", [&](flexbuffers::Builder &fbb) {
//				fbb.TypedVector([&]() {
//					fbb.Float(newScale);
//					fbb.Float(newScale);
//					fbb.Float(newScale);
//				});
//			});
//		}

		return vector<uint8_t>();
	}
private:
	double delta = 0.0;
	double time = 0.0;
	float position[3];
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

	SKMath::vec3 pos = vec3_right;
	SKMath::quat rot = quat_identity;
	SKMath::vec3 scl = vec3_one*0.1f;

	string modelNodePath = CreateModelFromFile(messenger, "iconTest", argv[1], pos, rot, scl);
	string fieldNodePath = CreateBoxField(messenger, "iconTestField", pos, rot, scl);

	RegisterInputHandler(messenger, "iconTestHandler", fieldNodePath, pos, rot, "/main", "inputEvent");
	SubscribeLogicStep(messenger, "/main", "logicStep");

	std::this_thread::sleep_for(std::chrono::seconds(3600));

	return 0;
}
