#include "FpsCam.h"


// When escape is pressed the mouse pointer can be locked / unlocked.
static bool holdButton = false;

FpsCam::FpsCam()
{
}

//FpsCam::FpsCam(GLFWwindow* window)
//{
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//	if (glfwRawMouseMotionSupported())
//		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
//}

void FpsCam::init(GLFWwindow* window) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

glm::mat4 FpsCam::getMatrix()
{
	glm::mat4 ret(1.0f);
	ret = glm::rotate(ret, rotation.x, glm::vec3(1, 0, 0));
	ret = glm::rotate(ret, rotation.y, glm::vec3(0, 1, 0));
	ret = glm::translate(ret, position);
	return ret;
}

void FpsCam::move(float angle, float fac)
{
	position.x += (float)cos(rotation.y + glm::radians(angle)) * fac;
	position.z += (float)sin(rotation.y + glm::radians(angle)) * fac;
}
void FpsCam::escapePressed(bool truePressedFalseReleased, GLFWwindow* window) {
	if (truePressedFalseReleased) {
		if (!holdButton) {
			glfwSetInputMode(window, GLFW_CURSOR, this->mousePointerEnabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
			this->mousePointerEnabled = !this->mousePointerEnabled;
		}
		holdButton = true;
	}
	else holdButton = false;
}
void FpsCam::updateCursor(double x, double y) {
	if (!mousePointerEnabled) {
		//double x, y;
		//glfwGetCursorPos(window, &x, &y);

		static double lastX = x;
		static double lastY = y;

		rotation.x -= (float)(lastY - y) / 100.0f;
		rotation.y -= (float)(lastX - x) / 100.0f;

		if (rotation.x >= MAX_ROTATIONX_RADIUS)
			rotation.x = MAX_ROTATIONX_RADIUS;
		else if (rotation.x <= MIN_ROTATIONX_RADIUS)
			rotation.x = MIN_ROTATIONX_RADIUS;

		lastX = x;
		lastY = y;
	}
}
void FpsCam::move(int moveMask) {
	if (moveMask & MoveMask::LEFT)
		moveLeft();
	if (moveMask & MoveMask::RIGHT)
		moveRight();
	if (moveMask & MoveMask::FORWARD)
		moveForward();
	if (moveMask & MoveMask::BACKWARDS)
		moveBackwards();
	if (moveMask & MoveMask::UP)
		moveUpwards();
	if (moveMask & MoveMask::DOWN)
		moveDownwards();
}

void FpsCam::moveLeft() {
	move(0, .5f);
}
void FpsCam::moveRight() {
	move(180, .5f);
}
void FpsCam::moveForward() {
	move(90, .5f);
}
void FpsCam::moveBackwards() {
	move(-90, .5f);
}
void FpsCam::moveUpwards() {
	this->position.y -= .5f;
}
void FpsCam::moveDownwards() {
	this->position.y += .5f;
}


void FpsCam::update(GLFWwindow* window)
{
	if (!mousePointerEnabled) {
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		static double lastX = x;
		static double lastY = y;

		rotation.x -= (float)(lastY - y) / 100.0f;
		rotation.y -= (float)(lastX - x) / 100.0f;

		if (rotation.x >= MAX_ROTATIONX_RADIUS)
			rotation.x = MAX_ROTATIONX_RADIUS;
		else if (rotation.x <= MIN_ROTATIONX_RADIUS)
			rotation.x = MIN_ROTATIONX_RADIUS;

		lastX = x;
		lastY = y;


		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			move(0, .5f);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			move(180, .5f);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			move(90, .5f);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			move(-90, .5f);
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			this->position.y -= .5f;
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			this->position.y += .5f;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		if (!holdButton) {
			glfwSetInputMode(window, GLFW_CURSOR, this->mousePointerEnabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
			this->mousePointerEnabled = !this->mousePointerEnabled;
		}
		holdButton = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
		holdButton = false;
	} 
}