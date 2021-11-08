#pragma once
#include "DecorationGLUnit.h"
#include "Shape.h"
#include "tigl.h"

class ColorGLUnit : public DecorationGLUnit
{
public:
	ColorGLUnit();
	~ColorGLUnit();
	virtual void update() const override;
	virtual void fill_vertices(std::vector<glm::vec3>& indices, std::vector<tigl::Vertex>& vertices_out, Shape::ShapeType& shapeType) const override;
	void set_texture_color_filter(float r, float g, float b, float a);
	glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f);
};