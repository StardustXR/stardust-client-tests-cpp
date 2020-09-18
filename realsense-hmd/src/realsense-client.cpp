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

	messenger.sendSignal("/hmd", "setPosition", [](flexbuffers::Builder &fbb) {
		fbb.TypedVector([&]() {
			fbb.Double(0);
			fbb.Double(0);
			fbb.Double(0);
		});
	});

	std::this_thread::sleep_for(std::chrono::seconds(300));

	return 0;
}
