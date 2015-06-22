#include "Level.h"

Level* Level::m_pLevel = NULL;


Level* Level::getInstance()
{
	if (m_pLevel == NULL)
	{
		m_pLevel = new Level();

	}
	return m_pLevel;
}


Level::Level()
{
}


Level::~Level()
{
}

bool Level::checkAllowed(GLfloat OldX, GLfloat OldZ, GLfloat NewX, GLfloat NewZ)
{
	mLogger->LogInfo("Old x=<" + std::to_string(OldX) + ">");
	mLogger->LogInfo("Old z=<" + std::to_string(OldZ) + ">");

	mLogger->LogInfo("New x=<" + std::to_string(NewX) + ">");
	mLogger->LogInfo("New z=<" + std::to_string(NewZ) + ">");

	return true;
}
