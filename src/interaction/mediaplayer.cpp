#include "mediaplayer.hpp"
#include "../dbus/player.hpp"
#include <stardustxr/fusion/types/fields/boxfield.hpp>

using namespace SKMath;

MediaPlayer::MediaPlayer(sdbus::IConnection &connection, std::string busName, SKMath::vec3 position, SKMath::quat rotation) :
mpProxy(sdbus::createProxy(connection, busName, "/org/mpris/MediaPlayer2")),
mediaPlayer(*mpProxy),
boxField(vec3_zero, quat_identity, { 0.075f, 0.12f, 0.01f }),
root(position, rotation, boxField, 0.01f),
body("../res/mediaplayer/body.glb", vec3_zero, quat_identity, vec3_one),
scrub(0.067f, 0, 1, 0.002f, 0.01f, color_from_hsva(0, 1, 1, 1)) {

	scrub.maxDistance = 0.05f;
	scrub.setPose(pose_t{
		{-0.0335f, -0.0154f, 0.005f},
		quat_identity
	});

	boxField.setSpatialParent(&root.item);
	body.setSpatialParent(&root.item);
	scrub.setSpatialParent(&body);
}

void MediaPlayer::update(double delta) {
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
}
