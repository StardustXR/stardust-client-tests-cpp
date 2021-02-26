#include <stardustxr/client/connector.hpp>
#include <stardustxr/client/messenger.hpp>
#include <stardustxr/client/stardust_scenegraph.hpp>

#include <chrono>
#include <cmath>
#include <thread>
#include <string>
#include <iostream>
#include <unistd.h>

using namespace std;

extern char *optarg;
extern int optind, opterr, optopt;

char modelNodePath[128];
float scale = 1.0f;

class LifeCycleNode : public StardustXR::ClientNode {
public:
	explicit LifeCycleNode(StardustXR::ClientMessenger *messenger) {
		this->messenger = messenger;
		STARDUSTXR_NODE_METHOD("logicStep", &LifeCycleNode::logicStep)
	}

	vector<uint8_t> logicStep(flexbuffers::Reference data, bool returnValue) {
		double delta = data.AsVector()[0].AsDouble();
		time += delta;
		printf("\033[4;0f"); // Clear the whole screen
		printf("Current time is %f with delta of %f", time, delta);

		messenger->sendSignal(modelNodePath, "setPosition", [&](flexbuffers::Builder &fbb) {
			fbb.TypedVector([&]() {
				fbb.Float(sinf(time)*0.05f);
				fbb.Float(0.0f);
				fbb.Float(cosf(time)*0.05f);
			});
		});

		return vector<uint8_t>();
	}
private:
	double time = 0.0;
	StardustXR::ClientMessenger *messenger;
};

int main(int argc, char *argv[]) {
	printf("Client starting...\n");
	int readFD, writeFD;
	if (!StardustXR::ConnectClient("/tmp/stardust.sock", readFD, writeFD)) {
		perror("Client failed to connect to server");
		return 1;
	}

	StardustXR::ClientStardustScenegraph scenegraph;
	StardustXR::ClientMessenger messenger(readFD, writeFD, &scenegraph);
	messenger.startHandler();
	scenegraph.addNode("/lifecycle", new LifeCycleNode(&messenger));

	sprintf(modelNodePath, "/model/%s", argv[1]);

	messenger.sendSignal("/model", "createFromFile", [&](flexbuffers::Builder &fbb) {
		fbb.Vector([&]() {
			fbb.String(argv[1]);
			fbb.String(argv[2]);
			fbb.TypedVector([&]() {
				fbb.Float(0.0f);
				fbb.Float(0.0f);
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

	messenger.sendSignal("/lifecycle", "subscribeLogicStep", [&](flexbuffers::Builder &fbb) {
		fbb.Vector([&]() {
			fbb.String("/lifecycle");
			fbb.String("logicStep");
		});
	});
	std::this_thread::sleep_for(std::chrono::seconds(120));

	return 0;
}
