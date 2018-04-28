#pragma once

#include "level.h"

struct Triangle
{
	glm::vec3 p1, p2, p3;

	Triangle(glm::vec3 _p1, glm::vec3 _p2, glm::vec3 _p3)
	{
		p1 = _p1;
		p2 = _p2;
		p3 = _p3;
	}
};

struct Ray
{
	glm::vec3 origin, dir;
};

class MainLevel : public Level
{
public:
	//Fill level with objects. Initialize all objects
	void Initialize();
	//Update all objects. Apply level logic
	void Update();

private:
	//Mouse click func
	static MainLevel* s_instance; //Used by mouse func
	static void MouseFunc(int _button, int _buttonState, int _x, int _y);

	static glm::vec3 TransformPoint(glm::vec3 _p, glm::mat4 _transform);
	static glm::vec3 GetRayDirection(float _x, float _y);
	
};