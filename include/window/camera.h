/**
 * @file camera.h
 * @author DargoDargonyx
 * @date 08/16/2026
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "util/helper.h"


typedef struct {
	IntPos pixel_pos;
	IntSize pixel_size;
} Camera;

// Play camera
int init_play_camera(void);
void destroy_play_camera(void);
Camera* get_play_camera(void);


#endif // CAMERA_H
