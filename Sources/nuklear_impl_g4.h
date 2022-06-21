#pragma once

#include <kinc/graphics4/graphics.h>
#include <kinc/graphics4/indexbuffer.h>
#include <kinc/graphics4/pipeline.h>
#include <kinc/graphics4/shader.h>
#include <kinc/graphics4/texture.h>
#include <kinc/graphics4/vertexbuffer.h>
#include <kinc/graphics4/vertexstructure.h>

typedef struct nk_kinc_device nk_kinc_device_t;



void nk_kinc_render(void);
void nk_kinc_device_create(void);
void nk_kinc_device_destroy(void);
void nk_kinc_font_stash_end(void);
#define NK_DEMO 1

#ifdef NK_DEMO
void nuklear_demo_ui(void);
#endif