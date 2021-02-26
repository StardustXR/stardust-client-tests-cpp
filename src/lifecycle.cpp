#include <chrono>
#include <thread>

#include <stardustxr/client/connector.hpp>
#include <stardustxr/client/stardust_scenegraph.hpp>
#include <stardustxr/client/messenger.hpp>

class LifeCycleNode : public StardustXR::ClientNode {
public:
	explicit LifeCycleNode() {
		STARDUSTXR_NODE_METHOD("logicStep", &LifeCycleNode::logicStep)
	}

	std::vector<uint8_t> logicStep(flexbuffers::Reference data, bool returnValue) {
		double delta = data.AsVector()[0].AsDouble();
		printf("\rLogic step delta is %f", delta);
		return std::vector<uint8_t>();
	}
};

int main(int argc, char *argv[]) {
	printf("Client starting...\n");
	int readFD, writeFD;
	if (!StardustXR::ConnectClient("/tmp/stardust.sock", readFD, writeFD)) {
		perror("Client failed to connect to server");
		return 1;
	}

	StardustXR::ClientStardustScenegraph scenegraph;
	scenegraph.addNode("/lifecycle", new LifeCycleNode());
	StardustXR::ClientMessenger messenger(readFD, writeFD, &scenegraph);

	messenger.sendSignal("/lifecycle", "subscribeLogicStep", [](flexbuffers::Builder &fbb) {
		fbb.Vector([&]() {
			fbb.String("/lifecycle");
			fbb.String("logicStep");
		});
	});
	std::this_thread::sleep_for(std::chrono::seconds(60));

	return 0;
}
