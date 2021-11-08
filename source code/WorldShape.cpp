#include "WorldShape.h"

WorldShape::WorldShape(std::shared_ptr<Shape> shape)
{
	this->shape = shape;
}

void WorldShape::getSpecificationVertices(std::shared_ptr<std::vector<tigl::Vertex>> specification_vertices, std::shared_ptr<std::vector<glm::vec3>> vertices_in, std::shared_ptr<std::vector<glm::vec3>> indices_in, int drawmask)
{
}
