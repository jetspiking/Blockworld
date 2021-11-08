#pragma once
#include <array>
#include <thread>
#include <future>
#include <glm/glm.hpp>

#include "tigl.h"
#include "Chunk.h"
#include "DataChunk.h"
#include "Block.h"
#include "Player.h"
#include "FpsCam.h"
#include "RandomNumberGenerator.h"
#include "glm/gtc/noise.hpp"

// Block size in width, height and depth is 1.
#define BLOCK_DIMENSIONS 1

// BlockWorld generates an entire map with player.
class BlockWorld
{
public:
	// Blockworld max height for terrain generation.
	static const int WORLD_SIZE_CHUNKS_HEIGHT =6;
	// Render distance and target render distance to decide how many chunks should be loaded around the players x position.
	int renderDistanceWidth, targetRenderDistanceWidth;
	// Render distance and target render distance to decide how many chunks should be loaded around the players z position.
	int renderDistanceDepth, targetRenderDistanceDepth;
	// BlockWorld constructor, gets parsed a buffer, vertex and indice array which can be used as a buffer.
	BlockWorld(std::shared_ptr<std::vector<tigl::Vertex>> buffer, std::shared_ptr<std::vector<glm::vec3>> vertices_in, std::shared_ptr<std::vector<glm::vec3>> indices_in, GLFWwindow* windowPointer);
	// Initialise Blockworld.
	void Initialise();
	// Draw Blockworld.
	void draw();
	// Get the player pointer.
	std::shared_ptr<Player> getPlayer() const noexcept;
	// Get the sun position.
	glm::vec3 getSunPosition() const;
	// Get the center of the world (modified player position).
	glm::vec3 getWorldCenter() const;
	// Get the surfaceCenterPosition, has a height based on the world height variable defined and described earlier.
	glm::vec3 getSurfaceCenterPosition() const;
	// Map to store chunks in.
	std::map<int, std::map<int, std::map<int, DataChunk>>> world;
	// Vec3 for a saved player position.
	glm::vec3 storedPlayerPosition = glm::vec3(0,0,0);
	// Function for adjusting the render distance.
	void changeRenderDistance(int renderDistance);
	// Function for placing blocks in a thread.
	void threadedPlaceBlock();
	// Function to place a block. WARNING, not implemented yet!
	void placeBlock(bool placeBlock);

private:
	// Configure a chunk, initialize the chunk and make sure we won't write in random memory.
	void configureChunk(int chunkX, int chunkY, int chunkZ);
	// Decide the block terrain based on the height of a specific block in the chunk.
	void decideBlockTerrain(std::shared_ptr<Block> block, int chunkY, int chunkBlockY);
	// Get the visibility drawmask of a block.
	int getVisiblityDrawmask(std::shared_ptr<Block> block, int chunkX, int chunkY, int chunkZ, int chunkBlockX, int chunkBlockY, int chunkBlockZ);
	// Set the block visiblity for a specific block based on whether surrounding blocks are respectively transparent or not.
	void setBlockVisibility(std::shared_ptr<Block> block, int chunkX, int chunkY, int chunkZ, int chunkBlockX, int chunkBlockY, int chunkBlockZ);
	// Function to build chunks, should be run from a thread.
	void threadedChunkBuild(int chunkX, int chunkY, int chunkZ);
	// Function to load the chunks, should be started from a thread.
	void threadedLoad();
	// Erase a specific map entry to free some memory.
	void eraseMapEntry(int chunkX, int chunkY, int chunkZ);
	// Check whether the surrounding chunks are loaded.
	bool areSurroundingChunksLoaded(int chunkX, int chunkY, int chunkZ);
	// Create a chunk and perform error handling and checking if surrounding chunks should also be generated first.
	void dynamicallyCreateChunk(int chunkX, int chunkY, int chunkZ);
	// Randomly create lilies in a chunk.
	void createLiliesInChunk(int chunkX, int chunkY, int chunkZ);
	// Build a lily on a specific position.
	void createLilyOnPosition(int chunkX, int chunkY, int chunkZ, int chunkBlockX, int chunkBlockY, int chunkBlockZ);
	// Randomly create different types of trees in a chunk.
	void createTreesInChunk(int chunkX, int chunkY, int chunkZ);
	// Build a tree on a specific position.
	void createTreeOnPosition(int chunkX, int chunkY, int chunkZ, int chunkBlockX, int chunkBlockY, int chunkBlockZ);
	// Randomly create bushes in a chunk.
	void createBushesInChunk(int chunkX, int chunkY, int chunkZ);
	// Build a bush on a specific position.
	void createBushOnPosition(int chunkX, int chunkY, int chunkZ, int chunkBlockX, int chunkBlockY, int chunkBlockZ);
	// Randomly create hay in a chunk.
	void createHayInChunk(int chunkX, int chunkY, int chunkZ);
	// Build hay on a specific position.
	void createHayOnPosition(int chunkX, int chunkY, int chunkZ, int chunkBlockX, int chunkBlockY, int chunkBlockZ);
	// Randomly create cacti in a chunk.
	void createCactiInChunk(int chunkX, int chunkY, int chunkZ);
	// Build a cactus on a specific position.
	void createCactusOnPosition(int chunkX, int chunkY, int chunkZ, int chunkBlockX, int chunkBlockY, int chunkBlockZ);
	// Render an entire chunk and load all vertices in the vertex buffer.
	void renderChunk(int chunkX, int chunkY, int chunkZ);
	// Fill all vertices in the chunks own VBO.
	void fillChunkVBO(int chunkX, int chunkY, int chunkZ);
	// Unrender a chunk by deleting the VBO.
	void unrenderChunk(int chunkX, int chunkY, int chunkZ);
	// Load a chunk and fill it up with environment data.
	void loadChunk(int chunkX, int chunkY, int chunkZ);
	// Function to create a heightmap.
	std::array<int, Chunk::CHUNK_SIZE_DEPTH* Chunk::CHUNK_SIZE_WIDTH> createChunkHeightMap(int chunkX, int chunkY, int chunkZ);
	// Function that creates flora for a chunk.
	void createFlora(int chunkX, int chunkY, int chunkZ);
	// Create terrain using simplex noise.
	void createSmoothTerrain(int chunkX, int chunkY, int chunkZ, const std::array<int, Chunk::CHUNK_SIZE_DEPTH* Chunk::CHUNK_SIZE_WIDTH>& heightMap);
	// Create a sun.
	void createSun();
	// Player object pointer.
	std::shared_ptr<Player> player;
	// TIGL Vertices buffer vector.
	std::shared_ptr<std::vector<tigl::Vertex>> buffer;
	// Vertices buffer vector.
	std::shared_ptr<std::vector<glm::vec3>> vertices_in;
	// Indices buffer vector.
	std::shared_ptr<std::vector<glm::vec3>> indices_in;
};

