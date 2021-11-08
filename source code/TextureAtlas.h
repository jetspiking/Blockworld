#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

class TextureAtlas {
public:
	TextureAtlas(int atlasWidthPx_in, int atlasHeightPx, int amountOfImagesInWidth, int amountOfImagesInHeight);
	~TextureAtlas();
	void updateTextureCoords(int image_col, int image_row);
	glm::vec2 textureCoords[4];
private:
	int atlasWidthPx;
	int atlasHeightPx;
	int amountOfImagesInWidth;
	int amountOfImagesInHeight;
};