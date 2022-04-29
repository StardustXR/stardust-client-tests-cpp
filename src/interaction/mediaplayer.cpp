#include "mediaplayer.hpp"
#include "../dbus/player.hpp"
#include <stardustxr/fusion/types/fields/boxfield.hpp>

using namespace StardustXRFusion;

MediaPlayer::MediaPlayer(sdbus::IConnection &connection, std::string busName, Vec3 position, Quat rotation) :
mpProxy(sdbus::createProxy(connection, busName, "/org/mpris/MediaPlayer2")),
mediaPlayer(*mpProxy),
boxField(nullptr, Vec3::Zero, Quat::Identity, Vec3(0.075f, 0.12f, 0.01f)),
root(position, rotation, boxField, 0.01f),
body(&root, "res/mediaplayer/body.glb", Vec3::Zero, Quat::Identity, Vec3::One),
scrub(&body, 0.067f, 0, 1, 0.002f, 0.01f, Color::FromHSVA(0, 1, 1, 1)) {

	scrub.maxDistance = 0.05f;
	scrub.setPose(Pose{
		{-0.0335f, -0.0154f, 0.005f},
		Quat::Identity
	});

	boxField.setSpatialParent(&root);
}

void MediaPlayer::update(double delta) {
	root.update();
	mediaPlayer.update(delta);
	if(mediaPlayer.updating()) {
		if(mediaPlayer.hasTime())
			scrub.maxValue = mediaPlayer.songLength;
		if(mediaPlayer.artChanged()) {
			body.setMaterialProperty(1, "emission", (mediaPlayer.art == "") ? "res/mediaplayer/notrack.png" : mediaPlayer.art);
		}
	}
	if(mediaPlayer.hasTime() && !scrub.isActive()) {
		if(scrub.activeChanged())
			mediaPlayer.setSongPosition(scrub.value);
		else
			scrub.setSliderValue(mediaPlayer.songPosition);
	}
	scrub.update();
}
