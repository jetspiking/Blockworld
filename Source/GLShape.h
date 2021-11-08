#pragma once
#include "Shape.h"
#include "Cuboid.h"
#include "Prism.h"
#include "Pyramid.h"
#include "DrawGLUnit.h"
#include "GLObject.h"

class GLShape : public DrawGLUnit
{
public:
	GLShape();
	GLShape(std::shared_ptr<Shape>);
	virtual void draw() const override;
	void init_draw(std::shared_ptr<std::vector<tigl::Vertex>> vertex_vector, std::shared_ptr<std::vector<glm::vec3>> vertices, std::shared_ptr<std::vector<glm::vec3>> indices);
	GLenum get_draw_type() const;
protected:
	std::shared_ptr<Shape> shape;
	std::shared_ptr<std::vector<tigl::Vertex>> vertex_vector = nullptr;
	std::shared_ptr<std::vector<glm::vec3>> vertices = nullptr;
	std::shared_ptr<std::vector<glm::vec3>> indices = nullptr;
private:
};

