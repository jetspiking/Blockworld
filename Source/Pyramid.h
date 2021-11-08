#pragma once
#include "Shape.h"

#define PYRAMID_A 0
#define PYRAMID_B 1
#define PYRAMID_C 2
#define PYRAMID_D 3
#define PYRAMID_E 4

class Pyramid : public Shape
{
public:
	Pyramid(glm::vec3 position, glm::vec3 dimensions);
	void getAllIndices(std::vector<glm::vec3>& vertices_out, std::vector<glm::vec3>& indices_out) const override;

	const static int vertices_count = 5;
	const static int indices_count = 18;
};

