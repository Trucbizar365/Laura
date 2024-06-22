#ifndef DELTA_TIME_H
#define DELTA_TIME_H

#include <GLFW/glfw3.h>

/**
* @brief DeltaTime class is used to calculate the time between frames to make the movement of objects independent of the frame rate.
 */
class DeltaTime
{
private:
	double prev_time;
	double curr_time;
public:
	DeltaTime();
	double getDeltaTime();
	void update();
};

#endif // DELTA_TIME_H
