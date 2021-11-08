#pragma once
#include <list>
#include "GLUnit.h"
#include "DrawGLUnit.h"
#include "DecorationGLUnit.h"
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

class GLUnit;
class DrawGLUnit;
class DecorationGLUnit;

class GLObject
{
public:
	GLObject();
	~GLObject();

	std::shared_ptr<DrawGLUnit> drawGLUnit = nullptr;
	std::shared_ptr<DecorationGLUnit> decorationGLUnit = nullptr;

	std::list<std::shared_ptr<GLUnit>> units;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale = glm::vec3(1,1,1);

	void addGLUnit(std::shared_ptr<GLUnit> unit);
	void setDrawGLUnit(std::shared_ptr<DrawGLUnit> unit);
	void setDecorationGLUnit(std::shared_ptr<DecorationGLUnit> unit);

	std::list<std::shared_ptr<GLUnit>> getGLUnits();
	void update(float elapsedTime);
	void draw(const glm::mat4& = glm::mat4(1.0f));

	template<class T>
	T* getGLUnit()
	{
		for (auto unit : units)
		{
			T* t = dynamic_cast<T*>(unit);
			if (t)
				return t;
		}
		return nullptr;
	}

	template<class T>
	void removeGLUnit()
	{
		units.remove_if([](GLUnit* c) 
		{
			T* t = dynamic_cast<T*>(c);
			return t != nullptr;
		});
	}

};

