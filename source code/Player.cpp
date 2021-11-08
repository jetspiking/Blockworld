#include "Player.h"

Player::Player(GLFWwindow* windowPointer)
{
	this->windowPointer = windowPointer;
	this->fpsCamera = std::make_shared<FpsCam>();
	this->fpsCamera->init(windowPointer);
	glfwSetWindowUserPointer(windowPointer, this);
	glfwSetScrollCallback(windowPointer, scrollCallback);
}

void Player::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	yoffset = yoffset * -1;
	Player* player = static_cast<Player*>(glfwGetWindowUserPointer(window));
	if (player->selectedBlockType + yoffset >= BLOCK_TYPE_AIR && player->selectedBlockType + yoffset <= BLOCK_TYPE_WATER)
		player->selectedBlockType += yoffset;
}

glm::vec3 Player::getPlayerChunkLocation(const int chunkSizeWidth, const int chunkSizeHeight, const int chunkSizeDepth) {
	//glm::vec3 playerPosition = this->fpsCamera->position;
	int x;
	if (this->fpsCamera->position.x > 0) {
		x = ((int)(this->fpsCamera->position.x / chunkSizeWidth) + 1);
	}
	else 
	{
		x = (int)(this->fpsCamera->position.x / chunkSizeWidth);
	}
	
	int y;
	if (this->fpsCamera->position.y > 0) {
		y = ((int)(this->fpsCamera->position.y / chunkSizeHeight) + 1);
	}
	else
	{
		y = (int)(this->fpsCamera->position.y / chunkSizeHeight);
	}

	int z;
	if (this->fpsCamera->position.z > 0) {
		z = ((int)(this->fpsCamera->position.z / chunkSizeDepth) + 1);
	}
	else
	{
		z = (int)(this->fpsCamera->position.z / chunkSizeDepth);
	}

	return glm::vec3(x, y, z);
}

glm::vec3 Player::getPlayerBlockPosition(const int chunkSizeWidth, const int chunkSizeHeight, const int chunkSizeDepth) {
	return glm::vec3(abs((int)this->fpsCamera->position.x%chunkSizeWidth), abs((int)this->fpsCamera->position.y%chunkSizeHeight), abs((int)this->fpsCamera->position.z%chunkSizeDepth));
}

void Player::update() {
	int moveMask = 0;

	if (glfwGetKey(this->windowPointer, GLFW_KEY_A) == GLFW_PRESS)
		moveMask |= FpsCam::LEFT;
	if (glfwGetKey(this->windowPointer, GLFW_KEY_D) == GLFW_PRESS)
		moveMask |= FpsCam::RIGHT;
	if (glfwGetKey(this->windowPointer, GLFW_KEY_W) == GLFW_PRESS)
		moveMask |= FpsCam::FORWARD;
	if (glfwGetKey(this->windowPointer, GLFW_KEY_S) == GLFW_PRESS)
		moveMask |= FpsCam::BACKWARDS;
	if (glfwGetKey(this->windowPointer, GLFW_KEY_SPACE) == GLFW_PRESS)
		moveMask |= FpsCam::UP;
	if (glfwGetKey(this->windowPointer, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		moveMask |= FpsCam::DOWN;
	this->fpsCamera->move(moveMask);

	double x, y;
	glfwGetCursorPos(this->windowPointer, &x, &y);
	this->fpsCamera->updateCursor(x, y);
	

	if (glfwGetKey(this->windowPointer, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		this->fpsCamera->escapePressed(true, this->windowPointer);
	}
	else if (glfwGetKey(this->windowPointer, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
		this->fpsCamera->escapePressed(false, this->windowPointer);
	}
}

std::shared_ptr<FpsCam> Player::getCamera() const noexcept
{
	return this->fpsCamera;
}
