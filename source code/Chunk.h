#pragma once
#include <map>
#include <mutex>
#include  "tigl.h"
#include "BlockAtlas.h"
#include "Block.h"

class Chunk
{
public:
	// Every chunk counts 16 blocks in width, can not be changed runtime.
	static const int CHUNK_SIZE_WIDTH = 16;
	// Every chunk counts 16 blocks in height, can not be changed runtime.
	static const int CHUNK_SIZE_HEIGHT = 16;
	// Every chunk counts 16 blocks in depth, can not be changed runtime.
	static const int CHUNK_SIZE_DEPTH = 16;
	// Array to place chunk blocks in.
	std::shared_ptr<Block> blocks[CHUNK_SIZE_WIDTH][CHUNK_SIZE_HEIGHT][CHUNK_SIZE_DEPTH];
};

