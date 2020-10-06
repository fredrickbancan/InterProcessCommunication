#include "EntityDisplayApp.h"
#include <iostream>
#include <string>
EntityDisplayApp::EntityDisplayApp(int screenWidth, int screenHeight) : m_screenWidth(screenWidth), m_screenHeight(screenHeight) {

}

EntityDisplayApp::~EntityDisplayApp() {

}

bool EntityDisplayApp::Startup() {

	InitWindow(m_screenWidth, m_screenHeight, "EntityDisplayApp");
	SetTargetFPS(60);
	return setUpEntityNSM();
}

bool EntityDisplayApp::setUpEntityNSM()
{
	/*First, opening virtual file to read entity count*/
	fileHandle = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"Entities");

	if (fileHandle == nullptr)
	{
		std::cout << std::endl << "Could not open file mapping object for reading entities. Error code: " << GetLastError() << std::endl;
		return false;
	}
	float* floatHandle = (float*)MapViewOfFile(fileHandle, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(float));// getting just the entity count value as float first.

	if (floatHandle == nullptr)
	{
		std::cout << std::endl << "Could not get entity count from file mapping object! Error code: " << GetLastError() << std::endl;
		return false;
	}

	entityCount = (int)*floatHandle;

	std::cout << std::endl << std::string("Got entity count! Value: ") + std::to_string(entityCount) << std::endl;

	//Done with integer view of virtual file, so closing view mapping
	UnmapViewOfFile(floatHandle);

	//view file for reading array of entities
	entityData = (Entity*)MapViewOfFile(fileHandle, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(Entity) * (entityCount + 1));//plus one for dummy count entity

	if (entityData == nullptr)
	{
		std::cout << std::endl << "Could not view file mapping object to read entities: " << GetLastError() << std::endl;
		return false;
	}

	std::cout << "Successfully opened file mapping object to read entities!" << std::endl;

	for (int i = 1; i < entityCount + 1; i++)
	{
		m_entities.push_back(entityData[i]);
	}
	return true;
}

void EntityDisplayApp::Shutdown() 
{
	closeEntityNSM();
	CloseWindow();        // Close window and OpenGL context
}

void EntityDisplayApp::closeEntityNSM()
{
	// unmap the memory block since we're done with it
	UnmapViewOfFile(entityData);

	//close file handle
	CloseHandle(fileHandle);
}

void EntityDisplayApp::Update(float deltaTime) 
{
	updateNSM();
}

void EntityDisplayApp::updateNSM()
{
	//update vector with new data
	m_entities.clear();
	for (int i = 1; i < entityCount + 1; i++)
	{
		m_entities.push_back(entityData[i]);
	}
}

void EntityDisplayApp::Draw() {
	BeginDrawing();

	ClearBackground(RAYWHITE);

	// draw entities
	for (auto& entity : m_entities) 
	{
		DrawRectanglePro(
			Rectangle{ entity.x, entity.y, entity.size, entity.size }, // rectangle
			Vector2{ entity.size / 2, entity.size / 2 }, // origin
			entity.rotation,
			Color{ entity.r, entity.g, entity.b, 255 });
	}

	// output some text, uses the last used colour
	DrawText("Press ESC to quit", 630, 15, 12, LIGHTGRAY);

	EndDrawing();
}