#include <chrono>
#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/IProxy.h>
#include <string>
#include <thread>
#include <vector>

#include "interaction/mediaplayer.hpp"

using namespace StardustXRFusion;
using namespace SKMath;

int main(int, char**) {
	StardustXRFusion::Setup();

	std::unique_ptr<sdbus::IConnection> sessionConnection = sdbus::createSessionBusConnection();

	std::vector<std::string> names;
	{
		std::unique_ptr<sdbus::IProxy> dbusProxy = sdbus::createProxy(*sessionConnection, "org.freedesktop.DBus", "/");
		auto listNamesMethod = dbusProxy->createMethodCall("org.freedesktop.DBus", "ListNames");
		auto namesReply = dbusProxy->callMethod(listNamesMethod);
		namesReply >> names;
	}

	std::vector<std::unique_ptr<MediaPlayer>> mediaPlayers;
	{
		uint playerCount = 0;
		for(std::string name : names) {
			if(name.rfind("org.mpris.MediaPlayer2.", 0) == 0) {
				printf("Creating media controller for connection %s\n", name.c_str());
				playerCount++;
				mediaPlayers.emplace_back(new MediaPlayer(*sessionConnection, name, -vec3_right * playerCount * 0.1f));
			}
		}
	}

	// MediaPlayer mp(*sessionConnection, "org.mpris.MediaPlayer2.Lollypop", -vec3_right * 0.1f);

	LifeCycle()->onLogicStep([&](double delta, double) {
		// mp.update(delta);
		for(std::unique_ptr<MediaPlayer> &player : mediaPlayers) {
			player->update(delta);
		}
	});

	std::this_thread::sleep_for(std::chrono::seconds(3600));
}
