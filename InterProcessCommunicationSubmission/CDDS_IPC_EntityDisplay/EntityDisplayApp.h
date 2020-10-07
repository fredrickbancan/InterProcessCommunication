#pragma once
#include <vector>
#include "raylib.h"
#include "WinInc.h"

struct Entity {
	float x = 0, y = 0;
	float rotation = 0;
	float speed = 0;
	unsigned char r = 0, g = 0, b = 0;
	float size = 1;
};

class EntityDisplayApp  {
public:
	EntityDisplayApp(int screenWidth = 800, int screenHeight = 450);
	~EntityDisplayApp();

	bool Startup();

	//opens the already set up virtual file for reading, returns false if fails.
	bool setUpEntityNSM();

	void Shutdown();

	//closes named shared memory
	void closeEntityNSM();

	void Update(float deltaTime);

	//reads new data from the virtual file and applies it to the entities.
	void updateNSM();

	void Draw();

protected:
	int m_screenWidth;
	int m_screenHeight;
	int entityCount = 0;
	// an array of an unknown number of entities
	std::vector<Entity> m_entities;
	Entity* entityData = nullptr;//handle for reading virtual file as entity array
	HANDLE fileHandle = nullptr;
};