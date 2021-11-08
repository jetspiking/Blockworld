#include "TextureColorComboGLUnit.h"

TextureColorComboGLUnit::TextureColorComboGLUnit()
{
}

TextureColorComboGLUnit::~TextureColorComboGLUnit()
{
}

void TextureColorComboGLUnit::update() const
{
	tigl::shader->enableColor(true);
	tigl::shader->enableTexture(true);
}

void TextureColorComboGLUnit::fill_vertices(std::vector<glm::vec3>& indices, std::vector<tigl::Vertex>& vertices_out, Shape::ShapeType& shapeType) const
{
		this->textureAtlas->updateTextureCoords(this->textureAtlasCoords.x, this->textureAtlasCoords.y);

		for (int i = 0; i < indices.size(); i++) {
			vertices_out[i] = (tigl::Vertex::PTC(indices[i], this->textureAtlas->textureCoords[i % 4], this->color)); 
		}
}

void TextureColorComboGLUnit::set_texture_color_filter(float r, float g, float b, float a)
{
	this->color.x = r;
	this->color.y = g;
	this->color.z = b;
	this->color.a = a;
}

void TextureColorComboGLUnit::set_texture_atlas(std::shared_ptr<TextureAtlas> textureAtlas)
{
	this->textureAtlas = textureAtlas;
}

std::shared_ptr<TextureAtlas> TextureColorComboGLUnit::get_texture_atlas()
{
	return this->textureAtlas;
}

void TextureColorComboGLUnit::set_texture_atlas_coords(int image_col, int image_row)
{
	this->textureAtlasCoords = glm::vec2(image_col, image_row);
}
