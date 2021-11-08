#include "Block.h"

Block::Block(std::shared_ptr<Cuboid> shape) : WorldShape::WorldShape(shape)
{
}

void Block::initializeTextureUsage(glm::vec2 atlas_coords, glm::vec2 atlas_dimensions) {
	this->atlasID = atlas_coords.y;
	textureCoordinates[0] = glm::vec2((double)(atlas_coords.x -1)	/ atlas_dimensions.x, (double) atlas_coords.y		/ atlas_dimensions.y);
	textureCoordinates[1] = glm::vec2((double)(atlas_coords.x -1)	/ atlas_dimensions.x, (double)(atlas_coords.y - 1)	/ atlas_dimensions.y);
	textureCoordinates[2] = glm::vec2((double) atlas_coords.x		/ atlas_dimensions.x, (double)(atlas_coords.y - 1)	/ atlas_dimensions.y);
	textureCoordinates[3] = glm::vec2((double) atlas_coords.x		/ atlas_dimensions.x, (double) atlas_coords.y		/ atlas_dimensions.y);

	textureCoordinatesTop[0] = glm::vec2((double)((atlas_coords.x+1) - 1) / atlas_dimensions.x, (double)atlas_coords.y / atlas_dimensions.y);
	textureCoordinatesTop[1] = glm::vec2((double)((atlas_coords.x+1) - 1) / atlas_dimensions.x, (double)(atlas_coords.y - 1) / atlas_dimensions.y);
	textureCoordinatesTop[2] = glm::vec2((double)(atlas_coords.x+1) / atlas_dimensions.x, (double)(atlas_coords.y - 1) / atlas_dimensions.y);
	textureCoordinatesTop[3] = glm::vec2((double)(atlas_coords.x+1) / atlas_dimensions.x, (double)atlas_coords.y / atlas_dimensions.y);
}

void Block::setColor(glm::vec4 rgba_color) {
	rgbaColor = rgba_color;
}

void Block::getSpecificationVertices(std::shared_ptr<std::vector<tigl::Vertex>> specification_vertices, std::shared_ptr<std::vector<glm::vec3>> vertices_in, std::shared_ptr<std::vector<glm::vec3>> indices_in, int drawmask) 
{
	this->drawmask = drawmask;
	std::shared_ptr<Cuboid> cuboid = std::static_pointer_cast<Cuboid>(this->shape);

	cuboid->getIndicesSide(*vertices_in, *indices_in, drawmask);
	for (int i = 0; i < indices_in->size(); i++)
		if (i < 4)
			specification_vertices->push_back(tigl::Vertex::PTC((*indices_in)[i], textureCoordinatesTop[i % 4], rgbaColor));
		else specification_vertices->push_back(tigl::Vertex::PTC((*indices_in)[i], textureCoordinates[i % 4], rgbaColor));

}	

