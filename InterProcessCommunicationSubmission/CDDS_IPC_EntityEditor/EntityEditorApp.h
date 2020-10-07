#pragma once
#include <vector>
#include "raylib.h"
#include "WinInc.h"

enum { ENTITY_COUNT = 10 };

struct Entity {
	float x = 0, y = 0;
	float rotation = 0;
	float speed = 0;
	unsigned char r = 0, g = 0, b = 0;
	float size = 1;
};

struct CombinedData
{
//	int count = ENTITY_COUNT;
	Entity entities[ENTITY_COUNT + 1];//plus one for dummy count entitiy
};

class EntityEditorApp {
public:
	EntityEditorApp(int screenWidth = 800, int screenHeight = 450);
	~EntityEditorApp();

	bool Startup();

	//creates and formats virtual file for named shared memory, returns false if fails.
	bool setUpEntityNSM();

	void Shutdown();

	//closes named shared memory
	void closeEntityNSM();

	void Update(float deltaTime);

	//updates virtual file with new data
	void updateNSM();

	void Draw();


protected:
	int m_screenWidth;
	int m_screenHeight;

	// define a block of entities that should be shared
	Entity m_entities[ENTITY_COUNT]; 

	//Entity used at the start of the packet to provide a count of all entities.
	Entity dummyCountEntity = Entity{ENTITY_COUNT};

	CombinedData packet = CombinedData{};
	CombinedData* dataHandle = nullptr;//handle for writing data to the virtual file
	HANDLE fileHandle = nullptr;
};