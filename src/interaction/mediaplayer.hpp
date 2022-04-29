#pragma once

#include <stardustxr/fusion/types/drawable/model.hpp>
#include <stardustxr/fusion/types/fields/boxfield.hpp>
#include <stardustxr/fusion/types/spatial/spatial.hpp>

#include "../dbus/player.hpp"
#include "grabbable.hpp"
#include "slider.hpp"
#include <string>

class MediaPlayer {
public:
	explicit MediaPlayer(sdbus::IConnection &connection, std::string busName, StardustXRFusion::Vec3 position = StardustXRFusion::Vec3::Zero, StardustXRFusion::Quat rotation = StardustXRFusion::Quat::Identity);

	void update(double delta);

protected:
	std::unique_ptr<sdbus::IProxy> mpProxy;
	Player mediaPlayer;

	StardustXRFusion::BoxField boxField;
	Grabbable root;
	StardustXRFusion::Model body;
	Slider scrub;
};
