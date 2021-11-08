#include "GLShape.h"

GLShape::GLShape()
{
}

GLShape::GLShape(std::shared_ptr<Shape> shape)
{
	this->shape = shape;
}

void GLShape::draw() const
{
	if (this->vertex_vector && this->vertices && this->indices) {
		GLenum draw_type = get_draw_type();

		this->shape->get_indices(*this->vertices, *this->indices);
		this->vertex_vector->resize(this->indices->size());

		Shape::ShapeType shapeType = this->shape->get_shape_type();
		this->glObject->decorationGLUnit->fill_vertices(*this->indices, *this->vertex_vector, shapeType);

		tigl::drawVertices(draw_type, *this->vertex_vector);
	}
}

void GLShape::init_draw(std::shared_ptr<std::vector<tigl::Vertex>> vertex_vector, std::shared_ptr<std::vector<glm::vec3>> vertices, std::shared_ptr<std::vector<glm::vec3>> indices)
{
	this->vertex_vector = vertex_vector;
	this->vertices = vertices;
	this->indices = indices;
}

GLenum GLShape::get_draw_type() const
{
	GLenum draw_type = GL_TRIANGLES;

	switch (this->shape->get_shape_type()) {
	case Shape::ABSTRACT:
		std::cout << "Why are you attempting to draw an abstract shape? Skipping draw." << std::endl;
		break;
	case Shape::CUBOID: draw_type = GL_QUADS; break;
	case Shape::CYLINDER: break;
	case Shape::SPHERE: break;
	case Shape::PRISM: break;
	case Shape::PYRAMID: break;
	}
	return draw_type;
}