#pragma once

#include "../dbus/player.hpp"
#include "grabbable.hpp"
#include "slider.hpp"
#include <stardustxr/fusion/types/spatial/spatial.hpp>
#include <string>

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/sk_math.hpp>
#include <stardustxr/fusion/types/drawable/model.hpp>
#include <stardustxr/fusion/types/fields/boxfield.hpp>

class MediaPlayer {
public:
	explicit MediaPlayer(sdbus::IConnection &connection, std::string busName, SKMath::vec3 position = SKMath::vec3_zero, SKMath::quat rotation = SKMath::quat_identity);

	void update(double delta);

protected:
	std::unique_ptr<sdbus::IProxy> mpProxy;
	Player mediaPlayer;

	StardustXRFusion::BoxField boxField;
	Grabbable root;
	StardustXRFusion::Model body;
	Slider scrub;
};
