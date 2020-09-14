#include <chrono>
#include <thread>

#include <stardustxr/client/connector.hpp>
#include <stardustxr/client/messenger.hpp>

int main(int argc, char *argv[]) {
	printf("Client starting...\n");
	int readFD, writeFD;
	if (!StardustXR::ConnectClient("/tmp/stardust.sock", readFD, writeFD)) {
		perror("Client failed to connect to server");
		return 1;
	}

	StardustXR::ClientScenegraph scenegraph;
	StardustXR::ClientMessenger messenger(readFD, writeFD, &scenegraph);

	flexbuffers::Builder fbb;
	fbb.TypedVector([&]() {
		fbb.Double(0.3535534f);
		fbb.Double(0.3535534f);
		fbb.Double(0.1464466f);
		fbb.Double(0.8535534f);
	});
	fbb.Finish();
	std::vector<uint8_t> data = fbb.GetBuffer();

	messenger.sendSignal("/test/mesh", "setRotation", data);
	std::this_thread::sleep_for(std::chrono::seconds(5));

	return 0;
}
