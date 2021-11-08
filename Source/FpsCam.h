#pragma once
#include <glm/glm.hpp>
#include "utils.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

// Max rotation radius to prevent "snapping" your neck backwards.
constexpr const auto MAX_ROTATIONX_RADIUS = .5*PI;
// Min rotation radius to prevent "snapping" your neck forwards.
constexpr const auto MIN_ROTATIONX_RADIUS = -.5*PI;
// Forward decleration for GLFWwindow, therefor include is not needed.
struct GLFWwindow;
// Class FpsCam can be used for any first person purpose.
class FpsCam
{
public:
	// Movemask that can be combined by bitwise operations.
	typedef enum MoveMask {
		LEFT = 1,
		RIGHT = 2,
		FORWARD = 4,
		BACKWARDS = 8,
		UP = 16,
		DOWN = 32
	} MoveMask;
	// FpsCam constructor.
	FpsCam();
	// Init function needs GLFWwindow to store a reference.
	void init(GLFWwindow* window);
	// Get the camera view matrix.
	glm::mat4 getMatrix();
	// Get the player position.
	glm::vec3 position = glm::vec3(0, 0, 0);
	// Get the player rotation.
	glm::vec2 rotation = glm::vec2(0, 0);
	// Update function, GLFWwindow is passed for a direct update.
	void update(GLFWwindow*);
	// Notify function for whenever escape is pressed.
	void escapePressed(bool truePressedFalseReleased, GLFWwindow* window);
	// Update the mouse position, cursor location is parsed as argument.
	void updateCursor(double x, double y);
	// Move function.
	void move(int moveMask);

private:
	// Enable / disable pointer, so the player could use a mouse pointer in the application.
	bool mousePointerEnabled = false;
	// Move function to move with angle and factor.
	void move(float angle, float fac);
	// Move camera left.
	void moveLeft();
	// Move camera right.
	void moveRight();
	// Move camera forward.
	void moveForward();
	// Move camera backward.
	void moveBackwards();
	// Move camera upwards.
	void moveUpwards();
	// Move camera downwards.
	void moveDownwards();
};


