#include <stardustxr/client/messenger.hpp>
#include <unistd.h>
#include "sk_math.hpp"

#pragma once

using namespace std;
using namespace SKMath;

namespace StardustFusion {

///////////////////////////////////////////
// Core Flexbuffer macros

#define FLEX_ARG(arg) \
	[&](flexbuffers::Builder &fbb) {\
		arg\
	}
#define FLEX_ARGS(args) \
	[&](flexbuffers::Builder &fbb) {\
		FLEX_VEC(args)\
	}
#define FLEX_SINGLE(arg) StardustXR::FlexbufferFromArguments(FLEX_ARG(arg))
#define FLEX(args) StardustXR::FlexbufferFromArguments(FLEX_ARGS(args))

///////////////////////////////////////////
// Core Flexbuffer type macros

#define FLEX_VEC(args) \
	fbb.Vector([&]() {\
		args\
	});
#define FLEX_STRING(str) \
	fbb.String(str.c_str());

///////////////////////////////////////////
// Special Flexbuffer type macros

#define FLEX_VEC3(vec) \
	fbb.TypedVector([&]() {\
		fbb.Float(vec.x);\
		fbb.Float(vec.y);\
		fbb.Float(vec.z);\
	});
#define FLEX_QUAT(quat) \
	fbb.TypedVector([&]() {\
		fbb.Float(quat.w);\
		fbb.Float(quat.x);\
		fbb.Float(quat.y);\
		fbb.Float(quat.z);\
	});

///////////////////////////////////////////
// Life Cycle functions

inline void SubscribeLogicStep(StardustXR::Messenger &messenger, string localNodePath, string localNodeMethod) {
	messenger.sendSignal(
		"/lifecycle",
		"subscribeLogicStep",
		FLEX_ARGS(
			FLEX_STRING(localNodePath)
			FLEX_STRING(localNodeMethod)
		)
	);
}

///////////////////////////////////////////
// Spatial functions

inline void setSpatialParent(StardustXR::Messenger &messenger, string spacePath, string spaceParentPath) {
	messenger.sendSignal(
		spacePath.c_str(),
		"registerInputHandler",
		FLEX_ARG(FLEX_STRING(spaceParentPath))
	);
}

///////////////////////////////////////////
// Input functions

inline void RegisterInputHandler(StardustXR::Messenger &messenger, string handlerName, string fieldNodePath, vec3 pos, quat rot, string localNodePath, string localNodeMethod) {
	messenger.sendSignal(
		"/input",
		"registerInputHandler",
		FLEX_ARGS(
			FLEX_STRING(handlerName)
			FLEX_STRING(fieldNodePath)
			FLEX_VEC3(pos)
			FLEX_QUAT(rot)
			FLEX_STRING(localNodePath)
			FLEX_STRING(localNodeMethod)
		)
	);
}

///////////////////////////////////////////
// Field functions

inline string CreateBoxField(StardustXR::Messenger &messenger, string name, vec3 pos, quat rot, vec3 size) {
	messenger.sendSignal(
		"/field",
		"createBoxField",
		FLEX_ARGS(
			FLEX_STRING(name)
			FLEX_VEC3(pos)
			FLEX_QUAT(rot)
			FLEX_VEC3(size)
		)
	);
	return "/field/"+name;
}

///////////////////////////////////////////
// Model functions
inline string CreateModelFromFile(StardustXR::Messenger &messenger, string modelName, string modelPath, vec3 pos, quat rot, vec3 scale) {
	messenger.sendSignal(
		"/model",
		"createFromFile",
		FLEX_ARGS(
			FLEX_STRING(modelName)
			FLEX_STRING(modelPath)
			FLEX_VEC3(pos)
			FLEX_QUAT(rot)
			FLEX_VEC3(scale)
		)
	);
	return "/model/"+modelName;
}

}
