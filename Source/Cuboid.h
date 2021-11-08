#pragma once
#include "Shape.h"

// CUBOID VERTEX A
#define CUBOID_A 0
// CUBOID VERTEX B
#define CUBOID_B 1
// CUBOID VERTEX C
#define CUBOID_C 2
// CUBOID VERTEX D
#define CUBOID_D 3
// CUBOID VERTEX E
#define CUBOID_E 4
// CUBOID VERTEX F
#define CUBOID_F 5
// CUBOID VERTEX G
#define CUBOID_G 6
// CUBOID VERTEX H
#define CUBOID_H 7

// Class Cuboid is child class of Shape
class Cuboid : public Shape
{
public:
	// CuboidSide mask to specify which vertices you want to receive. All is added for a quick select option.
	typedef enum CuboidSide {
		TOP = 1,
		BOTTOM = 2,
		LEFT = 4,
		RIGHT = 8,
		FRONT = 16,
		BACK = 32,
		ALL = 63
	} CuboidSide;
	// Cuboid constructor, needs a specific position and a dimension.
	Cuboid(glm::vec3 position, glm::vec3 dimensions);
	// Get the indices of a specific side, vertices_out and indices_out are used as an (out) buffer.
	void getIndicesSide(std::vector<glm::vec3>& vertices_out, std::vector<glm::vec3>& indices_out, int cuboidside);
	// Get all vertices, vertices_out is used as an (out) buffer.
	void getVertices(std::vector<glm::vec3>& vertices_out) const;
	// Get all indices, vertices_out and indices_out are used as an (out) buffer.
	void getAllIndices(std::vector<glm::vec3>& vertices_out, std::vector<glm::vec3>& indices_out) const override;
	// Amount of vertices a cuboid has are 8.
	const static int vertices_count = 8;
	// Amount of indices a cuboid has are 24.
	const static int indices_count = 24; 
};

