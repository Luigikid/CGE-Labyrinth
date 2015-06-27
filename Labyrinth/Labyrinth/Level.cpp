#include "Level.h"

Level* Level::m_pLevel = NULL;


Level* Level::getInstance()
{
	if (m_pLevel == NULL)
	{
		m_pLevel = new Level();
		m_pLevel->loadLevelFromFile("./../Labyrinth/Levels/Level1.txt");
	}
	return m_pLevel;
}


Level::Level()
{
	levelFinished = false;
}

void Level::setFloorTextureId(GLuint id)
{
	floorTextureId = id;
}

void Level::setWallTextureId(GLuint id)
{
	wallTextureId = id;
}


Level::~Level()
{
}

void Level::modifyGoalRotationValue(GLfloat value)
{
	goalRotation += value;
}

bool Level::checkAllowed(GLfloat OldX, GLfloat OldZ, GLfloat &NewX, GLfloat &NewZ)
{
	int x, z;
	float zOffset;
	float xOffset;

	//check with offset in up direction
	zOffset = NewZ - mCollissionOffset;
	getLevelFieldCoords(NewX, zOffset, x, z);

	if (getFieldTypeForCoords(x, z) == 'E')
		levelFinished = true;

	if (!isFree(x, z))
	{
		GLfloat tempX;
		getWorldSpaceCoordsFromLevelCoords(0, z, tempX, NewZ);
		NewZ -= mCollissionOffset + mBlockSize + 2.0;

		return true;
	}
		

	//check with offset in down direction
	zOffset = NewZ + mCollissionOffset;
	getLevelFieldCoords(NewX, zOffset, x, z);
	if (!isFree(x, z))
		return false;

	//check with offset in left direction
	xOffset = NewX - mCollissionOffset;
	getLevelFieldCoords(xOffset, NewZ, x, z);
	if (!isFree(x, z))
		return false;

	//check with offset in right direction
	xOffset = NewX + mCollissionOffset;
	getLevelFieldCoords(xOffset, NewZ, x, z);
	if (!isFree(x, z))
		return false;

	return true;

}

bool Level::checkLevelFinished()
{
	if (levelFinished && !noticedAboutFinished)
	{
		mLogger->LogInfo("level given info for finish");
		noticedAboutFinished = true;
		return true;
	}
	return false;
}


bool Level::isFree(int x, int z)
{
	if (mLevel[z][x] != '#')
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
		(fieldType == 'B') ||	// B ... Begin
		(fieldType == 'E'))		// E ... End
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
				drawCube();
			else if (fieldType == 'E')
				drawGoal();
			else
				drawFloor();
		}
		glTranslatef(-(row.size()*(mBlockSize * 2)), 0, mBlockSize * 2);
	}
	glPopMatrix();
}

void Level::getLevelFieldCoords(float x, float z, int &xInt, int &zInt)
{
	x = x - 2.0f;
	
	x = x + mBlockSize;	// add +1 blocksize as our blocks are from -blocksize to +blocksize (which is 2 blocksizes)
	z = z + mBlockSize;
	
	x = x / (2 * mBlockSize);
	z = z / (2 * mBlockSize);

	//always round to the smaller int (which is done by default)
	xInt = (int)x;
	zInt = (int)z;
}

void Level::getWorldSpaceCoordsFromLevelCoords(float levelX, float levelZ, float &worldX, float &worldZ)
{
	worldX = levelX * (2 * mBlockSize);
	worldX += 6*mCollissionOffset;
	
	worldZ = levelZ * (2 * mBlockSize);
	worldZ += 6*mCollissionOffset;


	mLogger->LogInfo("level x=<" + std::to_string(levelX) + ">");
	mLogger->LogInfo("level z=<" + std::to_string(levelZ) + ">");

	mLogger->LogInfo("world x=<" + std::to_string(worldX) + ">");
	mLogger->LogInfo("world z=<" + std::to_string(worldZ) + ">");
}

char Level::getFieldTypeForCoords(int x, int z)
{
	if ((x >= 0) && (z >= 0))	// TODO: implement limit check for above limit
		return mLevel[z][x];
	else
	{
		mLogger->LogError("tried to get Fieldtype for x=<" + std::to_string(x) + "> and z=<" + std::to_string(z) + ">");
		return '#';	//return a wall 
	}
}
void Level::drawFloor()
{
	glBindTexture(GL_TEXTURE_2D, wallTextureId);
	glBegin(GL_QUADS);
	// bottom face
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-mBlockSize, -mBlockSize, -mBlockSize);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(mBlockSize, -mBlockSize, -mBlockSize);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(mBlockSize, -mBlockSize, mBlockSize);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-mBlockSize, -mBlockSize, mBlockSize);

	glEnd();
}
void Level::drawGoal()
{
	glScalef(0.5, 0.5, 0.5);
	glRotatef(goalRotation, 0, 1, 0);
	
	
	glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
	glColor3f(1, 0.8431372549019608, 0);	// make it gold
	// Front
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Right
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Back
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	// Left
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();   // Done drawing the pyramid

	glRotatef(-goalRotation, 0, 1, 0);
	glScalef(2, 2, 2);
	drawFloor();
}

void Level::drawCube()
{
	glBindTexture(GL_TEXTURE_2D, floorTextureId);
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