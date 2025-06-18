#include <stdlib.h>
#include "joystick.h"

struct joystick* joystick_create() {				

	struct joystick *element = (struct joystick*) malloc (sizeof(struct joystick));				
	if (!element) return NULL;												
	element->right = 0;
	element->left = 0;
	element->up = 0;
	element->down = 0;
	element->fire = 0;
	return element;																
}

void joystick_destroy(struct joystick *element){ free(element);}							

void joystick_left(struct joystick *element){ element->left = element->left ^ 1;}			

void joystick_right(struct joystick *element){ element->right = element->right ^ 1;}		

void joystick_up(struct joystick *element){ element->up = element->up ^ 1;}			

void joystick_down(struct joystick *element){ element->down = element->down ^ 1;}		

void joystick_fire(struct joystick *element){ element->fire = element->fire ^ 1;}
