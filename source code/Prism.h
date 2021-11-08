#pragma once
#include "Shape.h"

#define PRISM_A 0
#define PRISM_B 1
#define PRISM_C 2
#define PRISM_D 3
#define PRISM_E 4
#define PRISM_F 5

class Prism : public Shape
{
public:
	Prism(glm::vec3 position, glm::vec3 dimensions);
	void getAllIndices(std::vector<glm::vec3>& vertices_out, std::vector<glm::vec3>& indices_out) const override;

	const static int vertices_count = 6;
	const static int indices_count = 24;
};

