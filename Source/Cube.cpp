#include "Cube.h"



tigl::Vertex* Shape::get_vertices(void)
{
	glm::vec3 vertices[] = {
		glm::vec3(this->x, this->y, this->z + this->depth),									//a (0)
		glm::vec3(this->x + this->width, this->y, this->z + this->depth),					//b (1)
		glm::vec3(this->x, this->y + this->height, this->z + this->depth),					//c (2)
		glm::vec3(this->x + this->width, this->y + this->height, this->z + this->depth),	//d (3)
		glm::vec3(this->x, this->y, this->z),												//e (4)
		glm::vec3(this->x + this->width, this->y, this->z),									//f (5)
		glm::vec3(this->x, this->y + this->width, this->z),									//g (6)
		glm::vec3(this->x + this->width, this->y + this->height, this->z)					//h (7)
	};

	glm::vec4 vectors[] = {
		glm::vec4(1,0,0,0),
		glm::vec4(0,1,0,0),
		glm::vec4(0,0,1,0),
		glm::vec4(0,0,0,1)
	};

	tigl::Vertex indices[] = {
		//front - abcd
		tigl::Vertex::PC(vertices[2], vectors[0]),
		tigl::Vertex::PC(vertices[0], vectors[1]),
		tigl::Vertex::PC(vertices[1], vectors[2]),
		tigl::Vertex::PC(vertices[3], vectors[3]),
		//right - bfdh
		tigl::Vertex::PC(vertices[3], vectors[0]),
		tigl::Vertex::PC(vertices[1], vectors[1]),
		tigl::Vertex::PC(vertices[5], vectors[2]),
		tigl::Vertex::PC(vertices[7], vectors[3]),
		//back - efgh
		tigl::Vertex::PC(vertices[6], vectors[0]),
		tigl::Vertex::PC(vertices[4], vectors[1]),
		tigl::Vertex::PC(vertices[5], vectors[2]),
		tigl::Vertex::PC(vertices[7], vectors[3]),
		//left - aecg
		tigl::Vertex::PC(vertices[2], vectors[0]),
		tigl::Vertex::PC(vertices[0], vectors[1]),
		tigl::Vertex::PC(vertices[4], vectors[2]),
		tigl::Vertex::PC(vertices[6], vectors[3]),
		//bottom - abef
		tigl::Vertex::PC(vertices[4], vectors[0]),
		tigl::Vertex::PC(vertices[0], vectors[1]),
		tigl::Vertex::PC(vertices[1], vectors[2]),
		tigl::Vertex::PC(vertices[5], vectors[3]),
		//top - cdgh
		tigl::Vertex::PC(vertices[6], vectors[0]),
		tigl::Vertex::PC(vertices[2], vectors[1]),
		tigl::Vertex::PC(vertices[3], vectors[2]),
		tigl::Vertex::PC(vertices[7], vectors[3]),
	};
	return indices;
}
