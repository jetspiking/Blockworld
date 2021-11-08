#pragma once
#include <iostream>
#include <sstream>
#include "FpsCam.h"
#include "BlockAtlas.h"
#include <cmath>

struct GLFWwindow;
// The player class stores BlockWorld player data, such as which block is currently selected by the player.
class Player
{
public:
	// Constructor needs a GLFWwindow pointer due to potential future usage.
	Player(GLFWwindow* windowPointer);
	// Scroll wheel callback to increase or decrease the currently selected block.
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	// Get the chunk in which the player is present.
	glm::vec3 getPlayerChunkLocation(const int chunkSizeWidth, const int chunkSizeHeight, const int chunkSizeDepth);
	// Get the block in which the player is standing.
	glm::vec3 getPlayerBlockPosition(const int chunkSizeWidth, const int chunkSizeHeight, const int chunkSizeDepth);
	// Update the player data.
	void update();
	// Pointer to the FPS-camera, needed so other classes can access the camera data.
	std::shared_ptr<FpsCam> getCamera() const noexcept;
	// Initialize the currently selected blocktype on air.
	int selectedBlockType = BLOCK_TYPE_AIR;
private:
	// Pointer to the GLFWwindow.
	GLFWwindow* windowPointer;
	// Pointer to the FpsCamera.
	std::shared_ptr<FpsCam> fpsCamera;
};

