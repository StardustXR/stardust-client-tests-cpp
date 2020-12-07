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
		printf("\033[4;0f"); // Clear the whole screen
		printf("Current time is %f with delta of %f\n", time, delta);

		messenger->executeRemoteMethod("/field/box", "distance", [&](flexbuffers::Builder &fbb) {
			fbb.TypedVector([&]() {
				fbb.Double(sin(time)*2.0);
				fbb.Double(0.0);
				fbb.Double(cos(time)*2.0);
			});
		}, [](flexbuffers::Reference data) {
			printf("Distance to stationary box with moving point is %f\n", data.AsDouble());
		});

		messenger->sendSignal("/field/sphere", "setOrigin", [&](flexbuffers::Builder &fbb) {
			fbb.TypedVector([&]() {
				fbb.Double(sin(time)*2.0);
				fbb.Double(0.0);
				fbb.Double(cos(time));
			});
		});
		messenger->executeRemoteMethod("/field/sphere", "distance", [](flexbuffers::Builder &fbb) {
			fbb.TypedVector([&]() {
				fbb.Double(0.0);
				fbb.Double(0.0);
				fbb.Double(0.0);
			});
		}, [](flexbuffers::Reference data) {
			printf("Distance to moving sphere with stationary point is %f\n", data.AsDouble());
		});

		return std::vector<uint8_t>();
	}
private:
	double time = 0.0;
	StardustXR::ClientMessenger *messenger;
};

int main(int argc, char *argv[]) {
	printf("\033[2J\033[0;0f");
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

	messenger.sendSignal("/lifecycle", "subscribeLogicStep", [&](flexbuffers::Builder &fbb) {
		fbb.Vector([&]() {
			fbb.String("/lifecycle");
			fbb.String("logicStep");
		});
	});
	messenger.sendSignal("/field", "createBoxField", [&](flexbuffers::Builder &fbb) {
		fbb.Vector([&]() {
			fbb.String("box");
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
				fbb.Double(0.5);
				fbb.Double(0.5);
				fbb.Double(0.5);
			});
		});
	});
	messenger.sendSignal("/field", "createSphereField", [&](flexbuffers::Builder &fbb) {
		fbb.Vector([&]() {
			fbb.String("sphere");
			fbb.TypedVector([&]() {
				fbb.Double(0.0);
				fbb.Double(0.0);
				fbb.Double(0.0);
			});
			fbb.Double(0.5);
		});
	});
	printf("\n\n\n");

	std::this_thread::sleep_for(std::chrono::seconds(120));

	return 0;
}
