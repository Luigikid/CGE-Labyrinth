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
	//check up direction
	float zUp = NewZ - 0.5f;
	int zUpInt = (int)zUp;
	
	//check down direction
	float zDown = NewZ + 0.5f;
	int zDownInt = (int)zDown;

	//check left 
	float xLeft = NewX - 0.5f;
	int xLeftInt = (int)xLeft;

	//check right
	float xRight = NewX + 0.5f;
	int xRightInt = (int)xRight;

	mLogger->LogInfo("z Coord for up check=<" + std::to_string(zUpInt) + ">");
	mLogger->LogInfo("z Coord for down check=<" + std::to_string(zDownInt) + ">");

	mLogger->LogInfo("x Coord for left check=<" + std::to_string(xLeftInt) + ">");
	mLogger->LogInfo("x Coord for right check=<" + std::to_string(xRightInt) + ">");


	if (!isFree(xLeftInt, zUpInt))
	{
		mLogger->LogInfo("left up not allowed");
		mLogger->LogInfo("which is mLevel[" + std::to_string(xLeftInt) + "][" + std::to_string(zUpInt) + " <" + std::to_string(mLevel[xLeftInt][zUpInt]) + ">");
		return false;
	}
	if (!isFree(xRightInt, zUpInt))
	{
		mLogger->LogInfo("right up not allowed");
		mLogger->LogInfo("which is mLevel[" + std::to_string(xRightInt) + "][" + std::to_string(zUpInt) + " <" + std::to_string(mLevel[xRightInt][zUpInt]) + ">");
		return false;
	}
	if (!isFree(xLeftInt, zDownInt))
	{
		mLogger->LogInfo("right up not allowed");
		mLogger->LogInfo("which is mLevel[" + std::to_string(xLeftInt) + "][" + std::to_string(zDownInt) + " <" + std::to_string(mLevel[xLeftInt][zDownInt]) + ">");
		return false;
	}
	if (!isFree(xRightInt, zDownInt))
	{
		mLogger->LogInfo("right up not allowed");
		mLogger->LogInfo("which is mLevel[" + std::to_string(xRightInt) + "][" + std::to_string(zDownInt) + " <" + std::to_string(mLevel[xRightInt][zDownInt]) + ">");
		return false;
	}
	return true;

}

bool Level::isFree(int x, int y)
{
	if (mLevel[x][y] != '#')
		return true;
	else
		return false;
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