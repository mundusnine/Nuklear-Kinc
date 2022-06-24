#include "nuklear_impl_kinc.h"

#include "nuklear.h"

#include <kinc/input/keyboard.h>
#include <kinc/input/mouse.h>
#include <kinc/system.h>

nk_kinc_t g_kinc = {0};

void key_down_callback(int key_code) {
	g_kinc.keyboard_key_state[key_code] = 1;
	g_kinc.keyboard_key_time_pressed[key_code] = kinc_time();
}
void key_up_callback(int key_code) {
	g_kinc.keyboard_key_time_pressed[key_code] = g_kinc.keyboard_key_state[key_code] = 0;
}

void mouse_btn_down(int windw, int btn, int x, int y) {
	g_kinc.mouse_btn_state[btn] = 1;
}
void mouse_btn_up(int windw, int btn, int x, int y) {
	g_kinc.mouse_btn_state[btn] = 0;
}

void mouse_scroll(int window, int delta) {
	g_kinc.mouse_scroll = delta;
}

int is_keyboard_pressed(int key_code) {
	return g_kinc.keyboard_key_state[key_code];
}
double keyboard_pressed_time(int key_code) {
	if (!g_kinc.keyboard_key_time_pressed[key_code])return 1;
	return kinc_time() - g_kinc.keyboard_key_time_pressed[key_code];
}
static void keyboard_key_press(unsigned character) {
	if (is_keyboard_pressed(KINC_KEY_CONTROL))return;
	g_kinc.text[g_kinc.text_len++] = character;
}
nk_kinputs_t nk_kinc_init(int default_inputs) {

	nk_kinc_device_create();
	nk_kinputs_t out = {
		.kb_key_down_callback=0,
		.kb_key_up_callback=0,
		.kb_key_press_callback=0,
		.mws_press_callback=0,
		.mws_release_callback=0,
		.mws_scroll_callback=0
	};
	if(default_inputs){
		/* input callbacks */
		kinc_keyboard_set_key_down_callback(key_down_callback);
		kinc_keyboard_set_key_up_callback(key_up_callback);
		kinc_keyboard_set_key_press_callback(keyboard_key_press);
		kinc_mouse_set_press_callback(mouse_btn_down);
		kinc_mouse_set_release_callback(mouse_btn_up);
		kinc_mouse_set_scroll_callback(mouse_scroll);
		return out;
	}
	else {
		out.kb_key_down_callback = key_down_callback;
		out.kb_key_up_callback = key_up_callback;
		out.kb_key_press_callback = keyboard_key_press;

		out.mws_press_callback = mouse_btn_down;
		out.mws_release_callback = mouse_btn_up;
		out.mws_scroll_callback = mouse_scroll;

		return out;
	}

}

void nk_kinc_shutdown(void) {
	nk_font_atlas_clear(g_kinc.atlas);
	nk_free(g_kinc.ctx);
	nk_kinc_device_destroy();
	memset(&g_kinc, 0, sizeof(g_kinc));
}
static int lastScroll = 0;
void nk_kinc_input(void) {
	struct nk_context* ctx = g_kinc.ctx;
	nk_input_begin(ctx);
	/* mouse */
	{
		int mouse_x, mouse_y;
		kinc_mouse_get_position(0, &mouse_x, &mouse_y);
		nk_input_motion(ctx, mouse_x, mouse_y);
		if (lastScroll == g_kinc.mouse_scroll) {
			g_kinc.mouse_scroll = 0;
		}
		nk_input_scroll(ctx, (struct nk_vec2) { 0.0f, (float)g_kinc.mouse_scroll });
		lastScroll = g_kinc.mouse_scroll;

		nk_input_button(ctx, NK_BUTTON_LEFT, mouse_x, mouse_y, g_kinc.mouse_btn_state[0]);
		nk_input_button(ctx, NK_BUTTON_RIGHT, mouse_x, mouse_y, g_kinc.mouse_btn_state[1]);
		nk_input_button(ctx, NK_BUTTON_MIDDLE, mouse_x, mouse_y, g_kinc.mouse_btn_state[2]);
	}

	/* keyboard */
	{
		nk_input_key(ctx, NK_KEY_DEL, is_keyboard_pressed(KINC_KEY_DELETE));
		nk_input_key(ctx, NK_KEY_ENTER, is_keyboard_pressed(KINC_KEY_RETURN));
		nk_input_key(ctx, NK_KEY_TAB, is_keyboard_pressed(KINC_KEY_TAB));
		int back_pressed = is_keyboard_pressed(KINC_KEY_BACKSPACE);
		if (back_pressed && keyboard_pressed_time(KINC_KEY_BACKSPACE) > 0.06) {
			nk_input_key(ctx, NK_KEY_BACKSPACE, 0);
			key_down_callback(KINC_KEY_BACKSPACE);
		}
		nk_input_key(ctx, NK_KEY_BACKSPACE, back_pressed);
		nk_input_key(ctx, NK_KEY_UP, is_keyboard_pressed(KINC_KEY_UP));
		nk_input_key(ctx, NK_KEY_DOWN, is_keyboard_pressed(KINC_KEY_DOWN));
		nk_input_key(ctx, NK_KEY_TEXT_START, is_keyboard_pressed(KINC_KEY_HOME));
		nk_input_key(ctx, NK_KEY_TEXT_END, is_keyboard_pressed(KINC_KEY_END));
		nk_input_key(ctx, NK_KEY_SCROLL_START, is_keyboard_pressed(KINC_KEY_HOME));
		nk_input_key(ctx, NK_KEY_SCROLL_END, is_keyboard_pressed(KINC_KEY_END));
		nk_input_key(ctx, NK_KEY_SCROLL_DOWN, is_keyboard_pressed(KINC_KEY_PAGE_DOWN));
		nk_input_key(ctx, NK_KEY_SCROLL_UP, is_keyboard_pressed(KINC_KEY_PAGE_UP));
		nk_input_key(ctx, NK_KEY_SHIFT, is_keyboard_pressed(KINC_KEY_SHIFT));

		if (is_keyboard_pressed(KINC_KEY_CONTROL)) {
			nk_input_key(ctx, NK_KEY_COPY, is_keyboard_pressed(KINC_KEY_C));
			nk_input_key(ctx, NK_KEY_PASTE, is_keyboard_pressed(KINC_KEY_V));
			nk_input_key(ctx, NK_KEY_CUT, is_keyboard_pressed(KINC_KEY_X));
			nk_input_key(ctx, NK_KEY_TEXT_UNDO, is_keyboard_pressed(KINC_KEY_Z));
			nk_input_key(ctx, NK_KEY_TEXT_REDO, is_keyboard_pressed(KINC_KEY_R));
			nk_input_key(ctx, NK_KEY_TEXT_WORD_LEFT, is_keyboard_pressed(KINC_KEY_LEFT));
			nk_input_key(ctx, NK_KEY_TEXT_WORD_RIGHT, is_keyboard_pressed(KINC_KEY_RIGHT));
			nk_input_key(ctx, NK_KEY_TEXT_LINE_START, is_keyboard_pressed(KINC_KEY_B));
			nk_input_key(ctx, NK_KEY_TEXT_LINE_END, is_keyboard_pressed(KINC_KEY_E));
			nk_input_key(ctx, NK_KEY_TEXT_SELECT_ALL, is_keyboard_pressed(KINC_KEY_A));
		}
		else {
			nk_input_key(ctx, NK_KEY_LEFT, is_keyboard_pressed(KINC_KEY_LEFT));
			nk_input_key(ctx, NK_KEY_RIGHT, is_keyboard_pressed(KINC_KEY_RIGHT));
			nk_input_key(ctx, NK_KEY_COPY, 0);
			nk_input_key(ctx, NK_KEY_PASTE, 0);
			nk_input_key(ctx, NK_KEY_CUT, 0);
			nk_input_key(ctx, NK_KEY_SHIFT, 0);
		}
		if (g_kinc.text_len > 0) {
			nk_input_glyph(ctx, g_kinc.text);
			for (int i = 0; i < g_kinc.text_len; ++i) {
				g_kinc.text[i] = '\0';
			}
			g_kinc.text_len = 0;
		}
	}
	nk_input_end(ctx);
}