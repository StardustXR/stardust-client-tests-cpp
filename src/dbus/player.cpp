#include "player.hpp"
#include <cmath>
#include <sdbus-c++/TypeTraits.h>

using namespace sdbus;

Player::Player(sdbus::IProxy &mpProxy) :
	org::mpris::MediaPlayer2::Player_proxy(mpProxy) {
}

Player::~Player() {}

void Player::onSeeked(const long &time) {
	songPosition = 0.000001f * time;
}

void Player::update(double delta) {
	time += delta;
	if(PlaybackStatus() == "Playing")
		songPosition += delta * songRate;
	if((time / dataInterval) > lastInterval) {
		lastInterval = floor(time / dataInterval);
		updating_ = true;
		updateData();
	} else 
		updating_ = false;
}

bool Player::updating() {
	return updating_;
}

bool Player::artChanged() {
	return updating_;
}

bool Player::hasTime() {
	return hasTime_;
}

void Player::setSongPosition(double songPosition) {
	if(hasTime_ && CanSeek() && trackID != "")
		SetPosition(trackID, 1000000 * songPosition);
}

void Player::updateData() {
	metadata = Metadata();

	Variant lengthVar = metadata["mpris:length"];
	hasTime_ = !lengthVar.isEmpty();
	if(!lengthVar.isEmpty()) {
		int64_t length = lengthVar.get<int64_t>();
		songLength = 0.000001f * length;
		songRate = Rate();
		onSeeked(Position());
	}


	Variant artVar = metadata["mpris:artUrl"];
	if(!artVar.isEmpty()) {
		std::string art = artVar.get<std::string>();
		std::string formattedArt = art;
		if(art.find("file://") != art.npos) 
			formattedArt = art.substr(7);
		
		artChanged_ = this->art != formattedArt;
		if(artChanged_)
			this->art = formattedArt;
	}

	Variant trackIDVar = metadata["mpris:trackid"];
	if(!trackIDVar.isEmpty()) {
		ObjectPath trackID = trackIDVar.get<ObjectPath>();
		if(trackID != this->trackID) {
			this->trackID = trackID;
		}
	}
}
