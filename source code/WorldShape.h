#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "Shape.h"

// Class WorldShape
class WorldShape
{
private:
protected:
	// Contains a pointer to a shape.
	std::shared_ptr<Shape> shape;
	// Constructor accepts a pointer to a shape, to set as reference later.
	WorldShape(std::shared_ptr<Shape> shape);
public:
	// Abstract function that should pass a vector with tigl::Vertex objects, so the object can be drawn directly after getting these vertices.
	virtual void getSpecificationVertices(std::shared_ptr<std::vector<tigl::Vertex>> specification_vertices, std::shared_ptr<std::vector<glm::vec3>> vertices_in, std::shared_ptr<std::vector<glm::vec3>> indices_in, int drawmask);

};

