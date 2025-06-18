#ifndef __JOYSTICK__ 																											
#define __JOYSTICK__																											

struct joystick {
	unsigned char right;
	unsigned char left;
	unsigned char up;
	unsigned char down;
	unsigned char fire;
};

struct joystick* joystick_create();																										
void joystick_destroy(struct joystick *element);																						
void joystick_right(struct joystick *element);																						
void joystick_left(struct joystick *element);																							
void joystick_up(struct joystick *element);																						
void joystick_down(struct joystick *element);																							
void joystick_fire(struct joystick *element);																						

#endif
