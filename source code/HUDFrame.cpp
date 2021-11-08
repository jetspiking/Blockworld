#include "HUDFrame.h"

HUDFrame::HUDFrame(std::shared_ptr<Square> shape) : WorldShape::WorldShape(shape)
{
}

void HUDFrame::initializeHUDTextureUsage() {
	textureCoordinates[0] = glm::vec2(.08, 1);
	textureCoordinates[1] = glm::vec2(.08, 0);
	textureCoordinates[2] = glm::vec2(1, 0);
	textureCoordinates[3] = glm::vec2(1, 1);
}

void HUDFrame::initializeTextureUsage(glm::vec2 atlas_coords, glm::vec2 atlas_dimensions) {
	this->atlasID = atlas_coords.y;
	textureCoordinates[0] = glm::vec2((double)(atlas_coords.x - 1) / atlas_dimensions.x, (double)atlas_coords.y / atlas_dimensions.y);
	textureCoordinates[1] = glm::vec2((double)(atlas_coords.x - 1) / atlas_dimensions.x, (double)(atlas_coords.y - 1) / atlas_dimensions.y);
	textureCoordinates[2] = glm::vec2((double)atlas_coords.x / atlas_dimensions.x, (double)(atlas_coords.y - 1) / atlas_dimensions.y);
	textureCoordinates[3] = glm::vec2((double)atlas_coords.x / atlas_dimensions.x, (double)atlas_coords.y / atlas_dimensions.y);
}

void HUDFrame::setColor(glm::vec4 rgba_color) {
	rgbaColor = rgba_color;
}

void HUDFrame::getSpecificationVertices(std::shared_ptr<std::vector<tigl::Vertex>> specification_vertices, std::shared_ptr<std::vector<glm::vec3>> vertices_in, std::shared_ptr<std::vector<glm::vec3>> indices_in, int drawmask)
{
	std::shared_ptr<Square> square = std::static_pointer_cast<Square>(this->shape);

	square->getAllIndices(*vertices_in, *indices_in);
	for (int i = 0; i < indices_in->size(); i++)
		if (i < 4)
			specification_vertices->push_back(tigl::Vertex::PTC((*indices_in)[i], textureCoordinates[i % 4], rgbaColor));
}

