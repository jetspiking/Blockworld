#pragma once
#include <random>
#include <iostream>

// Class to randomly generate numbers, replaced the older implementation of stdblib due to not recommended library.
class RandomNumberGenerator
{
public:
	// Constructor doesn't need alternative objects.
	RandomNumberGenerator();
	// Random device.
	std::random_device random;
	// mt19937 number generator.
	std::shared_ptr<std::mt19937> generator;
	// Distribution object to retrieve a random number.
	std::shared_ptr<std::uniform_real_distribution<>> distribution;
	// Get a random number.
	float getRandom();
};

