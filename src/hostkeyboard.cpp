#include <unistd.h>

#include <xcb/xcb.h>
#include <xcb/xinput.h>
#include <xcb/xcb_atom.h>
#include <xkbcommon/xkbcommon.h>
#include <xkbcommon/xkbcommon-x11.h>
#define explicit __explicit
#include <xcb/xkb.h>
#undef explicit

#include <stardustxr/fusion/fusion.hpp>
#include <stardustxr/common/flex.hpp>
#include <stardustxr/fusion/types/data/sender.hpp>

using namespace StardustXRFusion;
using namespace SKMath;

#define XCB_EVENT_RESPONSE_TYPE_MASK 0x7f

std::string windowName = "Stardust XR Host Keyboard";
uint32_t eventTypes = XCB_EVENT_MASK_KEYMAP_STATE | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE;

std::vector<PulseReceiver> keyboardInputs;
double checkReceiversInterval = 0.5;

int main(int, char *[]) {
	StardustXRFusion::Setup();

	xcb_connection_t *xConnect = xcb_connect(nullptr, nullptr);
	const xcb_query_extension_reply_t *xInputExtension = xcb_get_extension_data(xConnect, &xcb_input_id);
	if (!xInputExtension || !xInputExtension->present) {
		xcb_disconnect(xConnect);
		return 1;
	}
	uint8_t xInputOpcode = xInputExtension->major_opcode;
	xcb_screen_t *xScreen = xcb_setup_roots_iterator (xcb_get_setup(xConnect)).data;
	struct {
		xcb_input_event_mask_t head;
		xcb_input_xi_event_mask_t mask;
	} xInputMask;
	xcb_window_t xWindow = xcb_generate_id(xConnect);
	xcb_generic_event_t *xEvent;
	uint8_t first_xkb_event;
	if(!xkb_x11_setup_xkb_extension(xConnect, XKB_X11_MIN_MAJOR_XKB_VERSION, XKB_X11_MIN_MINOR_XKB_VERSION, XKB_X11_SETUP_XKB_EXTENSION_NO_FLAGS, NULL, NULL, &first_xkb_event, NULL))
		return 1;
	int32_t xKeyboardID = xkb_x11_get_core_keyboard_device_id(xConnect);
	if(xKeyboardID == -1)
		return 1;
	xkb_context *xkbContext = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
	xkb_keymap *xkbKeymap = xkb_x11_keymap_new_from_device(xkbContext, xConnect, xKeyboardID, XKB_KEYMAP_COMPILE_NO_FLAGS);
	std::string xkbKeymapString = xkb_keymap_get_as_string(xkbKeymap, XKB_KEYMAP_FORMAT_TEXT_V1);

	xcb_create_window(xConnect, XCB_COPY_FROM_PARENT, xWindow, xScreen->root, 0, 0, 800, 600, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, xScreen->root_visual, XCB_CW_EVENT_MASK, &eventTypes);
	xInputMask.head = { .deviceid = XCB_INPUT_DEVICE_ALL_MASTER, .mask_len = 1 };
	xInputMask.mask = (xcb_input_xi_event_mask_t) (XCB_INPUT_XI_EVENT_MASK_KEY_PRESS | XCB_INPUT_XI_EVENT_MASK_KEY_RELEASE);
	xcb_input_xi_select_events(xConnect, xWindow, 1, &xInputMask.head);
	xcb_change_property(xConnect, XCB_PROP_MODE_REPLACE, xWindow, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, windowName.length(), windowName.c_str());
	xcb_map_window(xConnect, xWindow);
	xcb_flush(xConnect);

	double elapsedTime = 0;
	PulseSender keyboard(StardustXRFusion::Root());
	StardustXRFusion::OnLogicStep([&](double delta, double) {
		double oldCheckCount = floor(elapsedTime / checkReceiversInterval);
		double newCheckCount = floor((elapsedTime / checkReceiversInterval) + delta);
		if(oldCheckCount < newCheckCount) { //check if
			keyboard.getReceivers([&](std::vector<PulseReceiver> &receivers) {
				keyboardInputs.clear();
				for(PulseReceiver receiver : receivers) {
					receiver.getMask([receiver](flexbuffers::Map mask) {
						if(mask["type"].AsString().str() == "keyboard") {
							keyboardInputs.push_back(receiver);
						}
					});
				}
			});
		}
		elapsedTime += delta;
	});

	while ((xEvent = xcb_wait_for_event(xConnect))) {
		switch(xEvent->response_type & XCB_EVENT_RESPONSE_TYPE_MASK) {
			case XCB_GE_GENERIC: {
				xcb_ge_generic_event_t *xGenericEvent = (xcb_ge_generic_event_t *)xEvent;
				if(xGenericEvent->extension == xInputOpcode) {
					switch (xGenericEvent->event_type) {
						case XCB_INPUT_KEY_PRESS: {
							xcb_input_key_press_event_t *xInputEvent = (xcb_input_key_press_event_t *)xGenericEvent;

							if (xInputEvent->flags & XCB_INPUT_KEY_EVENT_FLAGS_KEY_REPEAT)
								break;

							std::vector<uint8_t> map = StardustXR::FlexbufferFromArguments([&](flexbuffers::Builder &fbb) {
								fbb.Map([&] {
									fbb.String("type", "keyboard");
									fbb.String("keymap", xkbKeymapString);
									fbb.Map("modifiers", [&] {
										fbb.UInt("depressed", xInputEvent->mods.base);
										fbb.UInt("latched",   xInputEvent->mods.latched);
										fbb.UInt("locked",    xInputEvent->mods.locked);
										fbb.UInt("group",     xInputEvent->mods.effective);
									});
									fbb.UInt("pressed", xInputEvent->detail - 8);
								});
							});

							for(PulseReceiver &receiver : keyboardInputs) {
								receiver.sendData(map);
							}
						} break;
						case XCB_INPUT_KEY_RELEASE: {
							xcb_input_key_release_event_t *xInputEvent = (xcb_input_key_release_event_t *)xGenericEvent;

							if (xInputEvent->flags & XCB_INPUT_KEY_EVENT_FLAGS_KEY_REPEAT)
								break;

							std::vector<uint8_t> map = StardustXR::FlexbufferFromArguments([&](flexbuffers::Builder &fbb) {
								fbb.Map([&] {
									fbb.String("type", "keyboard");
									fbb.String("keymap", xkbKeymapString);
									fbb.Map("modifiers", [&] {
										fbb.UInt("depressed", xInputEvent->mods.base);
										fbb.UInt("latched",   xInputEvent->mods.latched);
										fbb.UInt("locked",    xInputEvent->mods.locked);
										fbb.UInt("group",     xInputEvent->mods.effective);
									});
									fbb.UInt("released", xInputEvent->detail - 8);
								});
							});

							for(PulseReceiver &receiver : keyboardInputs) {
								receiver.sendData(map);
							}
						} break;
					}
				}
			} break;
		}
	}

	shutdown:
	xcb_disconnect(xConnect);
}
