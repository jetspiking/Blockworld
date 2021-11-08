#include "Prism.h"

//						 /\ F
//						/  \
//					   /    \
//					  /		 \
//					 /		  \
//					/\ C	   \
//				   /  \		    \
//				D /+---\--------+ E
//				 /' `   \      '
//			    /'	 `	 \	  '
//			   /'      `  \  '
//			  /'         ` \'
//		   A +-------------+ B    

Prism::Prism(glm::vec3 position, glm::vec3 dimensions) : Shape::Shape(position, dimensions)
{
	this->set_shape_type(Shape::PRISM);
}

void Prism::getAllIndices(std::vector<glm::vec3>& vertices_out, std::vector<glm::vec3>& indices_out) const
{
	vertices_out.resize(Prism::vertices_count);
	//Vertices
	vertices_out[PRISM_A] = glm::vec3(position.x - (dimensions.x / 2.), position.y - (dimensions.y / 2.), position.z + (dimensions.z / 2.));	// A
	vertices_out[PRISM_B] = glm::vec3(position.x + (dimensions.x / 2.), position.y - (dimensions.y / 2.), position.z + (dimensions.z / 2.));	// B
	vertices_out[PRISM_C] = glm::vec3(position.x + (dimensions.x / 4.), position.y + (dimensions.y / 2.), position.z + (dimensions.z / 2.));	// C
	vertices_out[PRISM_D] = glm::vec3(position.x - (dimensions.x / 2.), position.y - (dimensions.y / 2.), position.z - (dimensions.z / 2.));	// D
	vertices_out[PRISM_E] = glm::vec3(position.x + (dimensions.x / 2.), position.y - (dimensions.y / 2.), position.z - (dimensions.z / 2.));	// E
	vertices_out[PRISM_F] = glm::vec3(position.x + (dimensions.x / 4.), position.y + (dimensions.y / 2.), position.z - (dimensions.z / 2.));	// F

	indices_out.resize(Prism::indices_count);
	//ABC
	indices_out[0] = vertices_out[PRISM_A];
	indices_out[1] = vertices_out[PRISM_B];
	indices_out[2] = vertices_out[PRISM_C];
	//DEF
	indices_out[3] = vertices_out[PRISM_D];
	indices_out[4] = vertices_out[PRISM_E];
	indices_out[5] = vertices_out[PRISM_F];
	//ABD
	indices_out[6] = vertices_out[PRISM_A];
	indices_out[7] = vertices_out[PRISM_B];
	indices_out[8] = vertices_out[PRISM_D];
	//EDB
	indices_out[9] = vertices_out[PRISM_E];
	indices_out[10] = vertices_out[PRISM_D];
	indices_out[11] = vertices_out[PRISM_B];
	//BEC
	indices_out[12] = vertices_out[PRISM_B];
	indices_out[13] = vertices_out[PRISM_E];
	indices_out[14] = vertices_out[PRISM_C];
	//ECF
	indices_out[15] = vertices_out[PRISM_E];
	indices_out[16] = vertices_out[PRISM_C];
	indices_out[17] = vertices_out[PRISM_F];
	//ADC
	indices_out[18] = vertices_out[PRISM_A];
	indices_out[19] = vertices_out[PRISM_D];
	indices_out[20] = vertices_out[PRISM_C];
	//DCF
	indices_out[21] = vertices_out[PRISM_D];
	indices_out[22] = vertices_out[PRISM_C];
	indices_out[23] = vertices_out[PRISM_F];
}