#pragma once
#include "WorldShape.h"
#include "HUDAtlas.h"
#include "Square.h"

// Class HUDFrame is a childclass of WorldShape.
class HUDFrame : public WorldShape
{
public:
	// To construct HUDFrame a smart pointer to a square object is needed.
	HUDFrame(std::shared_ptr<Square> shape);
	// Initialize the HUD texture usage.
	void initializeHUDTextureUsage();
	// Initialize the texture usage (more friendly less hardcoded function).
	void initializeTextureUsage(glm::vec2 atlas_coords, glm::vec2 atlas_dimensions);
	// Set the glm::vec4 to change the RGB and alpha values.
	void setColor(glm::vec4 rgba_color);
	// Function to get all vertices needed for drawing using the tigl library.
	virtual void getSpecificationVertices(std::shared_ptr<std::vector<tigl::Vertex>> specification_vertices, std::shared_ptr<std::vector<glm::vec3>> vertices_in, std::shared_ptr<std::vector<glm::vec3>> indices_in, int drawmask) override;
	// Atlas ID is initialized on HUD_TYPE_FRAME.
	int atlasID = HUD_TYPE_FRAME;
	// Set the glm::vec4 to change the RGB and alpha values.
	glm::vec4 rgbaColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
	// Texture coordinates.
	glm::vec2 textureCoordinates[4]{
		glm::vec2(),
		glm::vec2(),
		glm::vec2(),
		glm::vec2()
	};
	// Normals.
	glm::vec2 normals[4]{
		glm::vec3()
	};
};

