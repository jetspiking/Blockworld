#pragma once

class GLObject;

class GLUnit
{
protected:
	GLObject* glObject;
public:
	GLUnit();
	~GLUnit();

	virtual void update(float elapsedTime) const {};
	void setGLObject(GLObject* glObject);
};

