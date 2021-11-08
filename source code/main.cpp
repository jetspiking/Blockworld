#define _CRT_SECURE_NO_WARNINGS
// Only allowed in main file to prevent linker errors.
#define STB_IMAGE_IMPLEMENTATION 

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <time.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

// Including min / max breaks std due to the horrible naming of literally "min" and "max". Luckily Windows provided an option to disable this by defining "NOMINMAX".
#define NOMINMAX	
// Including windows so audio files can be played.
#include <windows.h>	
// Includes for the imgui library.
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

#include "tigl.h"
#include "cuboid.h"
#include "Pyramid.h"
#include "stb_image.h"
#include "GameCamera.h"
#include "BlockWorld.h"
#include "HUDFrame.h"
#include "FpsCam.h"
#include "Square.h"
#include "ModelLoader.h"

#define VSYNC_ON 1
#define VSYNC_OFF 0

//Prototypes
// On key callback to receive GLFW keyboard events.
void glfw_onKey(GLFWwindow * window, int key, int scancode, int action, int mode);
// Update function to update the deltatime.
void update();
// Update the GUI.
void imGuiUpdate();
// Initialize Dear ImGui.
void initImGui();
// Destroy ImGui for cleanup.
void destroyImGui();
// Initialize OpenGL.
bool initOpenGL();
// Intialize function to setup BlockWorld.
bool init();
// Create BlockWorld.
void create_BlockWorld();
// Enable Lighting in the shader.
void enableLighting();
// Disable Lighting in the shader.
void disableLighting();
// Draw function for BlockWorld and HUD elements.
void draw();
// Check application settings.
void checkApplicationSettings();
// Draw the HUD.
void drawHud(float aspect);
// Draw the player orientation in the top left corner.
void drawPlayerOrientation(float aspect);
// Read the configuration.
void readConfig();
// Write the configuration.
void writeConfig();

//ARC vars
// App title.
const char* APP_TITLE = "Blockworld";
// Save file name.
const char* APP_SAVE_FILENAME = "Blockworld.config";
// Window width on application startup.
const int gWindowWidth = 800;
// Window height on application startup.
const int gWindowHeight = 500;

// Whether the application is triggered fullscreen or not.
bool gFullscreen = false;
// Whether vsync is enabled or not.
bool gVsync = true;
// Last stored value of vsync.
bool gVsyncLast = false;
// Defines whether color should be used in the shader or not.
bool gUseColor = true;
// Last known value to decide color should be used in the shader or not.
bool gUseColorLast = false;
// Defines whether textures should be used in the shader or not.
bool gUseTextures = true;
// Last known value to decide textures should be used in the shader or not.
bool gUseTexturesLast = false;
// Defines whether lighting should be used in the shader or not.
bool gUseLighting = true;
// Last known value to decide lighting should be used in the shader or not.
bool gUseLightingLast = false;
// Defines whether an orthographic projection should be used or not.
bool gUseOrthographicProjection = false;
// Defines whether the configuration should be read or not.
bool gReadPressed = false;
// Defines whether the configuration should be written or not.
bool gWritePressed = false;
 
// Render distance is initialized on 1.
int gRenderDistance = 1;
// The last known render distance defines whether chunks should be reloaded or not.
int gRenderDistanceLast = 1;
// Texture id that is incremented automatically to prevent double texture id's.
int textureID = 0;
// HUD texture id.
int hudTextureID;
// Blockworld texture id.
int blockworldTextureID;

// Window pointer
GLFWwindow* gWindow = NULL;

// Shared (smart) pointers
// GameCamera pointer.
std::shared_ptr<GameCamera> gCamera;
// Buffer pointer.
std::shared_ptr<std::vector<tigl::Vertex>> gBuffer = std::make_shared<std::vector<tigl::Vertex>>();
// Vertices pointer.
std::shared_ptr<std::vector<glm::vec3>> gVertices_in = std::make_shared<std::vector<glm::vec3>>();
// Indices pointer.
std::shared_ptr<std::vector<glm::vec3>> gIndices_in = std::make_shared<std::vector<glm::vec3>>();
// Blockworld pointer.
std::shared_ptr<BlockWorld> gBlockWorld;
// Blockworld ModelLoader.
std::shared_ptr<ModelLoader> gModelLoader;

//Clear all OpenGL errors.
static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

// Read OpenGL errors.
static void GLCheckError() {
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
	}
}

// Main function
int main(void)
{
	// Initialize OpenGL and shaders.
	std::cout << "Initialize program" << std::endl;
	if (!init()) {
		return -1;
	}
	// Initialize Dear ImGui.
	initImGui();
	// Set the clear color to a light shade of blue that represents the sky.
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	// Create blockworld.
	create_BlockWorld();
	// Start the music! Loops automatically.
	PlaySound(TEXT("Waterfalls_and_Dreams.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	// Load the "Steve" model that is shown in the top left corner of the screen.
	gModelLoader = std::make_shared<ModelLoader>("models/steve/steve.obj");
	// Setup is completed, main loop can now be added.
	std::cout << "Setup done, entering program loop..." << std::endl;
	while (!glfwWindowShouldClose(gWindow))
	{
		// Check the application booleans and settings.
		checkApplicationSettings();
		// Update items if needed and update deltatime.
		update();
		// Draw items on screen.
		draw();
		// Update the Dear ImGui library.
		imGuiUpdate();
		// Swap the GLFW buffers.
		glfwSwapBuffers(gWindow);
		// Poll the GLFW events.
		glfwPollEvents();
	}
	// Cleanup Dear ImGui.
	destroyImGui();
	// Terminate GLFW.
	glfwTerminate();
	// Exit with code 0, successful.
	return 0;
}

// Check the application settings.
void checkApplicationSettings() {
	if (gVsync != gVsyncLast) {
		glfwSwapInterval(gVsync ? VSYNC_ON : VSYNC_OFF);
		gVsyncLast = gVsync;
	}
	if (gUseLighting != gUseLightingLast) {
		gUseLighting ? enableLighting() : disableLighting();
		gUseLightingLast = gUseLighting;
	}
	if (gUseColor != gUseColorLast) {
		tigl::shader->enableColor(gUseColor);
		gUseColorLast = gUseColor;
	}
	if (gUseTextures != gUseTexturesLast) {
		tigl::shader->enableTexture(gUseTextures);
		gUseTexturesLast = gUseTextures;
	}
	if (gRenderDistance != gRenderDistanceLast) {
		if (gBlockWorld) {
			gRenderDistanceLast = gRenderDistance;
			gBlockWorld->changeRenderDistance(gRenderDistance);
		}
	}
	if (gReadPressed) {
		std::cout << "Reading config " << std::endl;
		readConfig();
	}
	if (gWritePressed) {
		std::cout << "Writing config " << std::endl;
		writeConfig();
	}
}

// Update Dear ImGui windows.
void imGuiUpdate() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin(APP_TITLE);

	ImGui::Text("Graphical settings\n\n");
	ImGui::Checkbox("Vsync", &gVsync);
	ImGui::Checkbox("Lighting", &gUseLighting);
	ImGui::Checkbox("Textures", &gUseTextures);
	ImGui::Checkbox("Colors", &gUseColor);
	ImGui::Text("\n\nApplication settings\n\n");
	ImGui::Checkbox("Orthographic projection", &gUseOrthographicProjection);
	ImGui::Text("\nRender radius in chunks\n");
	ImGui::SliderInt("", &gRenderDistance, 0, 10);

	ImGui::Text("\n\nDebugging information:\n\n");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::Text("\n\Settings configuration\n");
	gReadPressed = ImGui::Button("Read (load)");
	gWritePressed = ImGui::Button("Write (save)");

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// Initialize Dear ImGui.
void initImGui() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(gWindow, true);
	ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
}

// Cleanup Dear ImGui.
void destroyImGui() {
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}

// Initialize OpenGL.
bool initOpenGL() {
	if (!glfwInit())
	{
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}
	if (gFullscreen)
	{
		GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* pVmode = glfwGetVideoMode(pMonitor);
		if (pVmode != NULL)
			gWindow = glfwCreateWindow(pVmode->width, pVmode->height, APP_TITLE, pMonitor, NULL);
	}
	else
	{
		gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
	}
	if (gWindow == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(gWindow);
	glfwSetKeyCallback(gWindow, glfw_onKey);
	glewExperimental = GL_TRUE;
	std::cout << "Glew init" << std::endl;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to init GLEW" << std::endl;
		return false;
	}
	std::cout << "Clear color, depth test, glfwSwapInterval" << std::endl;
	glClearColor(.4f, .75f, .6f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glfwSwapInterval(VSYNC_ON);

	return true;
}

// Generate a texture with a unique ID that is incremented automatically.
int generateTexture(const char* texture) {
	int width, height, comp;
	unsigned char* data = stbi_load(texture, &width, &height, &comp, STBI_rgb_alpha);
	GLuint textureId = ++textureID;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // GL_NEAREST
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
	stbi_image_free(data);
	return textureID;
}

// Initialize the tigl library.
bool init()
{
	if (!initOpenGL())
		return false;
	tigl::init();
	blockworldTextureID = generateTexture("textures_blockworld.png");
	gCamera = std::make_shared<GameCamera>(gWindow);
	return true;
}

// Enable lighting.
void enableLighting() {
	glm::vec3 sunPos = gBlockWorld->getSunPosition();
	sunPos.x += 200;
	tigl::shader->enableLighting(true);
	tigl::shader->setLightCount(1);
	tigl::shader->setLightPosition(0, sunPos);
	tigl::shader->setLightDirectional(0, true);
	tigl::shader->setLightAmbient(0, glm::vec3(.5f, .5f, .5f));
	tigl::shader->setLightDiffuse(0, glm::vec3(.6f, .6f, .6f));
	tigl::shader->setLightSpecular(0, glm::vec3(1.f, .8f, .4f));
	tigl::shader->setShinyness(5.f);
}

// Disable lighting.
void disableLighting() {
	tigl::shader->enableLighting(false);
}

// Create BlockWorld.
void create_BlockWorld() {
	gBlockWorld = std::make_shared<BlockWorld>(gBuffer, gVertices_in, gIndices_in, gWindow);
	gBlockWorld->getPlayer()->getCamera()->position = -1.f * gBlockWorld->getSurfaceCenterPosition();
}

// Key callback, can be used in the future for a more clean way to callback keys to classes.
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
}

// Define the desired amount of updates per second.
static const double updatesPerSecond = 100.;
// Timer value.
static double timer = 1 / updatesPerSecond;
// Last frame time.
double lastFrameTime = .0;

// Update the deltatime and update the Blockworld player.
void update()
{
	double currentFrameTime = glfwGetTime();
	double deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

	timer -= deltaTime;
	if (timer <= 0) {
		timer = 1. / updatesPerSecond;
		gBlockWorld->getPlayer()->update();
	}
}

// Read the stored configuration file.
void readConfig() {
	std::ifstream configFile(APP_SAVE_FILENAME);
	std::string data;
	if (configFile.is_open())
	{
		while (std::getline(configFile, data))
		{
			std::cout << "Reading line...\n" << data << "\n";
			std::size_t found = data.find("vsync");
			if (found != std::string::npos) {
				gVsync = data.find("0") == std::string::npos;
			}
			else 
			{
				found = data.find("lighting");
				if (found != std::string::npos) {
					gUseLighting = data.find("0") == std::string::npos;
				}
				else {
					found = data.find("textures");
					if (found != std::string::npos) {
						gUseTextures = data.find("0") == std::string::npos;
					}
					else {
						found = data.find("colors");
						if (found != std::string::npos) {
							gUseColor = data.find("0") == std::string::npos;
						}
						else {
							found = data.find("renderdistance");
							if (found != std::string::npos) {
								gRenderDistance = data.find("0") == std::string::npos;
							}
							else {
								found = data.find("orthographic");
								if (found != std::string::npos) {
									gUseOrthographicProjection = data.find("0") == std::string::npos;
								}
							}
						}
					}
				}
			}
		}
		configFile.close();
	}
}

// Write the configuration file.
void writeConfig() {
	std::ofstream configFile(APP_SAVE_FILENAME);
	if (configFile.is_open())
	{
		configFile << "vsync:" << gVsync << std::endl;
		configFile << "lighting:" << gUseLighting << std::endl;
		configFile << "textures:" << gUseTextures << std::endl;
		configFile << "colors:" << gUseColor << std::endl;
		configFile << "renderdistance:" << gRenderDistance << std::endl;
		configFile << "orthographic:" << gUseOrthographicProjection << std::endl;
		configFile.close();
	}
	else std::cout << "Unable to create file, do you have write permissions?";
}

// Draw the HUD
void drawHud(float aspect) {
	float HUD_DIMENSION = .2;
	std::shared_ptr<Square> square = std::make_shared<Square>(glm::vec3(.05, -1 + (HUD_DIMENSION / 2.) + .1, -1), glm::vec3(HUD_DIMENSION, HUD_DIMENSION * aspect, .1));
	std::shared_ptr<Square> playerBlock = std::make_shared<Square>(glm::vec3(.05, -1 + (HUD_DIMENSION / 2.) + .1, -1), glm::vec3((HUD_DIMENSION / 2.5f), (HUD_DIMENSION / 2.5f) * aspect, .09));

	std::shared_ptr<HUDFrame> hudFrame = std::make_shared<HUDFrame>(square);
	std::shared_ptr<HUDFrame> playerFrame = std::make_shared<HUDFrame>(playerBlock);

	hudFrame->setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
	playerFrame->setColor(glm::vec4(1.f, 1.f, 1.f, .9f));

	hudFrame->initializeHUDTextureUsage();
	playerFrame->initializeTextureUsage(glm::vec2(2, gBlockWorld->getPlayer()->selectedBlockType), glm::vec2(25, 22));

	hudFrame->getSpecificationVertices(gBuffer, gVertices_in, gIndices_in, -1);
	if (gBlockWorld->getPlayer()->selectedBlockType != BLOCK_TYPE_AIR)
		playerFrame->getSpecificationVertices(gBuffer, gVertices_in, gIndices_in, -1);
}

// Draw the player orientation in the top left corner (rotated Steve)
void drawPlayerOrientation(float aspect) {
	glm::mat4 playerModel = glm::mat4(1.f);
	playerModel = glm::translate(playerModel, glm::vec3(-0.8f, 0.5f, 0));
	playerModel = glm::rotate(playerModel, gBlockWorld->getPlayer()->getCamera()->rotation.x, glm::vec3(1, 0, 0));
	playerModel = glm::rotate(playerModel, gBlockWorld->getPlayer()->getCamera()->rotation.y, glm::vec3(0, 1, 0));
	playerModel = glm::rotate(playerModel, 0.f, glm::vec3(0, 0, 1));
	playerModel = glm::scale(playerModel, glm::vec3(.045f, .05f, .05f));
	tigl::shader->setModelMatrix(playerModel);
	gModelLoader->draw();
}

// Draw function 
void draw() {
	if (gUseLighting)
		tigl::shader->enableLighting(true);

	int width, height;

	// 3D drawing
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity;
	glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BITS);
	//END

	glfwGetWindowSize(gWindow, &width, &height);
	glViewport(0, 0, width, height);
	glm::mat4 projection;

	if (!gUseOrthographicProjection)
		projection = glm::perspective(glm::radians(70.f), width / (float)height, .1f, 50000.f);
	else
		projection = glm::ortho(-50.f, 50.f, -50.f, 50.f, -5000.f, 5000.f);

	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setViewMatrix(gBlockWorld->getPlayer()->getCamera()->getMatrix());
	glm::mat4 parentMatrix1 = glm::mat4(1.f);
	tigl::shader->setModelMatrix(parentMatrix1);

	float m_frustumZNear = .1f;
	float m_frustumZFar = 1000.f;
	float aspect;
	if (height == 0) height = 1;
	aspect = width / height;

	glFrustum(-aspect * m_frustumZNear, aspect * m_frustumZNear, -m_frustumZNear, m_frustumZNear, m_frustumZNear, m_frustumZFar);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	double xPosi, yPosi;
	glfwGetCursorPos(gWindow, &xPosi, &yPosi);

	static int holdingKeyQ = false;
	static int holdingKeyE = false;

	if (glfwGetKey(gWindow, GLFW_KEY_Q) == GLFW_PRESS) {
		holdingKeyQ = true;
	}
	if (glfwGetKey(gWindow, GLFW_KEY_Q) == GLFW_RELEASE) {
		if (holdingKeyQ) {
			gBlockWorld->placeBlock(true);
			holdingKeyQ = false;
		}
	}

	if (glfwGetKey(gWindow, GLFW_KEY_E) == GLFW_PRESS) {
		holdingKeyE = true;
	}
	if (glfwGetKey(gWindow, GLFW_KEY_E) == GLFW_RELEASE) {
		if (holdingKeyE) {
			gBlockWorld->placeBlock(false);
			holdingKeyE = false;
		}
	}

	glBindTexture(GL_TEXTURE_2D, blockworldTextureID);
	gBlockWorld->draw();

	if (gUseLighting)
		tigl::shader->enableLighting(false);
	
	//2D overlay drawing
	glClear(GL_DEPTH_BUFFER_BIT);
	tigl::shader->setProjectionMatrix(glm::mat4(1.f));
	tigl::shader->setViewMatrix(glm::mat4(1.f));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity;
	glOrtho(0, width, height, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity;
	glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BITS);
	
	drawHud(aspect);
	drawPlayerOrientation(aspect);

	tigl::shader->setModelMatrix(glm::mat4(1.f));

	glBindTexture(GL_TEXTURE_2D, blockworldTextureID);
	tigl::drawVertices(GL_QUADS, *gBuffer);
	gBuffer->clear();


}

