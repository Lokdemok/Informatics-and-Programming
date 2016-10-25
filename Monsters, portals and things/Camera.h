#ifndef camera_h
#define camera_h

using namespace sf;

View camera;

View setPlayerCoordinateForView(float x, float y)
{
	float tempX = x; 
	float tempY = y;
	if (x < 320) tempX = 320;
	if (y < 240) tempY = 240;
	if (y > 720) tempY = 720;
	camera.setCenter(tempX, tempY); 
	return camera;
}
#endif