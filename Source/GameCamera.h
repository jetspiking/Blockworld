#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "utils.h"
#include <iostream>

// EXTRA CAMERA CLASS TO ROTATE AROUND AN ORIGIN POSITION. FULLY FUNCTIONAL, BUT CURRENTLY NOT USED, THEREFOR NO FURTHER COMMENTARY IS ADDED!

#define INC_DEC_VALUE_RADIUS .07
#define INC_DEC_VALUE_ROTATIONX .0095
#define INC_DEC_VALUE_ROTATIONY .025

struct GLFWwindow;

class GameCamera
{
public:
	GameCamera(GLFWwindow*);
	glm::mat4 getMatrix() const;
	void update2(GLFWwindow*);
	void checkTargetRadius();
	void checkTargetRotation();
	void setPlayer1Cam();
	void setPlayer2Cam();
	void setStartGamePosition();
	glm::vec3 translation = glm::vec3(0, 0, 0);
	double targetRadius = -1;
	glm::vec3 targetRotation = glm::vec3(-1, -1 , -1);
private:
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	bool mousePointerEnabled = false;
	void move(float angle, float fac);
};


