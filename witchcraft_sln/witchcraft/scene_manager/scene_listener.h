#ifndef SCENE_LISTENER_H
#define SCENE_LISTENER_H

#include "scene_manager_2d.h"

typedef enum {SE_TIMER_EXPIRED=0}SCENE_EVENT_TYPE;

class SceneManager2D;

class qSceneListener : public qEngineObject
{
public:
	SCENE_EVENT_TYPE listening_for;
	virtual void event(SceneManager2D const & manager, void * callback_data) const = 0;
};

#endif // !SCENE_LISTENER_H
