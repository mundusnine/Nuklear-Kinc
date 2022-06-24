#pragma once

#include "nuklear_impl_g4.h"

/**
 * @brief 
 * Initialize nk for use with Kinc.
 * @param default_inputs Give the value of > 0 to use only the internal functions for handling input.
 * Else we return the function pointer that you can use in your inputhandling system.
 * @return nk_kinputs_t An array of function pointers used for key_ down,up,pressed and mouse down,up,scroll 
 */
nk_kinputs_t nk_kinc_init(int default_inputs);
void nk_kinc_shutdown(void);
void nk_kinc_input(void);

#ifndef NK_KINC_TEXT_MAX
#define NK_KINC_TEXT_MAX 512
#endif

typedef struct nk_kinc nk_kinc_t;
struct nk_kinc {
	nk_kinc_device_t* dev;
	struct nk_context* ctx;
	struct nk_font_atlas* atlas;
	float width, height;
	unsigned int text[NK_KINC_TEXT_MAX];
	size_t text_len;
	uint8_t keyboard_key_state[255];
	double keyboard_key_time_pressed[255];
	uint8_t mouse_btn_state[16];
	int mouse_scroll;
};

typedef struct nk_kinputs {
	void (*kb_key_down_callback)(int /*key_code*/);
	void (*kb_key_up_callback)(int /*key_code*/);
	void (*kb_key_press_callback)(unsigned /*character*/);
	void (*mws_press_callback)(int /*window*/, int /*button*/, int /*x*/, int /*y*/);
	void (*mws_release_callback)(int /*window*/, int /*button*/, int /*x*/, int /*y*/);
	void (*mws_scroll_callback)(int /*window*/, int /*delta*/);
} nk_kinputs_t;

extern nk_kinc_t g_kinc;