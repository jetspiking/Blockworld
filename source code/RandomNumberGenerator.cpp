#include "RandomNumberGenerator.h"

RandomNumberGenerator::RandomNumberGenerator()
{
	this->generator = std::make_shared<std::mt19937>(this->random());
	this->distribution = std::make_shared<std::uniform_real_distribution<>>(0, 1);
}

float RandomNumberGenerator::getRandom()
{
	return (*this->distribution)(*this->generator);
}
