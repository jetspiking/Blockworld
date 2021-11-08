#include "BlockWorld.h"
RandomNumberGenerator randomNumberGenerator;

bool placeBlockOnCurrentPos = false;
std::shared_ptr<std::thread> chunkThread;
bool threadShouldStop = false;
std::mutex mutex;

BlockWorld::BlockWorld(std::shared_ptr<std::vector<tigl::Vertex>> buffer, std::shared_ptr<std::vector<glm::vec3>> vertices_in, std::shared_ptr<std::vector<glm::vec3>> indices_in, GLFWwindow* windowPointer)
{
	this->buffer = buffer;
	this->vertices_in = vertices_in;
	this->indices_in = indices_in;

	this->renderDistanceWidth = 1;
	this->targetRenderDistanceWidth = 1;
	this->renderDistanceDepth = 1;
	this->targetRenderDistanceDepth = 1;

	this->player = std::make_shared<Player>(windowPointer);
	Initialise();
}

void BlockWorld::Initialise()
{
	chunkThread = std::make_shared<std::thread>(&BlockWorld::threadedLoad, this);
}

void BlockWorld::configureChunk(int chunkX, int chunkY, int chunkZ) {
	if (!this->world[chunkX][chunkY][chunkZ].isChunkConfigured) {
		this->world[chunkX][chunkY][chunkZ].chunk = std::make_shared<Chunk>();
		for (int chunkBlockX = 0; chunkBlockX < Chunk::CHUNK_SIZE_WIDTH; chunkBlockX++)
			for (int chunkBlockY = 0; chunkBlockY < Chunk::CHUNK_SIZE_HEIGHT; chunkBlockY++)
				for (int chunkBlockZ = 0; chunkBlockZ < Chunk::CHUNK_SIZE_DEPTH; chunkBlockZ++) {
					std::shared_ptr<Cuboid> cuboid = std::make_shared<Cuboid>(glm::vec3(Chunk::CHUNK_SIZE_WIDTH * chunkX * BLOCK_DIMENSIONS + (chunkBlockX * BLOCK_DIMENSIONS), Chunk::CHUNK_SIZE_HEIGHT * chunkY * BLOCK_DIMENSIONS + (chunkBlockY * BLOCK_DIMENSIONS), Chunk::CHUNK_SIZE_DEPTH * chunkZ * BLOCK_DIMENSIONS + (chunkBlockZ * BLOCK_DIMENSIONS)), glm::vec3(BLOCK_DIMENSIONS, BLOCK_DIMENSIONS, BLOCK_DIMENSIONS));
					std::shared_ptr<Block> block = std::make_shared<Block>(cuboid);
					block->initializeTextureUsage(glm::vec2(1, BLOCK_TYPE_AIR), glm::vec2(25, 22));
					this->world[chunkX][chunkY][chunkZ].chunk->blocks[chunkBlockX][chunkBlockY][chunkBlockZ] = block;
				}
		createSmoothTerrain(chunkX, chunkY, chunkZ, createChunkHeightMap(chunkX, chunkY, chunkZ));

		//Was added later
		for (int chunkBlockX = 0; chunkBlockX < Chunk::CHUNK_SIZE_WIDTH; chunkBlockX++)
			for (int chunkBlockY = 0; chunkBlockY < Chunk::CHUNK_SIZE_HEIGHT; chunkBlockY++)
				for (int chunkBlockZ = 0; chunkBlockZ < Chunk::CHUNK_SIZE_DEPTH; chunkBlockZ++) {
					std::shared_ptr<Block> block = this->world[chunkX][chunkY][chunkZ].chunk->blocks[chunkBlockX][chunkBlockY][chunkBlockZ];
					if (block->atlasID == BLOCK_TYPE_AIR)
						continue;
					decideBlockTerrain(block, chunkY, chunkBlockY);
				}
		this->world[chunkX][chunkY][chunkZ].isChunkConfigured = true;
	}
}

void BlockWorld::changeRenderDistance(int renderDistance) {
	this->targetRenderDistanceWidth = renderDistance;
	this->targetRenderDistanceDepth = renderDistance;
}

void BlockWorld::decideBlockTerrain(std::shared_ptr<Block> block, int chunkY, int chunkBlockY) {
	if (block->atlasID != BLOCK_TYPE_AIR) {

		int blockType = BLOCK_TYPE_AIR;

		int chunkYVar = Chunk::CHUNK_SIZE_HEIGHT * chunkY * BLOCK_DIMENSIONS + (chunkBlockY * BLOCK_DIMENSIONS);

		if (chunkYVar >= BLOCK_LEVEL_WATER_START && chunkYVar <= BLOCK_LEVEL_WATER_END) blockType = BLOCK_TYPE_WATER;
		else if (chunkYVar >= BLOCK_LEVEL_SAND_START && chunkYVar <= BLOCK_LEVEL_SAND_END) blockType = BLOCK_TYPE_SAND;
		else if (chunkYVar >= BLOCK_LEVEL_MUD_START && chunkYVar <= BLOCK_LEVEL_MUD_END) blockType = BLOCK_TYPE_MUD;
		else if (chunkYVar >= BLOCK_LEVEL_WETGRASS_START && chunkYVar <= BLOCK_LEVEL_WETGRASS_END) blockType = BLOCK_TYPE_WETGRASS;
		else blockType = BLOCK_TYPE_GRASS;

		if (blockType == BLOCK_TYPE_WATER) [[unlikely]]
			block->setColor(glm::vec4(.3f, .5f, 1.f, .7f));
		else [[likely]]
			block->setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));

		block->initializeTextureUsage(glm::vec2(1, blockType), glm::vec2(25, 22));
	}
}

int BlockWorld::getVisiblityDrawmask(std::shared_ptr<Block> block, int chunkX, int chunkY, int chunkZ, int chunkBlockX, int chunkBlockY, int chunkBlockZ) {
	int drawmask = 0;

	// If chunkblockx != 0, check left
	if (chunkBlockX != 0) {
		std::shared_ptr<Block> altBlock = this->world[chunkX][chunkY][chunkZ].chunk->blocks[chunkBlockX - 1][chunkBlockY][chunkBlockZ];
		if (altBlock->atlasID == BLOCK_TYPE_AIR) {
			drawmask |= Cuboid::CuboidSide::LEFT;
		}
	}
	// If chunkblockx is 0, check the most right block in the chunk to the left
	else /*if (chunkX != (int)(this->storedPlayerPosition.x - this->renderDistanceWidth))*/ {
		std::shared_ptr<Block> altBlock = this->world[chunkX - 1][chunkY][chunkZ].chunk->blocks[Chunk::CHUNK_SIZE_WIDTH - 1][chunkBlockY][chunkBlockZ];
		if (altBlock != nullptr) {
			if (altBlock->atlasID == BLOCK_TYPE_AIR) {
				drawmask |= Cuboid::CuboidSide::LEFT;
			}
		}
		else drawmask |= Cuboid::CuboidSide::LEFT;
	}

	// If chunkblocky != 0, check down
	if (chunkBlockY != 0) {
		std::shared_ptr<Block> altBlock = this->world[chunkX][chunkY][chunkZ].chunk->blocks[chunkBlockX][chunkBlockY - 1][chunkBlockZ];
		if (altBlock->atlasID == BLOCK_TYPE_AIR) {
			drawmask |= Cuboid::CuboidSide::BOTTOM;
		}
	}
	// If chunkblocky is 0, check the most upper block in the chunk below
	else if (chunkY != 0) {
		std::shared_ptr<Block> altBlock = this->world[chunkX][chunkY - 1][chunkZ].chunk->blocks[chunkBlockX][Chunk::CHUNK_SIZE_HEIGHT - 1][chunkBlockZ];
		if (altBlock != nullptr) {
			if (altBlock->atlasID == BLOCK_TYPE_AIR) {
				drawmask |= Cuboid::CuboidSide::BOTTOM;
			}
		}
		else drawmask |= Cuboid::CuboidSide::BOTTOM;
	}

	// If chunkblockz != 0, check back
	if (chunkBlockZ != 0) {
		std::shared_ptr<Block> altBlock = this->world[chunkX][chunkY][chunkZ].chunk->blocks[chunkBlockX][chunkBlockY][chunkBlockZ - 1];
		if (altBlock->atlasID == BLOCK_TYPE_AIR) {
			drawmask |= Cuboid::CuboidSide::BACK;
		}
	}
	// If chunkblockz is 0, check the closest block in the chunk to the back
	else /*if (chunkZ != (int)(this->storedPlayerPosition.z - this->renderDistanceDepth))*/ {
		std::shared_ptr<Block> altBlock = this->world[chunkX][chunkY][chunkZ - 1].chunk->blocks[chunkBlockX][chunkBlockY][Chunk::CHUNK_SIZE_DEPTH - 1];
		if (altBlock != nullptr) {
			if (altBlock->atlasID == BLOCK_TYPE_AIR) {
				drawmask |= Cuboid::CuboidSide::BACK;
			}
		}
		else drawmask |= Cuboid::CuboidSide::BACK;
	}

	// If chunkblockx != CHUNK_SIZE_WIDTH-1, check right
	if (chunkBlockX != Chunk::CHUNK_SIZE_WIDTH - 1) {
		std::shared_ptr<Block> altBlock = this->world[chunkX][chunkY][chunkZ].chunk->blocks[chunkBlockX + 1][chunkBlockY][chunkBlockZ];
		if (altBlock->atlasID == BLOCK_TYPE_AIR) {
			drawmask |= Cuboid::CuboidSide::RIGHT;
		}
	}
	// If chunkblockx is CHUNK_SIZE_WIDTH-1, check the most left block in the chunk to the right
	else /*if (chunkX != (int)(this->storedPlayerPosition.x + this->renderDistanceWidth))*/ {
		std::shared_ptr<Block> altBlock = this->world[chunkX + 1][chunkY][chunkZ].chunk->blocks[0][chunkBlockY][chunkBlockZ];
		if (altBlock != nullptr) {
			if (altBlock->atlasID == BLOCK_TYPE_AIR) {
				drawmask |= Cuboid::CuboidSide::RIGHT;
			}
		}
		else drawmask |= Cuboid::CuboidSide::RIGHT;
	}

	// If chunkblocky != CHUNK_SIZE_HEIGHT-1, check up
	if (chunkBlockY != Chunk::CHUNK_SIZE_HEIGHT - 1) {
		std::shared_ptr<Block> altBlock = this->world[chunkX][chunkY][chunkZ].chunk->blocks[chunkBlockX][chunkBlockY + 1][chunkBlockZ];
		if (altBlock->atlasID == BLOCK_TYPE_AIR || (altBlock->atlasID == BLOCK_TYPE_WATER && altBlock->drawmask > 0)) {
			drawmask |= Cuboid::CuboidSide::TOP;
		}
	}
	// If chunkblocky is CHUNK_SIZE_HEIGHT-1, check the lowest block in the chunk above
	else if (chunkY != BlockWorld::WORLD_SIZE_CHUNKS_HEIGHT - 1) {
		std::shared_ptr<Block> altBlock = this->world[chunkX][chunkY + 1][chunkZ].chunk->blocks[chunkBlockX][0][chunkBlockZ];
		if (altBlock != nullptr) {
			if (altBlock->atlasID == BLOCK_TYPE_AIR || (altBlock->atlasID == BLOCK_TYPE_WATER && altBlock->drawmask > 0)) {
				drawmask |= Cuboid::CuboidSide::TOP;
			}
		}
		else drawmask |= Cuboid::CuboidSide::TOP;
	}

	// If chunkblockz != CHUNK_SIZE_DEPTH-1, check front
	if (chunkBlockZ != Chunk::CHUNK_SIZE_DEPTH - 1) {
		std::shared_ptr<Block> altBlock = this->world[chunkX][chunkY][chunkZ].chunk->blocks[chunkBlockX][chunkBlockY][chunkBlockZ + 1];
		if (altBlock->atlasID == BLOCK_TYPE_AIR) {
			drawmask |= Cuboid::CuboidSide::FRONT;
		}
	}
	// If chunkblockz is CHUNK_SIZE_DEPTH-1, check the furthest block in the chunk to the front
	else /*if (chunkZ != (int)(this->storedPlayerPosition.z + this->renderDistanceDepth))*/ {
		std::shared_ptr<Block> altBlock = this->world[chunkX][chunkY][chunkZ + 1].chunk->blocks[chunkBlockX][chunkBlockY][0];
		if (altBlock != nullptr) {
			if (altBlock->atlasID == BLOCK_TYPE_AIR) {
				drawmask |= Cuboid::CuboidSide::FRONT;
			}
		}
		else drawmask |= Cuboid::CuboidSide::FRONT;
	}
	return drawmask;
}

void BlockWorld::setBlockVisibility(std::shared_ptr<Block> block, int chunkX, int chunkY, int chunkZ, int chunkBlockX, int chunkBlockY, int chunkBlockZ) {
	block->drawmask = getVisiblityDrawmask(block, chunkX, chunkY, chunkZ, chunkBlockX, chunkBlockY, chunkBlockZ);
}

void BlockWorld::createLiliesInChunk(int chunkX, int chunkY, int chunkZ) {
	int randomX = (int)(randomNumberGenerator.getRandom() * 100) % Chunk::CHUNK_SIZE_WIDTH;
	int randomZ = (int)(randomNumberGenerator.getRandom() * 100) % Chunk::CHUNK_SIZE_DEPTH;
	for (int chunkBlockY = Chunk::CHUNK_SIZE_HEIGHT - 1; chunkBlockY > -1; chunkBlockY--) {
		std::shared_ptr<Block> block = this->world[chunkX][chunkY][chunkZ].chunk->blocks[randomX][chunkBlockY][randomZ];
		if (block->atlasID == BLOCK_TYPE_WATER && getVisiblityDrawmask(block, chunkX, chunkY, chunkZ, randomX, chunkBlockY, randomZ) > 0) {
			createLilyOnPosition(chunkX, chunkY, chunkZ, randomX, chunkBlockY, randomZ);
		}
	}
}

void BlockWorld::createLilyOnPosition(int chunkX, int chunkY, int chunkZ, int chunkBlockX, int chunkBlockY, int chunkBlockZ) {
	if (chunkBlockY + 1 > Chunk::CHUNK_SIZE_HEIGHT - 1) return;
	std::shared_ptr<Block> block = this->world[chunkX][chunkY][chunkZ].chunk->blocks[chunkBlockX][BLOCK_LEVEL_WATER_END - 1][chunkBlockZ];
	if (block->atlasID == BLOCK_TYPE_AIR) {
		block->initializeTextureUsage(glm::vec2(1, BLOCK_TYPE_HEDGE), glm::vec2(25, 22));
		block->setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
		block->drawmask = Cuboid::TOP;
	}
}

void BlockWorld::createTreesInChunk(int chunkX, int chunkY, int chunkZ) {
	int randomX = (int)(randomNumberGenerator.getRandom() * 100) % Chunk::CHUNK_SIZE_WIDTH;
	int randomZ = (int)(randomNumberGenerator.getRandom() * 100) % Chunk::CHUNK_SIZE_DEPTH;
	for (int chunkBlockY = Chunk::CHUNK_SIZE_HEIGHT - 1; chunkBlockY > -1; chunkBlockY--) {
		std::shared_ptr<Block> block = this->world[chunkX][chunkY][chunkZ].chunk->blocks[randomX][chunkBlockY][randomZ];
		if (block->atlasID == BLOCK_TYPE_GRASS && getVisiblityDrawmask(block, chunkX, chunkY, chunkZ, randomX, chunkBlockY, randomZ) > 0) {
			createTreeOnPosition(chunkX, chunkY, chunkZ, randomX, chunkBlockY, randomZ);
		}
	}
}

void BlockWorld::createTreeOnPosition(int chunkX, int chunkY, int chunkZ, int chunkBlockX, int chunkBlockY, int chunkBlockZ) {
	int trunkHeight = -1;
	int randomTreeType = (int)(randomNumberGenerator.getRandom() * 100) % 3;
	int randomExtraHeight = (int)(randomNumberGenerator.getRandom() * 100) % 4 - 2;
	int treeTypes[3] = { BLOCK_TYPE_DARKWOOD, BLOCK_TYPE_BIRCHWOOD, BLOCK_TYPE_OAKWOOD };

	for (int treeHeight = 1; treeHeight < ENTITY_HEIGHT_TREE + 1 + randomExtraHeight; treeHeight++) {
		if (chunkBlockY + ENTITY_HEIGHT_TREE + randomExtraHeight + 4 > Chunk::CHUNK_SIZE_HEIGHT - 1)
		{
			trunkHeight = treeHeight;
			break;
		}
		else
			trunkHeight = treeHeight;
		std::shared_ptr<Block> block = this->world[chunkX][chunkY][chunkZ].chunk->blocks[chunkBlockX][chunkBlockY + treeHeight][chunkBlockZ];
		block->initializeTextureUsage(glm::vec2(1, treeTypes[randomTreeType]), glm::vec2(25, 22));
		block->setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
		block->drawmask = Cuboid::ALL;
	}
	int appleCount = 0;
	if (trunkHeight > 1) {
		for (int leaveX = -2; leaveX < 3; leaveX++)
			for (int leaveY = 1; leaveY < 6; leaveY++)
				for (int leaveZ = -2; leaveZ < 3; leaveZ++) {
					if (chunkBlockX + leaveX > -1 && chunkBlockX + leaveX < Chunk::CHUNK_SIZE_WIDTH)
						if (trunkHeight + leaveY + chunkBlockY > -1 && trunkHeight + leaveY + chunkBlockY < Chunk::CHUNK_SIZE_HEIGHT)
							if (chunkBlockZ + leaveZ > -1 && chunkBlockZ + leaveZ < Chunk::CHUNK_SIZE_DEPTH) {
								std::shared_ptr<Block> block = this->world[chunkX][chunkY][chunkZ].chunk->blocks[chunkBlockX + leaveX][chunkBlockY + trunkHeight + leaveY][chunkBlockZ + leaveZ];
								if (block->atlasID == BLOCK_TYPE_AIR) {

									if ((int)(randomNumberGenerator.getRandom() * 100) % 100 != 0 || appleCount >= 1 || treeTypes[randomTreeType] != BLOCK_TYPE_OAKWOOD) [[likely]] {
										block->initializeTextureUsage(glm::vec2(1, BLOCK_TYPE_LEAVES), glm::vec2(25, 22));
										block->setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
									}
									else [[unlikely]] {
										block->initializeTextureUsage(glm::vec2(1, BLOCK_TYPE_HEDGE), glm::vec2(25, 22));
										block->setColor(glm::vec4(1.5f, .0f, .0f, 1.f));
										appleCount++;
									}
									block->drawmask = Cuboid::ALL;
								}
							}
				}
	}
}

void BlockWorld::createBushesInChunk(int chunkX, int chunkY, int chunkZ) {
	int randomX = (int)(randomNumberGenerator.getRandom() * 100) % Chunk::CHUNK_SIZE_WIDTH;
	int randomZ = (int)(randomNumberGenerator.getRandom() * 100) % Chunk::CHUNK_SIZE_DEPTH;
	for (int chunkBlockY = Chunk::CHUNK_SIZE_HEIGHT - 1; chunkBlockY > -1; chunkBlockY--) {
		std::shared_ptr<Block> block = this->world[chunkX][chunkY][chunkZ].chunk->blocks[randomX][chunkBlockY][randomZ];
		if (block->atlasID == BLOCK_TYPE_GRASS && getVisiblityDrawmask(block, chunkX, chunkY, chunkZ, randomX, chunkBlockY, randomZ) > 0 > 0) {
			createBushOnPosition(chunkX, chunkY, chunkZ, randomX, chunkBlockY, randomZ);
		}
	}
}

void BlockWorld::createBushOnPosition(int chunkX, int chunkY, int chunkZ, int chunkBlockX, int chunkBlockY, int chunkBlockZ) {
	if (chunkBlockY + 1 > Chunk::CHUNK_SIZE_HEIGHT - 1) return;
	std::shared_ptr<Block> block = this->world[chunkX][chunkY][chunkZ].chunk->blocks[chunkBlockX][chunkBlockY + 1][chunkBlockZ];
	if (block->atlasID == BLOCK_TYPE_AIR) {
		block->initializeTextureUsage(glm::vec2(1, BLOCK_TYPE_HEDGE), glm::vec2(25, 22));
		block->setColor(glm::vec4(.7f, 1.f, 1.f, 1.f));
		block->drawmask = Cuboid::ALL;
	}
}

void BlockWorld::createHayInChunk(int chunkX, int chunkY, int chunkZ) {
	int randomX = (int)(randomNumberGenerator.getRandom() * 100) % Chunk::CHUNK_SIZE_WIDTH;
	int randomZ = (int)(randomNumberGenerator.getRandom() * 100) % Chunk::CHUNK_SIZE_DEPTH;
	for (int chunkBlockY = Chunk::CHUNK_SIZE_HEIGHT - 1; chunkBlockY > -1; chunkBlockY--) {
		std::shared_ptr<Block> block = this->world[chunkX][chunkY][chunkZ].chunk->blocks[randomX][chunkBlockY][randomZ];
		if (block->atlasID == BLOCK_TYPE_GRASS && getVisiblityDrawmask(block, chunkX, chunkY, chunkZ, randomX, chunkBlockY, randomZ) > 0) {
			createHayOnPosition(chunkX, chunkY, chunkZ, randomX, chunkBlockY, randomZ);
		}
	}
}

void BlockWorld::createHayOnPosition(int chunkX, int chunkY, int chunkZ, int chunkBlockX, int chunkBlockY, int chunkBlockZ) {
	if (chunkBlockY + 1 > Chunk::CHUNK_SIZE_HEIGHT - 1) return;
	std::shared_ptr<Block> block = this->world[chunkX][chunkY][chunkZ].chunk->blocks[chunkBlockX][chunkBlockY + 1][chunkBlockZ];
	if (block->atlasID == BLOCK_TYPE_AIR) {
		block->initializeTextureUsage(glm::vec2(1, BLOCK_TYPE_HAY), glm::vec2(25, 22));
		block->setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
		block->drawmask = Cuboid::ALL;
	}
}

void BlockWorld::createCactiInChunk(int chunkX, int chunkY, int chunkZ) {
	int randomX = (int)(randomNumberGenerator.getRandom() * 100) % Chunk::CHUNK_SIZE_WIDTH;
	int randomZ = (int)(randomNumberGenerator.getRandom() * 100) % Chunk::CHUNK_SIZE_DEPTH;
	for (int chunkBlockY = Chunk::CHUNK_SIZE_HEIGHT - 1; chunkBlockY > -1; chunkBlockY--) {
		std::shared_ptr<Block> block = this->world[chunkX][chunkY][chunkZ].chunk->blocks[randomX][chunkBlockY][randomZ];
		if (block->atlasID == BLOCK_TYPE_SAND && getVisiblityDrawmask(block, chunkX, chunkY, chunkZ, randomX, chunkBlockY, randomZ) > 0) [[unlikely]] {
			createCactusOnPosition(chunkX, chunkY, chunkZ, randomX, chunkBlockY, randomZ);
		}
	}
}

void BlockWorld::createCactusOnPosition(int chunkX, int chunkY, int chunkZ, int chunkBlockX, int chunkBlockY, int chunkBlockZ) {
	for (int cactusHeight = 1; cactusHeight < ENTITY_HEIGHT_CACTUS + 1; cactusHeight++) {
		if (chunkBlockY + ENTITY_HEIGHT_CACTUS > Chunk::CHUNK_SIZE_HEIGHT - 1) return;
		std::shared_ptr<Block> block = this->world[chunkX][chunkY][chunkZ].chunk->blocks[chunkBlockX][chunkBlockY + cactusHeight][chunkBlockZ];
		block->initializeTextureUsage(glm::vec2(1, BLOCK_TYPE_HEDGE), glm::vec2(25, 22));
		block->setColor(glm::vec4(.3f, 1.f, .3f, 1.f));
		block->drawmask = Cuboid::ALL;
	}
}

void BlockWorld::renderChunk(int chunkX, int chunkY, int chunkZ) {
	if (!this->world[chunkX][chunkY][chunkZ].isChunkRendered && this->world[chunkX][chunkY][chunkZ].isChunkConfigured && this->world[chunkX][chunkY][chunkZ].isChunkLoaded) {

		this->world[chunkX][chunkY][chunkZ].buffer = std::make_shared<std::vector<tigl::Vertex>>();
		this->world[chunkX][chunkY][chunkZ].vertices_in = std::make_shared<std::vector<glm::vec3>>();
		this->world[chunkX][chunkY][chunkZ].indices_in = std::make_shared<std::vector<glm::vec3>>();

		for (int chunkBlockX = 0; chunkBlockX < Chunk::CHUNK_SIZE_WIDTH; chunkBlockX++)
			for (int chunkBlockY = 0; chunkBlockY < Chunk::CHUNK_SIZE_HEIGHT; chunkBlockY++)
				for (int chunkBlockZ = 0; chunkBlockZ < Chunk::CHUNK_SIZE_DEPTH; chunkBlockZ++) {
					if (this->world[chunkX][chunkY][chunkZ].chunk->blocks[chunkBlockX][chunkBlockY][chunkBlockZ]) {
						std::shared_ptr<Block> block = this->world[chunkX][chunkY][chunkZ].chunk->blocks[chunkBlockX][chunkBlockY][chunkBlockZ];

						if (block->drawmask != 0 && block->atlasID != BLOCK_TYPE_AIR) {
							block->getSpecificationVertices(this->world[chunkX][chunkY][chunkZ].buffer, this->world[chunkX][chunkY][chunkZ].vertices_in, this->world[chunkX][chunkY][chunkZ].indices_in, block->drawmask);
						}
					}
				}
		this->world[chunkX][chunkY][chunkZ].isChunkRendered = true;
	}
}

void BlockWorld::fillChunkVBO(int chunkX, int chunkY, int chunkZ) {
	if (!this->world[chunkX][chunkY][chunkZ].isAddedToVBO) {
		this->world[chunkX][chunkY][chunkZ].vertexBufferObject = tigl::createVbo(*this->world[chunkX][chunkY][chunkZ].buffer);
		this->world[chunkX][chunkY][chunkZ].buffer->empty();
		this->world[chunkX][chunkY][chunkZ].vertices_in->empty();
		this->world[chunkX][chunkY][chunkZ].indices_in->empty();
		this->world[chunkX][chunkY][chunkZ].isAddedToVBO = true;
	}
}

void BlockWorld::unrenderChunk(int chunkX, int chunkY, int chunkZ) {
	if (this->world[chunkX][chunkY][chunkZ].isChunkRendered) {
		// Cleanup chunk VBO code here
		// ...
		// End of cleanup VBO code

		this->world[chunkX][chunkY][chunkZ].isChunkRendered = false;
	}
}

void BlockWorld::loadChunk(int chunkX, int chunkY, int chunkZ) {
	if (this->world[chunkX][chunkY][chunkZ].chunk->blocks) {
		for (int chunkBlockX = 0; chunkBlockX < Chunk::CHUNK_SIZE_WIDTH; chunkBlockX++)
			for (int chunkBlockY = 0; chunkBlockY < Chunk::CHUNK_SIZE_HEIGHT; chunkBlockY++)
				for (int chunkBlockZ = 0; chunkBlockZ < Chunk::CHUNK_SIZE_DEPTH; chunkBlockZ++) {
					std::shared_ptr<Block> block = this->world[chunkX][chunkY][chunkZ].chunk->blocks[chunkBlockX][chunkBlockY][chunkBlockZ];
					if (block->atlasID == BLOCK_TYPE_AIR)
						continue;
					//decideBlockTerrain(block, chunkY, chunkBlockY);
					setBlockVisibility(block, chunkX, chunkY, chunkZ, chunkBlockX, chunkBlockY, chunkBlockZ);
				}
		this->world[chunkX][chunkY][chunkZ].isChunkLoaded = true;
	}
}

std::array<int, Chunk::CHUNK_SIZE_DEPTH* Chunk::CHUNK_SIZE_WIDTH> BlockWorld::createChunkHeightMap(int chunkX, int chunkY, int chunkZ) {
	std::array<int, Chunk::CHUNK_SIZE_DEPTH* Chunk::CHUNK_SIZE_WIDTH> heightMap;
	for (int z = 0; z < Chunk::CHUNK_SIZE_DEPTH; z++)
		for (int x = 0; x < Chunk::CHUNK_SIZE_WIDTH; x++) {
			int voxelX = x + chunkX * Chunk::CHUNK_SIZE_WIDTH;
			int voxelZ = z + chunkZ * Chunk::CHUNK_SIZE_DEPTH;
			float value = glm::simplex(glm::vec2{ voxelX / 200.f, voxelZ / 200.f });
			value = (value + 1) / 2;
			value *= Chunk::CHUNK_SIZE_HEIGHT * this->WORLD_SIZE_CHUNKS_HEIGHT * BLOCK_DIMENSIONS;
			heightMap[z * Chunk::CHUNK_SIZE_DEPTH + x] = static_cast<int>(value);
		}
	return heightMap;
}

void BlockWorld::createFlora(int chunkX, int chunkY, int chunkZ) {
	createCactiInChunk(chunkX, chunkY, chunkZ);
	createTreesInChunk(chunkX, chunkY, chunkZ);
	createLiliesInChunk(chunkX, chunkY, chunkZ);
	createBushesInChunk(chunkX, chunkY, chunkZ);
	createHayInChunk(chunkX, chunkY, chunkZ);
}

void BlockWorld::createSmoothTerrain(int chunkX, int chunkY, int chunkZ, const std::array<int, Chunk::CHUNK_SIZE_DEPTH* Chunk::CHUNK_SIZE_WIDTH>& heightMap) {
	for (int z = 0; z < Chunk::CHUNK_SIZE_DEPTH; z++)
		for (int x = 0; x < Chunk::CHUNK_SIZE_WIDTH; x++) {
			int height = heightMap[z * Chunk::CHUNK_SIZE_DEPTH + x];
			for (int y = 0; y < Chunk::CHUNK_SIZE_HEIGHT; y++) {
				int blockY = chunkY * Chunk::CHUNK_SIZE_HEIGHT + y;
				if (blockY <= height) {
					std::shared_ptr<Block> block = this->world[chunkX][chunkY][chunkZ].chunk->blocks[x][y][z];
					block->initializeTextureUsage(glm::vec2(1, BLOCK_TYPE_GRASS), glm::vec2(25, 22));
				}
			}
		}

}
void BlockWorld::draw() {
	this->storedPlayerPosition = this->player->getPlayerChunkLocation(Chunk::CHUNK_SIZE_WIDTH, Chunk::CHUNK_SIZE_HEIGHT, Chunk::CHUNK_SIZE_DEPTH) * -1.f;

	for (int chunkX = ((int)this->storedPlayerPosition.x) - this->renderDistanceWidth; chunkX <= ((int)this->storedPlayerPosition.x) + this->renderDistanceWidth; chunkX++)
		for (int chunkY = 0; chunkY < this->WORLD_SIZE_CHUNKS_HEIGHT; chunkY++)
			for (int chunkZ = ((int)this->storedPlayerPosition.z) - this->renderDistanceDepth; chunkZ <= ((int)this->storedPlayerPosition.z) + this->renderDistanceDepth; chunkZ++) {
				if (this->world[chunkX][chunkY].contains(chunkZ))
					if (this->world[chunkX][chunkY][chunkZ].isAddedToVBO)
						tigl::drawVertices(GL_QUADS, this->world[chunkX][chunkY][chunkZ].vertexBufferObject);

					else if (this->world[chunkX][chunkY][chunkZ].isChunkRendered) {
						fillChunkVBO(chunkX, chunkY, chunkZ);
					}
			}
	createSun();
}

void BlockWorld::threadedChunkBuild(int chunkX, int chunkY, int chunkZ) {
	if (!this->world[chunkX][chunkY][chunkZ].isChunkRendered && !this->world[chunkX][chunkY][chunkZ].isAddedToVBO) {
		dynamicallyCreateChunk(chunkX, chunkY, chunkZ);
	}
}


void BlockWorld::threadedLoad() {
	while (1) {
		if (this->targetRenderDistanceWidth != this->renderDistanceWidth || this->targetRenderDistanceDepth != this->renderDistanceDepth) {
			for (const auto& [keyX, x] : this->world)
				for (const auto& [keyY, y] : x)
					for (const auto& [keyZ, z] : y)
						this->world[keyX][keyY].erase(keyZ);
			this->renderDistanceWidth = this->targetRenderDistanceWidth;
			this->renderDistanceDepth = this->targetRenderDistanceDepth;
		}

		for (int chunkX = ((int)this->storedPlayerPosition.x) - this->renderDistanceWidth; chunkX <= ((int)this->storedPlayerPosition.x) + this->renderDistanceWidth; chunkX++)
			for (int chunkY = 0; chunkY < this->WORLD_SIZE_CHUNKS_HEIGHT; chunkY++)
				for (int chunkZ = ((int)this->storedPlayerPosition.z) - this->renderDistanceDepth; chunkZ <= ((int)this->storedPlayerPosition.z) + this->renderDistanceDepth; chunkZ++) {
					threadedChunkBuild(chunkX, chunkY, chunkZ);
					threadedChunkBuild(chunkX - 1, chunkY, chunkZ);
					threadedChunkBuild(chunkX + 1, chunkY, chunkZ);
					threadedChunkBuild(chunkX, chunkY - 1, chunkZ);
					threadedChunkBuild(chunkX, chunkY + 1, chunkZ);
					threadedChunkBuild(chunkX, chunkY, chunkZ - 1);
					threadedChunkBuild(chunkX, chunkY, chunkZ + 1);
				}

		for (const auto& [keyX, x] : this->world)
			for (const auto& [keyY, y] : x)
				for (const auto& [keyZ, z] : y) {
					bool canDelete = true;
					for (int chunkX = ((int)this->storedPlayerPosition.x) - this->renderDistanceWidth; chunkX <= ((int)this->storedPlayerPosition.x) + this->renderDistanceWidth; chunkX++)
						for (int chunkY = 0; chunkY < this->WORLD_SIZE_CHUNKS_HEIGHT; chunkY++)
							for (int chunkZ = ((int)this->storedPlayerPosition.z) - this->renderDistanceDepth; chunkZ <= ((int)this->storedPlayerPosition.z) + this->renderDistanceDepth; chunkZ++) {
								if (chunkX == keyX && chunkY == keyY && chunkZ == keyZ)
									canDelete = false;
							}
					if (canDelete) {
						this->world[keyX][keyY].erase(keyZ);
					}
				}
		threadedPlaceBlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

	}
	std::cout << "Thread has stopped" << std::endl;
}

void BlockWorld::eraseMapEntry(int chunkX, int chunkY, int chunkZ) {
	this->world[chunkX][chunkY][chunkZ].buffer->clear();
	this->world[chunkX][chunkY][chunkZ].vertices_in->clear();
	this->world[chunkX][chunkY][chunkZ].indices_in->clear();
	this->world[chunkX][chunkY][chunkZ].isChunkConfigured = false;
	this->world[chunkX][chunkY][chunkZ].isChunkLoaded = false;
	this->world[chunkX][chunkY][chunkZ].isChunkRendered = false;
	this->world[chunkX][chunkY][chunkZ].isAddedToVBO = false;
}

bool BlockWorld::areSurroundingChunksLoaded(int chunkX, int chunkY, int chunkZ) {
	if (!this->world[chunkX - 1][chunkY][chunkZ].isChunkLoaded && !this->world[chunkX - 1][chunkY][chunkZ].isChunkRendered && !this->world[chunkX - 1][chunkY][chunkZ].isAddedToVBO) {
		return false;
	}
	if (!this->world[chunkX + 1][chunkY][chunkZ].isChunkLoaded && !this->world[chunkX + 1][chunkY][chunkZ].isChunkRendered && !this->world[chunkX + 1][chunkY][chunkZ].isAddedToVBO) {
		return false;
	}
	if (!this->world[chunkX][chunkY - 1][chunkZ].isChunkLoaded && !this->world[chunkX][chunkY - 1][chunkZ].isChunkRendered && !this->world[chunkX][chunkY - 1][chunkZ].isAddedToVBO) {
		return false;
	}
	if (!this->world[chunkX][chunkY + 1][chunkZ].isChunkLoaded && !this->world[chunkX][chunkY + 1][chunkZ].isChunkRendered && !this->world[chunkX][chunkY + 1][chunkZ].isAddedToVBO) {
		return false;
	}
	if (!this->world[chunkX][chunkY][chunkZ - 1].isChunkLoaded && !this->world[chunkX][chunkY][chunkZ - 1].isChunkRendered && !this->world[chunkX][chunkY][chunkZ - 1].isAddedToVBO) {
		return false;
	}
	if (!this->world[chunkX][chunkY][chunkZ + 1].isChunkLoaded && !this->world[chunkX][chunkY][chunkZ + 1].isChunkRendered && !this->world[chunkX][chunkY][chunkZ + 1].isAddedToVBO) {
		return false;
	}
	return true;
}

void BlockWorld::dynamicallyCreateChunk(int chunkX, int chunkY, int chunkZ) {
	if (!this->world[chunkX - 1][chunkY][chunkZ].isChunkConfigured) {
		configureChunk(chunkX - 1, chunkY, chunkZ);
	}
	if (!this->world[chunkX + 1][chunkY][chunkZ].isChunkConfigured) {
		configureChunk(chunkX + 1, chunkY, chunkZ);
	}
	if (!this->world[chunkX][chunkY - 1][chunkZ].isChunkConfigured) {
		configureChunk(chunkX, chunkY - 1, chunkZ);
	}
	if (!this->world[chunkX][chunkY + 1][chunkZ].isChunkConfigured) {
		configureChunk(chunkX, chunkY + 1, chunkZ);
	}
	if (!this->world[chunkX][chunkY][chunkZ - 1].isChunkConfigured) {
		configureChunk(chunkX, chunkY, chunkZ - 1);
	}
	if (!this->world[chunkX][chunkY][chunkZ + 1].isChunkConfigured) {
		configureChunk(chunkX, chunkY, chunkZ + 1);
	}

	configureChunk(chunkX, chunkY, chunkZ);
	loadChunk(chunkX, chunkY, chunkZ);
	createFlora(chunkX, chunkY, chunkZ);
	renderChunk(chunkX, chunkY, chunkZ);
}
#include "glm/gtx/string_cast.hpp"

void BlockWorld::threadedPlaceBlock() {
	if (placeBlockOnCurrentPos) {
		//TO DO	
	}
}

void BlockWorld::placeBlock(bool placeBlock)
{
	placeBlockOnCurrentPos = true;
}

void BlockWorld::createSun() {
	glm::vec2 atlas_coords = glm::vec2(1, BLOCK_TYPE_SUN);
	glm::vec2 atlas_dimensions = glm::vec2(25, 22);
	glm::vec2 textureCoordinates[4]{
	glm::vec2((double)(atlas_coords.x - 1) / atlas_dimensions.x, (double)atlas_coords.y / atlas_dimensions.y),
	glm::vec2((double)(atlas_coords.x - 1) / atlas_dimensions.x, (double)(atlas_coords.y - 1) / atlas_dimensions.y),
	glm::vec2((double)atlas_coords.x / atlas_dimensions.x, (double)(atlas_coords.y - 1) / atlas_dimensions.y),
	glm::vec2((double)atlas_coords.x / atlas_dimensions.x, (double)atlas_coords.y / atlas_dimensions.y)
	};
	int sunSize = 5000;

	glm::vec3 sunPos = getSunPosition();
	tigl::Vertex vertex1 = tigl::Vertex::PTC(glm::vec3(sunPos.x + (sunSize / 2.), sunPos.y - (sunSize / 2.), sunPos.z), glm::vec2(textureCoordinates[0]), glm::vec4(1.f, 1.f, 1.f, 1.f));
	tigl::Vertex vertex2 = tigl::Vertex::PTC(glm::vec3(sunPos.x + (sunSize / 2.), sunPos.y + (sunSize / 2.), sunPos.z), glm::vec2(textureCoordinates[1]), glm::vec4(1.f, 1.f, 1.f, 1.f));
	tigl::Vertex vertex3 = tigl::Vertex::PTC(glm::vec3(sunPos.x - (sunSize / 2.), sunPos.y + (sunSize / 2.), sunPos.z), glm::vec2(textureCoordinates[2]), glm::vec4(1.f, 1.f, 1.f, 1.f));
	tigl::Vertex vertex4 = tigl::Vertex::PTC(glm::vec3(sunPos.x - (sunSize / 2.), sunPos.y - (sunSize / 2.), sunPos.z), glm::vec2(textureCoordinates[3]), glm::vec4(1.f, 1.f, 1.f, 1.f));

	std::vector<tigl::Vertex> vertices = {
		vertex1,
		vertex2,
		vertex3,
		vertex4
	};

	tigl::drawVertices(GL_QUADS, vertices);
}

std::shared_ptr<Player> BlockWorld::getPlayer() const noexcept
{
	return this->player;
}

glm::vec3 BlockWorld::getSunPosition() const {
	return glm::vec3(this->storedPlayerPosition.x * Chunk::CHUNK_SIZE_WIDTH / 2.f, WORLD_SIZE_CHUNKS_HEIGHT * Chunk::CHUNK_SIZE_HEIGHT * 15.f, 20000.f);
}

glm::vec3 BlockWorld::getWorldCenter() const {
	return glm::vec3((this->storedPlayerPosition.x * Chunk::CHUNK_SIZE_WIDTH) / 2.f, (WORLD_SIZE_CHUNKS_HEIGHT * Chunk::CHUNK_SIZE_HEIGHT) / 2., (this->storedPlayerPosition.z * Chunk::CHUNK_SIZE_DEPTH) / 2.);
}

glm::vec3 BlockWorld::getSurfaceCenterPosition() const {
	return glm::vec3((this->storedPlayerPosition.x * Chunk::CHUNK_SIZE_WIDTH) / 2.f, (WORLD_SIZE_CHUNKS_HEIGHT * Chunk::CHUNK_SIZE_HEIGHT), (this->storedPlayerPosition.z * Chunk::CHUNK_SIZE_DEPTH) / 2.);

}