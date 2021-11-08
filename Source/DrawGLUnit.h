#pragma once

#include "GLUnit.h"

class DrawGLUnit : public GLUnit
{
public:
	DrawGLUnit();
	~DrawGLUnit();
	virtual void draw() const = 0;
};

