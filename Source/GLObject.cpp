#include "GLObject.h"


GLObject::GLObject()
{
}


GLObject::~GLObject()
{
}

void GLObject::addGLUnit(std::shared_ptr<GLUnit> unit)
{
	unit->setGLObject(this);
	units.push_back(unit);
}

void GLObject::setDrawGLUnit(std::shared_ptr<DrawGLUnit> unit)
{
	unit->setGLObject(this);
	this->drawGLUnit = unit;
}

void GLObject::setDecorationGLUnit(std::shared_ptr<DecorationGLUnit> unit)
{
	unit->setGLObject(this);
	this->decorationGLUnit = unit;
}

std::list<std::shared_ptr<GLUnit>> GLObject::getGLUnits()
{
	return units;
}


void GLObject::draw(const glm::mat4 &parentMatrix)
{

	if (!drawGLUnit) return;
	if (!decorationGLUnit) return;

	glm::mat4 modelMatrix = parentMatrix;
	position = glm::vec3(0, 0, 0);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, scale);
	
	tigl::shader->setModelMatrix(modelMatrix);
	
	decorationGLUnit->update();
	drawGLUnit->draw();
}

void GLObject::update(float elapsedTime)
{
	for (auto &unit : units)
		unit->update(elapsedTime);
}