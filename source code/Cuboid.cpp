#include "Cuboid.h"

//		 G .+-------+ H
//		 .' |     .'|
//	  C +---+---+' D|
//		|   |   |   |
//		|E .+---+---+ F
//		|.'     | .'
//	  A +-------+' B

Cuboid::Cuboid(glm::vec3 position, glm::vec3 dimensions) : Shape::Shape(position, dimensions)
{
	this->set_shape_type(Shape::CUBOID);
}

void Cuboid::getIndicesSide(std::vector<glm::vec3>& vertices_out, std::vector<glm::vec3>& indices_out, int cuboidside) {
	getVertices(vertices_out);
	indices_out.resize(Cuboid::indices_count);
	if (TOP & cuboidside) {
		//GCDH
		indices_out[0] = vertices_out[CUBOID_G];
		indices_out[1] = vertices_out[CUBOID_C];
		indices_out[2] = vertices_out[CUBOID_D];
		indices_out[3] = vertices_out[CUBOID_H];
	}
	if (BOTTOM & cuboidside) {
		//BFEA
		indices_out[4] = vertices_out[CUBOID_B];
		indices_out[5] = vertices_out[CUBOID_F];
		indices_out[6] = vertices_out[CUBOID_E];
		indices_out[7] = vertices_out[CUBOID_A];
	}
	if (LEFT & cuboidside) {
		//GEAC
		indices_out[8] = vertices_out[CUBOID_G];
		indices_out[9] = vertices_out[CUBOID_E];
		indices_out[10] = vertices_out[CUBOID_A];
		indices_out[11] = vertices_out[CUBOID_C];
	}
	if (RIGHT & cuboidside) {
		//DBFH
		indices_out[12] = vertices_out[CUBOID_D];
		indices_out[13] = vertices_out[CUBOID_B];
		indices_out[14] = vertices_out[CUBOID_F];
		indices_out[15] = vertices_out[CUBOID_H];
	}
	if (FRONT & cuboidside) {
		//CABD
		indices_out[16] = vertices_out[CUBOID_C];
		indices_out[17] = vertices_out[CUBOID_A];
		indices_out[18] = vertices_out[CUBOID_B];
		indices_out[19] = vertices_out[CUBOID_D];
	}
	if (BACK & cuboidside) {
		//HFEG
		indices_out[20] = vertices_out[CUBOID_H];
		indices_out[21] = vertices_out[CUBOID_F];
		indices_out[22] = vertices_out[CUBOID_E];
		indices_out[23] = vertices_out[CUBOID_G];
	}
}

void Cuboid::getVertices(std::vector<glm::vec3>& vertices_out) const {
	vertices_out.resize(Cuboid::vertices_count);
	//Vertices centered
	vertices_out[CUBOID_A] = glm::vec3(position.x - (dimensions.x / 2.), position.y - (dimensions.y / 2.), position.z + (dimensions.z / 2.));		// A
	vertices_out[CUBOID_B] = glm::vec3(position.x + (dimensions.x / 2.), position.y - (dimensions.y / 2.), position.z + (dimensions.z / 2.));		// B
	vertices_out[CUBOID_C] = glm::vec3(position.x - (dimensions.x / 2.), position.y + (dimensions.y / 2.), position.z + (dimensions.z / 2.));		// C
	vertices_out[CUBOID_D] = glm::vec3(position.x + (dimensions.x / 2.), position.y + (dimensions.y / 2.), position.z + (dimensions.z / 2.));		// D
	vertices_out[CUBOID_E] = glm::vec3(position.x - (dimensions.x / 2.), position.y - (dimensions.y / 2.), position.z - (dimensions.z / 2.));		// E
	vertices_out[CUBOID_F] = glm::vec3(position.x + (dimensions.x / 2.), position.y - (dimensions.y / 2.), position.z - (dimensions.z / 2.));		// F
	vertices_out[CUBOID_G] = glm::vec3(position.x - (dimensions.x / 2.), position.y + (dimensions.y / 2.), position.z - (dimensions.z / 2.));		// G
	vertices_out[CUBOID_H] = glm::vec3(position.x + (dimensions.x / 2.), position.y + (dimensions.y / 2.), position.z - (dimensions.z / 2.));		// H
}


void Cuboid::getAllIndices(std::vector<glm::vec3>& vertices_out, std::vector<glm::vec3>& indices_out) const
{
	getVertices(vertices_out);
	indices_out.resize(Cuboid::indices_count);
	//CABD
	indices_out[0] = vertices_out[CUBOID_C];
	indices_out[1] = vertices_out[CUBOID_A];
	indices_out[2] = vertices_out[CUBOID_B];
	indices_out[3] = vertices_out[CUBOID_D];
	//DBFH
	indices_out[4] = vertices_out[CUBOID_D];
	indices_out[5] = vertices_out[CUBOID_B];
	indices_out[6] = vertices_out[CUBOID_F];
	indices_out[7] = vertices_out[CUBOID_H];
	//HFEG
	indices_out[8] = vertices_out[CUBOID_H];
	indices_out[9] = vertices_out[CUBOID_F];
	indices_out[10] = vertices_out[CUBOID_E];
	indices_out[11] = vertices_out[CUBOID_G];
	//GEAC 
	indices_out[12] = vertices_out[CUBOID_G];
	indices_out[13] = vertices_out[CUBOID_E];
	indices_out[14] = vertices_out[CUBOID_A];
	indices_out[15] = vertices_out[CUBOID_C];
	//EABF
	indices_out[16] = vertices_out[CUBOID_E];
	indices_out[17] = vertices_out[CUBOID_A];
	indices_out[18] = vertices_out[CUBOID_B];
	indices_out[19] = vertices_out[CUBOID_F];
	//GCDH
	indices_out[20] = vertices_out[CUBOID_G];
	indices_out[21] = vertices_out[CUBOID_C];
	indices_out[22] = vertices_out[CUBOID_D];
	indices_out[23] = vertices_out[CUBOID_H];
}



