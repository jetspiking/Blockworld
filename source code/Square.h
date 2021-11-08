#pragma once
#include "Shape.h"

#define SQUARE_A 0
#define SQUARE_B 1
#define SQUARE_C 2
#define SQUARE_D 3

class Square :public Shape
{
public:
	Square(glm::vec3 position, glm::vec3 dimensions);
	void getVertices(std::vector<glm::vec3>& vertices_out) const;
	void getAllIndices(std::vector<glm::vec3>& vertices_out, std::vector<glm::vec3>& indices_out) const override;
	
	const static int vertices_count = 4;
	const static int indices_count = 4;
};

