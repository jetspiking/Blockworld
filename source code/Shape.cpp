#include "Shape.h"

Shape::Shape(glm::vec3 position, glm::vec3 dimensions)
{
	this->position = position;
	this->dimensions = dimensions;
}

void Shape::set_shape_type(ShapeType shape_type)
{
	this->shape_type=shape_type;
}

Shape::ShapeType Shape::get_shape_type() const noexcept
{
	return this->shape_type;
}


