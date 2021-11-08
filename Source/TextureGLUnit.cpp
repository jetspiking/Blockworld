#include "TextureGLUnit.h"

TextureGLUnit::TextureGLUnit()
{
}

TextureGLUnit::~TextureGLUnit()
{
}

void TextureGLUnit::update() const
{
	tigl::shader->enableTexture(true);
	tigl::shader->enableColor(false);
}

void TextureGLUnit::fill_vertices(std::vector<glm::vec3>& indices, std::vector<tigl::Vertex>& vertices_out, Shape::ShapeType& shapeType) const
{
	this->textureAtlas->updateTextureCoords(this->textureAtlasCoords.x, this->textureAtlasCoords.y);
	for (int i = 0; i < indices.size(); i++)
		vertices_out[i] = (tigl::Vertex::PT(indices[i], this->textureAtlas->textureCoords[i % 4]));
}

void TextureGLUnit::set_texture_atlas(std::shared_ptr<TextureAtlas> textureAtlas)
{
	this->textureAtlas = textureAtlas;
}

std::shared_ptr<TextureAtlas> TextureGLUnit::get_texture_atlas()
{
	return this->textureAtlas;
}

void TextureGLUnit::set_texture_atlas_coords(int image_col, int image_row)
{
	this->textureAtlasCoords[0] = image_col;
	this->textureAtlasCoords[1] = image_row;
}
