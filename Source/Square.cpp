#include "Square.h"

//	  C +-------+ D
//		|       |
//		|       |
//		|       | 
//	  A +-------+ B

Square::Square(glm::vec3 position, glm::vec3 dimensions) : Shape::Shape(position, dimensions)
{
	this->set_shape_type(Shape::SQUARE);
}

void Square::getVertices(std::vector<glm::vec3>& vertices_out) const {
	vertices_out.resize(Square::vertices_count);
	//Vertices centered
	vertices_out[SQUARE_A] = glm::vec3(position.x - (dimensions.x / 2.), position.y - (dimensions.y / 2.), position.z + (dimensions.z / 2.));		// A
	vertices_out[SQUARE_B] = glm::vec3(position.x + (dimensions.x / 2.), position.y - (dimensions.y / 2.), position.z + (dimensions.z / 2.));		// B
	vertices_out[SQUARE_C] = glm::vec3(position.x - (dimensions.x / 2.), position.y + (dimensions.y / 2.), position.z + (dimensions.z / 2.));		// C
	vertices_out[SQUARE_D] = glm::vec3(position.x + (dimensions.x / 2.), position.y + (dimensions.y / 2.), position.z + (dimensions.z / 2.));		// D
}

void Square::getAllIndices(std::vector<glm::vec3>& vertices_out, std::vector<glm::vec3>& indices_out) const
{
	getVertices(vertices_out);
	indices_out.resize(Square::indices_count);
	//CABD
	indices_out[0] = vertices_out[SQUARE_C];
	indices_out[1] = vertices_out[SQUARE_A];
	indices_out[2] = vertices_out[SQUARE_B];
	indices_out[3] = vertices_out[SQUARE_D];
}
