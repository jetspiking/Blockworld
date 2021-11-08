#include "Pyramid.h"

//					 E
//					 + 
//					/ \`.
//				   /'  \ `. 
//				  /.    \  `.
//			   C /+------\---'+ D
//				/' ` .    \  '
//			   /'      ` . \'
//			A +------------+ B      

Pyramid::Pyramid(glm::vec3 position, glm::vec3 dimensions) : Shape::Shape(position, dimensions)
{
	this->set_shape_type(Shape::PYRAMID);
}

void Pyramid::getAllIndices(std::vector<glm::vec3>& vertices_out, std::vector<glm::vec3>& indices_out) const
{
	vertices_out.resize(Pyramid::vertices_count);
	//Vertices
	vertices_out[PYRAMID_A] = glm::vec3(position.x - (dimensions.x / 2.), position.y - (dimensions.y / 2.), position.z + (dimensions.z / 2.));	// A
	vertices_out[PYRAMID_B] = glm::vec3(position.x + (dimensions.x / 2.), position.y - (dimensions.y / 2.), position.z + (dimensions.z / 2.));	// B
	vertices_out[PYRAMID_C] = glm::vec3(position.x - (dimensions.x / 2.), position.y - (dimensions.y / 2.), position.z - (dimensions.z / 2.));	// C
	vertices_out[PYRAMID_D] = glm::vec3(position.x + (dimensions.x / 2.), position.y - (dimensions.y / 2.), position.z - (dimensions.z / 2.));	// D
	vertices_out[PYRAMID_E] = glm::vec3(position.x + (dimensions.x / 4.), position.y + (dimensions.y / 2.), position.z + (dimensions.z / 4.));  // E

	indices_out.resize(Pyramid::indices_count);
	//ABE
	indices_out[0] = vertices_out[PYRAMID_A]; 
	indices_out[1] = vertices_out[PYRAMID_B];
	indices_out[2] = vertices_out[PYRAMID_E];
	//BDE
	indices_out[3] = vertices_out[PYRAMID_B];
	indices_out[4] = vertices_out[PYRAMID_D];
	indices_out[5] = vertices_out[PYRAMID_E];
	//DCE
	indices_out[6] = vertices_out[PYRAMID_D];
	indices_out[7] = vertices_out[PYRAMID_C];
	indices_out[8] = vertices_out[PYRAMID_E];
	//CAE
	indices_out[9]  = vertices_out[PYRAMID_C];
	indices_out[10] = vertices_out[PYRAMID_A];
	indices_out[11] = vertices_out[PYRAMID_E];
	//ABC
	indices_out[12] = vertices_out[PYRAMID_A];
	indices_out[13] = vertices_out[PYRAMID_B];
	indices_out[14] = vertices_out[PYRAMID_C];
	//DCB
	indices_out[15] = vertices_out[PYRAMID_D];
	indices_out[16] = vertices_out[PYRAMID_C];
	indices_out[17] = vertices_out[PYRAMID_B];
}