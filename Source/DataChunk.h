#pragma once
#include <memory>
#include "tigl.h"
#include "Chunk.h"

class DataChunk
{
public: 
	// If all blocks are created in the chunk
	bool isChunkLoaded = false;		
	// If all blocks contain an evaluated drawmask
	bool isChunkConfigured = false;
	// If all blocks are added to the buffer
	bool isChunkRendered = false;
	// If all blocks are added to the VBO
	bool isAddedToVBO = false;
	// Pointer to a vertex buffer object, data can be stored on the graphics card to prevent obsolete calls from the CPU to the GPU.
	std::shared_ptr<tigl::VBO> vertexBufferObject;
	// Pointer to a chunk object, this is a pointer so the chunk can be removed easily.
	std::shared_ptr<Chunk> chunk;
	// Vector that functions as buffer in which blocks can be stored.
	std::shared_ptr<std::vector<tigl::Vertex>> buffer;
	// Vector that functions as buffer in which vertices can be stored.
	std::shared_ptr<std::vector<glm::vec3>> vertices_in;
	// Vector that functions as buffer in which indices can be stored.
	std::shared_ptr<std::vector<glm::vec3>> indices_in;
};

