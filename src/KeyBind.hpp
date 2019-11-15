#include <thread>
#include <functional>
#include <xcb/xcb.h>
#define XK_LATIN1
#include <xcb/xcb_keysyms.h>
#include <X11/keysymdef.h>


template<typename functionRtrT, typename ...functionArgsT>
class KeyBind {
	public:
	KeyBind() =delete;
	KeyBind(uint32_t keysym, xcb_mod_mask_t modifier, std::function<functionRtrT(functionArgsT...)> func) {
		dpy = xcb_connect(NULL, NULL);
		keysyms = xcb_key_symbols_alloc(dpy);
		keycodes = xcb_key_symbols_get_keycode(keysyms, keysym);
		screen = xcb_setup_roots_iterator(xcb_get_setup(dpy)).data;
		root = screen->root;
		mod = modifier;
		running = true;
		function = func;
		evt = nullptr;
		xcb_grab_key(dpy, true, root, modifier, keycodes[0], XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);
		xcb_flush(dpy);
	}

	void operator()(functionArgsT... arguments) {
		kbthread = std::thread([this, arguments...]{
			while(running && (evt = xcb_wait_for_event(dpy))) {
				switch(evt->response_type & ~0x80) {
					case XCB_KEY_RELEASE: {
						function(arguments...);
					}
				}
				if(evt) {
					std::free(evt);
				}
			}
		});
	}

	~KeyBind() {
		xcb_ungrab_key(dpy, keycodes[0], root, mod);
		if(keycodes) {
			std::cerr << std::hex << (uint32_t) keycodes[0] << std::dec;
			std::free(keycodes);
			std::cerr << "freed keycodes array\n";
		}
		if(keysyms) {
			xcb_key_symbols_free(keysyms);
			std::cerr << "freed keysyms\n";
		}
		running = false;
		if(kbthread.joinable()) kbthread.join();
		std::cerr << "joined child thread back to main thread\n";
		if(dpy) {
			xcb_disconnect(dpy);
			std::cerr << "disconnected dpy\n";
		}
	}

	private:
	xcb_connection_t* dpy;
	xcb_window_t root;
	xcb_key_symbols_t* keysyms;
	xcb_keycode_t* keycodes;
	xcb_mod_mask_t mod;
	xcb_screen_t* screen;
	xcb_generic_event_t* evt;
	std::thread kbthread;
	std::function<functionRtrT(functionArgsT...)> function;
	bool running;
};