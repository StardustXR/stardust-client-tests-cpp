#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include <flatbuffers/flexbuffers.h>
#include "../include/math_util.hpp"

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/fusion/types/model.hpp>
#include <stardustxr/fusion/types/fields/boxfield.hpp>
#include <stardustxr/fusion/types/input/inputhandler.hpp>
#include <stardustxr/fusion/types/input/types/handinput.hpp>

using namespace StardustXRFusion;
using namespace SKMath;
using namespace std;

const uint patternLockSize = 3;
const float orbSpacing = 0.075f;
const float orbOffset = (patternLockSize - 1.0f) * 0.5f;

struct Orb {
	Model *model;
	Model *activatedModel;
	vec3 position;
};

struct OrbLink {
	Orb *startOrb;
	Orb *endOrb;
	Model *model;
};

Orb orbs[patternLockSize][patternLockSize][patternLockSize];

int main(int, char *[]) {
	StardustXRFusion::Setup();
	Spatial root = Spatial::create(vec3_forward*0.5f, quat_identity, vec3_one, true, true, false);

	for(uint x=0; x<patternLockSize; ++x) {
		for(uint y=0; y<patternLockSize; ++y) {
			for(uint z=0; z<patternLockSize; ++z) {
				orbs[x][y][z] = {};
				orbs[x][y][z].position = vec3{x-orbOffset, y-orbOffset, z-orbOffset} * orbSpacing;
				orbs[x][y][z].model = new Model("../res/patternlock/lock_orb.glb", orbs[x][y][z].position);
				orbs[x][y][z].model->setSpatialParent(&root);
				orbs[x][y][z].activatedModel = nullptr;
			}
		}
	}
	vector<OrbLink> orbLinks;

    BoxField field(vec3_zero, quat_identity, vec3_one * ((patternLockSize - 1) * orbSpacing));
	field.setSpatialParent(&root);
    InputHandler handler(&root, field, -vec3_one * orbOffset * orbSpacing, quat_identity);
    handler.setSpatialParent(&root);
	handler.handHandlerMethod = [&root](const StardustXRFusion::HandInput &hand, const StardustXRFusion::Datamap &datamap) {
		const SKMath::vec3 pinchPos = (hand.thumb().tip().pose.position + hand.index().tip().pose.position) * 0.5f;
		const float pinchStrength = datamap.getFloat("pinchStrength");

		if(pinchStrength > 0.9f && hand.distance < orbSpacing / 2) {
			vec3 coords = (pinchPos / orbSpacing) + (vec3_one / 2);
			// coords.x = clamp(coords.x, -0.5f, patternLockSize-0.5f);
			// coords.y = clamp(coords.y, -0.5f, patternLockSize-0.5f);
			// coords.z = clamp(coords.z, -0.5f, patternLockSize-0.5f);
			Orb *orb = &orbs[(uint) coords.x][(uint) coords.y][(uint) coords.z];
			if(orb->activatedModel == nullptr) {
				orb->activatedModel = new Model("../res/patternlock/lock_orb_active.glb", orb->position);
				orb->activatedModel->setSpatialParent(&root);
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

// 				const vec3 indexTipPos = {
// 					hand->finger_joints()->Get(8)->position().x(),
// 					-hand->finger_joints()->Get(8)->position().y(),
// 					-hand->finger_joints()->Get(8)->position().z()
// 				};
// 				const vec3 thumbTipPos = {
// 					hand->finger_joints()->Get(2)->position().x(),
// 					-hand->finger_joints()->Get(2)->position().y(),
// 					-hand->finger_joints()->Get(2)->position().z()
// 				};
// 				const vec3 pinchPos = (indexTipPos + thumbTipPos) * 0.5f;

//                 if(pinchStrength > 0.9f && distance < orbSpacing / 2) {
//                     vec3 coords = (pinchPos / orbSpacing) + (vec3_one / 2);
// //					coords.x = clamp(coords.x, -0.5f, patternLockSize-0.5f);
// //					coords.y = clamp(coords.y, -0.5f, patternLockSize-0.5f);
// //					coords.z = clamp(coords.z, -0.5f, patternLockSize-0.5f);
//                     Orb *orb = &orbs[(uint) coords.x][(uint) coords.y][(uint) coords.z];
// 					if(orb->activatedModel == nullptr) {
// 						orb->activatedModel = new Model("../res/patternlock/lock_orb_active.glb", orb->position);
// 						orb->activatedModel->setSpatialParent(&root);
// 					}
// 				}
// 			} return true;
// 			default: return false;
// 		}
// 	});


	std::this_thread::sleep_for(std::chrono::seconds(3600));
}
