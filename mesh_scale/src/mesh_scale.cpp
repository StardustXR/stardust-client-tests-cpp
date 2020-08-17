#include <chrono>
#include <thread>

#include <stardustxr/client/clientconnector.hpp>
#include <stardustxr/messenger.hpp>
#include <stardustxr/dummy/blank_scenegraph.hpp>

int main(int argc, char *argv[]) {
	printf("Client starting...\n");
	int readFD, writeFD;
	if (!StardustXR::ConnectClient("/tmp/stardust.sock", readFD, writeFD)) {
		perror("Client failed to connect to server");
		return 1;
	}

	StardustXR::BlankScenegraph scenegraph;
	StardustXR::Messenger messenger(readFD, writeFD, &scenegraph);

	flexbuffers::Builder fbb;
	fbb.TypedVector([&]() {
		fbb.Double(2);
		fbb.Double(2);
		fbb.Double(2);
	});
	fbb.Finish();
	std::vector<uint8_t> data = fbb.GetBuffer();

	messenger.sendSignal("/test/mesh", "set_scale", data);

	std::this_thread::sleep_for(std::chrono::seconds(300));

	return 0;
}
