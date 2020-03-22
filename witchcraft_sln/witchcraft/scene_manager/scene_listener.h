#ifndef SCENE_LISTENER_H
#define SCENE_LISTENER_H

#include "2d_scene_manager.h"

typedef enum {SE_TIMER_EXPIRED=0}SCENE_EVENT_TYPE;

class cSceneListener
{
public:
	SCENE_EVENT_TYPE _listening_for_type;

	virtual void event(c2DSceneManager const & manager, void * callback_data) const = 0;
};

#endif // !SCENE_LISTENER_H
