#pragma once
#include "GLUnit.h"
#include "Shape.h"
#include <iostream>

class DecorationGLUnit : public GLUnit
{
public:
	DecorationGLUnit();
	~DecorationGLUnit();
	virtual void update() const = 0;
	virtual void fill_vertices(std::vector<glm::vec3>& indices, std::vector<tigl::Vertex>& vertices_out, Shape::ShapeType& shapeType) const = 0;
};