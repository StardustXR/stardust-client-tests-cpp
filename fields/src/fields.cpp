#include <chrono>
#include <cmath>
#include <thread>

#include <stardustxr/client/connector.hpp>
#include <stardustxr/client/stardust_scenegraph.hpp>
#include <stardustxr/client/messenger.hpp>

class LifeCycleNode : public StardustXR::ClientNode {
public:
	explicit LifeCycleNode(StardustXR::ClientMessenger *messenger) {
		this->messenger = messenger;
		STARDUSTXR_NODE_METHOD("logicStep", &LifeCycleNode::logicStep)
	}

	std::vector<uint8_t> logicStep(flexbuffers::Reference data, bool returnValue) {
		double delta = data.AsVector()[0].AsDouble();
		time += delta;
		printf("Current time is %f with delta of %f\n", time, delta);

		double distance = messenger->executeRemoteMethod("/field/test", "distance", [&](flexbuffers::Builder &fbb) {
			fbb.TypedVector([&]() {
				fbb.Double(sin(time)*2.0);
				fbb.Double(0.0);
				fbb.Double(cos(time)*2.0);
			});
		}).AsDouble();

		printf("Distance is %f\n", distance);

		return std::vector<uint8_t>();
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
	scenegraph.addNode("/lifecycle", new LifeCycleNode(&messenger));

	messenger.sendSignal("/lifecycle", "subscribeLogicStep", [&](flexbuffers::Builder &fbb) {
		fbb.Vector([&]() {
			fbb.String("/lifecycle");
			fbb.String("logicStep");
		});
	});
	messenger.sendSignal("/field", "createBoxField", [&](flexbuffers::Builder &fbb) {
		fbb.Vector([&]() {
			fbb.String("test");
			fbb.TypedVector([&]() {
				fbb.Double(0.0f);
				fbb.Double(0.0f);
				fbb.Double(0.0f);
			});
			fbb.TypedVector([&]() {
				fbb.Double(0.0f);
				fbb.Double(0.0f);
				fbb.Double(0.0f);
				fbb.Double(1.0f);
			});
			fbb.TypedVector([&]() {
				fbb.Double(1.0f);
				fbb.Double(1.0f);
				fbb.Double(1.0f);
			});
		});
	});
	std::this_thread::sleep_for(std::chrono::seconds(60));

	return 0;
}
