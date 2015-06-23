#include "Level.h"

Level* Level::m_pLevel = NULL;


Level* Level::getInstance()
{
	if (m_pLevel == NULL)
	{
		m_pLevel = new Level();
		m_pLevel->loadLevelFromFile("./../Labyrinth/Level/Level1.txt");

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

	float x, y;
	getStartingPoint(x, y);
	mLogger->LogInfo("starting at X=<" + std::to_string(x) + ">");
	mLogger->LogInfo("starting at Y=<" + std::to_string(y) + ">");
}

bool Level::isValidFieldType(char fieldType)
{
	if ((fieldType == '#') ||
		(fieldType == '\n') ||
		(fieldType == ' ') ||
		(fieldType == 'B'))
	{
		return true;
	}
	else
		return false;
}

void Level::renderLevel()
{
	glPushMatrix();

	for (std::vector<char> row : mLevel)
	{
		for (char fieldType : row)
		{
			glTranslatef(mBlockSize * 2, 0, 0);
			
			if (fieldType == '#')
			{
				drawCube();
			}
		}
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

void Level::getStartingPoint(float &x, float &y)
{
	y = 0;
	for (std::vector<char> row : mLevel)
	{
		x = 0;
		for (char fieldType : row)
		{
			if (fieldType == 'B')
				return;
			x++;
		}
		y++;
		
	}
}