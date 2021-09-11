#pragma once

#include "../../include/mediaplayer.hpp"
#include <sdbus-c++/ConvenienceApiClasses.h>
#include <string>

class Player : protected org::mpris::MediaPlayer2::Player_proxy {
public:
	Player(sdbus::IProxy &mpProxy);
	virtual ~Player();

	void onSeeked(const long &time);
	void update(double delta);

	bool updating();
	bool artChanged();

	bool hasTime();

	void setSongPosition(double songPosition);

	double songPosition = 0.0;
	double songLength = 1.0;

	std::string art = "";

protected:
	std::map<std::string, sdbus::Variant> metadata;
	void updateData();
	bool updating_ = false;
	bool artChanged_ = false;
	bool hasTime_ = true;

	double songRate = 1.0;

	double dataInterval = 0.5;
	double time = 0;
	int32_t lastInterval = -1;

	sdbus::ObjectPath trackID;
};