#include <chrono>
#include <iostream>
#include <sdbus-c++/ConvenienceApiClasses.h>
#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/IProxy.h>
#include <thread>

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/model.hpp>
#include <stardustxr/fusion/types/fields/boxfield.hpp>

#include "dbus/player.hpp"
#include "interaction/grabbable.hpp"
#include "interaction/slider.hpp"

using namespace StardustXRFusion;
using namespace SKMath;

int main(int, char *[]) {
	StardustXRFusion::Setup();

	std::unique_ptr<sdbus::IConnection> sessionConnection = sdbus::createSessionBusConnection();
	std::unique_ptr<sdbus::IProxy> mpProxy = sdbus::createProxy(*sessionConnection, "org.mpris.MediaPlayer2.playerctld", "/org/mpris/MediaPlayer2");
	Player mediaPlayer(*mpProxy);

	BoxField boxField(vec3_zero, quat_identity, { 0.075f, 0.12f, 0.01f });
	Grabbable root(-vec3_right * 0.1f, quat_identity, boxField, 0.01f);
	boxField.setSpatialParent(&root.item);
	Model body("../res/mediaplayer/body.glb", vec3_zero, quat_identity, vec3_one);
	Slider scrub(0.067f, 0, 1, 0.002f, 0.01f, color_from_hsva(0, 1, 1, 1));
	scrub.maxDistance = 0.05f;
	scrub.setPose(pose_t{
		{-0.0335f, -0.0154f, 0.005f},
		quat_identity
	});

	body.setSpatialParent(&root.item);
	scrub.setSpatialParent(&body);

	LifeCycle()->onLogicStep([&](double delta, double) {
		root.update();
		mediaPlayer.update(delta);
		if(mediaPlayer.updating()) {
			if(mediaPlayer.hasTime())
				scrub.maxValue = mediaPlayer.songLength;
			if(mediaPlayer.artChanged()) {
				body.setMaterialProperty(1, "emission", (mediaPlayer.art == "") ? "../res/mediaplayer/notrack.png" : mediaPlayer.art);
			}
		}
		if(mediaPlayer.hasTime() && !scrub.isActive()) {
			if(scrub.activeChanged())
				mediaPlayer.setSongPosition(scrub.value);
			else
				scrub.setSliderValue(mediaPlayer.songPosition);
		}
		scrub.update();
	});

	std::this_thread::sleep_for(std::chrono::seconds(3600));
}
