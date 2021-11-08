#include "ColorGLUnit.h"

ColorGLUnit::ColorGLUnit()
{
}

ColorGLUnit::~ColorGLUnit()
{
}

void ColorGLUnit::update() const
{
	tigl::shader->enableTexture(false);
	tigl::shader->enableColor(true);
}

void ColorGLUnit::fill_vertices(std::vector<glm::vec3>& indices, std::vector<tigl::Vertex>& vertices_out, Shape::ShapeType& shapeType) const
{
	for (int i = 0; i < indices.size(); i++)
		vertices_out[i] = (tigl::Vertex::PC(indices[i], this->color));
}

void ColorGLUnit::set_texture_color_filter(float r, float g, float b, float a)
{
	this->color.x = r;
	this->color.y = g;
	this->color.z = b;
	this->color.a = a;
}
