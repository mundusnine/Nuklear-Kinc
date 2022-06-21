#pragma once

#include "nuklear_impl_g4.h"

void nk_kinc_init(void);
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

extern nk_kinc_t g_kinc;