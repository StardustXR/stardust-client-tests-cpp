#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "../include/math_util.hpp"

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/drawable/model.hpp>
#include <stardustxr/fusion/types/fields/boxfield.hpp>
#include <stardustxr/fusion/types/input/inputhandler.hpp>
#include <stardustxr/fusion/types/input/types/handinput.hpp>

using namespace StardustXRFusion;

using namespace std;

const uint patternLockSize = 3;
const float orbSpacing = 0.075f;
const float orbOffset = (patternLockSize - 1.0f) * 0.5f;

struct Orb {
	Model *model;
	Model *activatedModel;
	Vec3 position;
};

struct OrbLink {
	Orb *startOrb;
	Orb *endOrb;
	Model *model;
};

Orb orbs[patternLockSize][patternLockSize][patternLockSize];

int main(int, char *[]) {
	StardustXRFusion::Setup();
	Spatial root(Root(), Vec3::Forward*0.5f, Quat::Identity, Vec3::One, true, true, false);

	for(uint x=0; x<patternLockSize; ++x) {
		for(uint y=0; y<patternLockSize; ++y) {
			for(uint z=0; z<patternLockSize; ++z) {
				orbs[x][y][z] = {};
				orbs[x][y][z].position = Vec3{x-orbOffset, y-orbOffset, z-orbOffset} * orbSpacing;
				orbs[x][y][z].model = new Model(&root, "res/patternlock/lock_orb.glb", orbs[x][y][z].position);
				orbs[x][y][z].activatedModel = nullptr;
			}
		}
	}
	vector<OrbLink> orbLinks;

    BoxField field(&root, Vec3::Zero, Quat::Identity, Vec3::One * ((patternLockSize - 1) * orbSpacing));
    InputHandler handler(&root, field, -Vec3::One * orbOffset * orbSpacing, Quat::Identity);
	handler.handHandlerMethod = [&root](const std::string uuid, const StardustXRFusion::HandInput &hand, const StardustXRFusion::Datamap &datamap) {
		const Vec3 pinchPos = (hand.thumb().tip().pose.position + hand.index().tip().pose.position) * 0.5f;
		const float pinchStrength = datamap.getFloat("pinchStrength");

		if(pinchStrength > 0.9f && hand.distance < orbSpacing / 2) {
			Vec3 coords = (pinchPos / orbSpacing) + (Vec3::One / 2);
			// coords.x = clamp(coords.x, -0.5f, patternLockSize-0.5f);
			// coords.y = clamp(coords.y, -0.5f, patternLockSize-0.5f);
			// coords.z = clamp(coords.z, -0.5f, patternLockSize-0.5f);
			Orb *orb = &orbs[(uint) coords.x][(uint) coords.y][(uint) coords.z];
			if(orb->activatedModel == nullptr) {
				orb->activatedModel = new Model(&root, "res/patternlock/lock_orb_active.glb", orb->position);
			}
		}

		return false;
	};
// 	auto test = [&](const StardustXR::InputData *inputData) {
// 		flexbuffers::Map datamap = inputData->datamap_flexbuffer_root().AsMap();
//         float distance = inputData->distance();
// 		switch(inputData->input_type()) {
// 			case StardustXR::InputDataRaw_Hand: {
// 				const float pinchStrength = datamap["pinchStrength"].AsFloat();
// 				const StardustXR::Hand *hand = inputData->input_as_Hand();

// 				const Vec3 indexTipPos = {
// 					hand->finger_joints()->Get(8)->position().x(),
// 					-hand->finger_joints()->Get(8)->position().y(),
// 					-hand->finger_joints()->Get(8)->position().z()
// 				};
// 				const Vec3 thumbTipPos = {
// 					hand->finger_joints()->Get(2)->position().x(),
// 					-hand->finger_joints()->Get(2)->position().y(),
// 					-hand->finger_joints()->Get(2)->position().z()
// 				};
// 				const Vec3 pinchPos = (indexTipPos + thumbTipPos) * 0.5f;

//                 if(pinchStrength > 0.9f && distance < orbSpacing / 2) {
//                     Vec3 coords = (pinchPos / orbSpacing) + (Vec3::One / 2);
// //					coords.x = clamp(coords.x, -0.5f, patternLockSize-0.5f);
// //					coords.y = clamp(coords.y, -0.5f, patternLockSize-0.5f);
// //					coords.z = clamp(coords.z, -0.5f, patternLockSize-0.5f);
//                     Orb *orb = &orbs[(uint) coords.x][(uint) coords.y][(uint) coords.z];
// 					if(orb->activatedModel == nullptr) {
// 						orb->activatedModel = new Model("res/patternlock/lock_orb_active.glb", orb->position);
// 						orb->activatedModel->setSpatialParent(&root);
// 					}
// 				}
// 			} return true;
// 			default: return false;
// 		}
// 	});

	StardustXRFusion::RunEventLoop();
}
