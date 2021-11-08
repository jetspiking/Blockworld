#pragma once
#include "WorldShape.h"
#include "Cuboid.h"
#include "BlockAtlas.h"

// Class Block is a child class of WorldShape.
class Block : public WorldShape
{
public:
	// To construct a block an existing Cuboid object is needed, wrapped in a smart pointer for easier memory management.
	Block(std::shared_ptr<Cuboid> shape);
	// Initialize the textures.
	void initializeTextureUsage(glm::vec2 atlas_coords, glm::vec2 atlas_dimensions);
	// Set the color.
	void setColor(glm::vec4 rgba_color);
	// Get the tigl vertices which contain data about texture, color and normal which can be drawn directly.
	virtual void getSpecificationVertices(std::shared_ptr<std::vector<tigl::Vertex>> specification_vertices, std::shared_ptr<std::vector<glm::vec3>> vertices_in, std::shared_ptr<std::vector<glm::vec3>> indices_in, int drawmask) override;
	// A block is initialized on BLOCK_TYPE_AIR to prevent crashes because of forgetting to set the atlas ID.
	int atlasID = BLOCK_TYPE_AIR;
	// Integer to store the Cuboid drawmask.
	int drawmask = 0;
	// glm::vec4 to specify the block color and alpha values.
	glm::vec4 rgbaColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
	// Texturecoordinates of a block, these are column 1 in the resource atlas.
	glm::vec2 textureCoordinates[4]{
		glm::vec2(),
		glm::vec2(),
		glm::vec2(),
		glm::vec2()
	};
	// Texturecoordinates for the top of a block, these are column 2 in the resource atlas.
	glm::vec2 textureCoordinatesTop[4]{
		glm::vec2(),
		glm::vec2(),
		glm::vec2(),
		glm::vec2()
	};
	// Normals for the block, used for the block lighting.
	glm::vec2 normals[4]{
		glm::vec3()
	};
	protected:
private:
};

