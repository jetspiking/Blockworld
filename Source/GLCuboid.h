#pragma once
#include "GLShape.h"
#include "TextureColorComboGLUnit.h"
class GLCuboid : public GLShape
{
public:
	GLCuboid(std::shared_ptr<Cuboid> cuboid);
	virtual void draw() const override;
	void setDrawMask(int drawMask);
private:
	int drawMask = -1;
};

