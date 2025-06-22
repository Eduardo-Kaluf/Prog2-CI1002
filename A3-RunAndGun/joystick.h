#ifndef __JOYSTICK__ 																											
#define __JOYSTICK__																											

#include "player.h"

struct joystick {
	unsigned char right;
	unsigned char left;
	unsigned char up;
	unsigned char down;
	unsigned char fire;
};

struct joystick* joystick_create();

void joystick_right(struct joystick *element);

void joystick_left(struct joystick *element);

void joystick_up(struct joystick *element);

void joystick_down(struct joystick *element);

void joystick_fire(struct joystick *element);

void destroy_joystick(struct joystick *element);

void joystick_control(struct player *player, int key);

#endif
