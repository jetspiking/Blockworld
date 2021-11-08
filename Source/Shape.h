#pragma once
#include <iostream>
#include <sstream>
#include "utils.h"
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>

class Shape
{
public:
	typedef enum ShapeType { ABSTRACT, CUBOID, CYLINDER, PRISM, PYRAMID, SPHERE, SQUARE } ShapeType;
	virtual void getAllIndices(std::vector<glm::vec3>& vertices_out, std::vector<glm::vec3>& indices_out) const = 0;
	virtual ShapeType get_shape_type() const noexcept;
protected:
	Shape(glm::vec3 position, glm::vec3 dimensions);
	glm::vec3 position, dimensions;
	void set_shape_type(ShapeType);
private:
	ShapeType shape_type = ABSTRACT;
};

