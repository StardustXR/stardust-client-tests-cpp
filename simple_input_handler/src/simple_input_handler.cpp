#include <chrono>
#include <cmath>
#include <thread>

#include <stardustxr/client/connector.hpp>
#include <stardustxr/client/stardust_scenegraph.hpp>
#include <stardustxr/client/messenger.hpp>

class UberNode : public StardustXR::ClientNode {
public:
	explicit UberNode(StardustXR::ClientMessenger *messenger) {
		this->messenger = messenger;
		STARDUSTXR_NODE_METHOD("logicStep", &UberNode::logicStep)
		STARDUSTXR_NODE_METHOD("input", &UberNode::input)
	}

	std::vector<uint8_t> logicStep(flexbuffers::Reference data, bool returnValue) {
		double delta = data.AsVector()[0].AsDouble();
		time += delta;
		printf("\r\033[A\033[ACurrent time is %f with delta of %f\n", time, delta);

		// messenger->sendSignal("/test/mesh", "setPosition", [&](flexbuffers::Builder &fbb) {
		// 	fbb.TypedVector([&]() {
		// 		fbb.Double(std::sin(time));
		// 		fbb.Double(1.65f);
		// 		fbb.Double(std::cos(time)-2.0);
		// 	});
		// });
		messenger->sendSignal("/field/stardust_xr_icon", "setOrigin", [&](flexbuffers::Builder &fbb) {
			fbb.TypedVector([&]() {
				fbb.Double(sin(time)*2.0);
				fbb.Double(0.0);
				fbb.Double(cos(time));
			});
		});

		return std::vector<uint8_t>();
	}

	std::vector<uint8_t> input(flexbuffers::Reference data, bool returnValue) {
		flexbuffers::Map map = data.AsMap();
		flexbuffers::Map datamap = map["datamap"].AsMap();

		float select = datamap["select"].AsFloat();
		float distance = map["distance"].AsFloat();

		messenger->sendSignal("/test/mesh", "setPosition", [&](flexbuffers::Builder &fbb) {
			fbb.TypedVector([&]() {
				fbb.Double(std::sin(time));
				fbb.Double(1.65f+(1.0f/distance*select));
				fbb.Double(std::cos(time)-2.0);
			});
		});

		printf("Select is %f and distance is %f\n", select, distance);

		flexbuffers::Builder fbb;
		fbb.Bool(false);
		fbb.Finish();
		return fbb.GetBuffer();
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
	scenegraph.addNode("/uber", new UberNode(&messenger));

	messenger.sendSignal("/lifecycle", "subscribeLogicStep", [&](flexbuffers::Builder &fbb) {
		fbb.Vector([&]() {
			fbb.String("/uber"); // Callback node
			fbb.String("logicStep"); // Callback method
		});
	});
	messenger.sendSignal("/field", "createSphereField", [&](flexbuffers::Builder &fbb) {
		fbb.Vector([&]() {
			fbb.String("stardust_xr_icon"); // Name
			fbb.TypedVector([&]() { // Origin
				fbb.Double(0.0);
				fbb.Double(0.0);
				fbb.Double(0.0);
			});
			fbb.Double(0.35); // Radius
		});
	});
	messenger.sendSignal("/input", "registerInputHandler", [&](flexbuffers::Builder &fbb) {
		fbb.Vector([&]() {
			fbb.String("stardust_xr_icon"); // Input method name
			fbb.String("stardust_xr_icon"); // Field name
			fbb.String("/uber"); // Callback node
			fbb.String("input"); // Callback method
		});
	});
	std::this_thread::sleep_for(std::chrono::seconds(120));

	return 0;
}
