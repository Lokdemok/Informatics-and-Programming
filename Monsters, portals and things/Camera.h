using namespace sf;

View camera;

void getplayercoordinateforview(float x, float y)
{
	float tempX = x; float tempY = y;
	if (x < 320) tempX = 320;
	if (y < 240) tempY = 240;
	if (y > 554) tempY = 554;

	camera.setCenter(tempX, tempY); ;
}
