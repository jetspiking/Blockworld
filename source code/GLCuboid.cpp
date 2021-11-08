#include "GLCuboid.h"

GLCuboid::GLCuboid(std::shared_ptr<Cuboid> cuboid)
{
	this->shape = cuboid;
}

void GLCuboid::draw() const
{
	std::shared_ptr<Cuboid> cuboid = std::static_pointer_cast<Cuboid>(this->shape);
	
	if (drawMask==-1)
	cuboid->get_indices(*this->vertices, *this->indices);
	else
	cuboid->get_indices_side(*this->vertices, *this->indices, this->drawMask);
	
	this->vertex_vector->resize(this->indices->size());

	Shape::ShapeType shapeType = this->shape->get_shape_type();
	this->glObject->decorationGLUnit->fill_vertices(*this->indices, *this->vertex_vector, shapeType);
	
	std::shared_ptr<TextureColorComboGLUnit> decorationGLUnit = std::dynamic_pointer_cast<TextureColorComboGLUnit>(this->glObject->decorationGLUnit);
	if (decorationGLUnit) {
	}

	tigl::drawVertices(GL_QUADS, *this->vertex_vector);
}

void GLCuboid::setDrawMask(int drawMask)
{
	this->drawMask = drawMask;
}
