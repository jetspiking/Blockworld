#pragma once
#include "DecorationGLUnit.h"
#include "TextureAtlas.h"
#include "Shape.h"
#include "tigl.h"

class TextureColorComboGLUnit : public DecorationGLUnit
{
public:
	TextureColorComboGLUnit();
	~TextureColorComboGLUnit();
	virtual void update() const override;
	virtual void fill_vertices(std::vector<glm::vec3>& indices, std::vector<tigl::Vertex>& vertices_out, Shape::ShapeType& shapeType) const override;
	void set_texture_color_filter(float r, float g, float b, float a);
	void set_texture_atlas(std::shared_ptr<TextureAtlas> textureAtlas);
	std::shared_ptr<TextureAtlas> get_texture_atlas();
	void set_texture_atlas_coords(int image_col, int image_row);
	glm::vec2 textureAtlasCoords = glm::vec2(-1, -1);
	glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f);
	std::shared_ptr<TextureAtlas> textureAtlas;
};