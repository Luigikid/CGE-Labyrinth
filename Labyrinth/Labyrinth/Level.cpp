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

void Level::loadLevelFromFile(std::string fileName)
{
	std::ifstream LevelFile;
	std::string line;
	char fieldType;

	LevelFile.open(fileName);
	if (LevelFile.is_open())
	{
		std::vector<char> row;

		while (LevelFile >> std::noskipws >> fieldType)
		{
			mLogger->LogInfo("fieldType=<" + std::to_string(fieldType) + ">");

			if (fieldType == '\n')
			{
				mLevel.push_back(row);
				row.clear();
			}
			else
			{
				if (isValidFieldType(fieldType))	// skip invalid field types
					row.push_back(fieldType);
			}
		}
		mLevel.push_back(row);
		LevelFile.close();
	}
	else
		mLogger->LogError("could not read Level file");
}

bool Level::isValidFieldType(char fieldType)
{
	if ((fieldType == '#') ||
		(fieldType == '\n') ||
		(fieldType == ' '))
	{
		return true;
	}
	else
		return false;
}

void Level::renderLevel()
{
	glPushMatrix();

	// spielfeld wird von oben links nach unten rechts gezeichnet (vogelperspektive)
	for (std::vector<char> row : mLevel)
	{
		for (char fieldType : row)
		{
			/* 1 feld weiter nach rechts translaten */
			glTranslatef(mBlockSize * 2, 0, 0);
			
			if (fieldType == '#')
			{
				drawCube();
			}
		}
		/* in die naechste "zeile" translaten */
		glTranslatef(-(row.size()*(mBlockSize * 2)), 0, mBlockSize * 2);
	}
	glPopMatrix();
}

void Level::drawCube()
{
	//Die Kisten textur ist eine "hoch-frequente" Textur -> flimmert
	//hoch frequenz deswegen weil die textur ja ein 2d array ist -> kann man als funktion aufzeichnen -> ist ein signal -> hat eine frequenz
	//das flackern bekommt man mit "mip - mapping" weg -> ist heute common practice
	glBegin(GL_QUADS);
	// front face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-mBlockSize, -mBlockSize, mBlockSize);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(mBlockSize, -mBlockSize, mBlockSize);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(mBlockSize, mBlockSize, mBlockSize);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-mBlockSize, mBlockSize, mBlockSize);
	// back face
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-mBlockSize, -mBlockSize, -mBlockSize);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-mBlockSize, mBlockSize, -mBlockSize);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(mBlockSize, mBlockSize, -mBlockSize);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(mBlockSize, -mBlockSize, -mBlockSize);
	// top face
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-mBlockSize, mBlockSize, -mBlockSize);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-mBlockSize, mBlockSize, mBlockSize);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(mBlockSize, mBlockSize, mBlockSize);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(mBlockSize, mBlockSize, -mBlockSize);
	// bottom face
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-mBlockSize, -mBlockSize, -mBlockSize);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(mBlockSize, -mBlockSize, -mBlockSize);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(mBlockSize, -mBlockSize, mBlockSize);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-mBlockSize, -mBlockSize, mBlockSize);
	// right face
	glTexCoord2f(1.0f, 0.0f); glVertex3f(mBlockSize, -mBlockSize, -mBlockSize);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(mBlockSize, mBlockSize, -mBlockSize);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(mBlockSize, mBlockSize, mBlockSize);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(mBlockSize, -mBlockSize, mBlockSize);
	// left face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-mBlockSize, -mBlockSize, -mBlockSize);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-mBlockSize, -mBlockSize, mBlockSize);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-mBlockSize, mBlockSize, mBlockSize);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-mBlockSize, mBlockSize, -mBlockSize);
	glEnd();
}