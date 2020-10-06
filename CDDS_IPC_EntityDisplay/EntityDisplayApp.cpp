#include "EntityDisplayApp.h"
#include "Windows.h"
#include <iostream>
#include <string>
EntityDisplayApp::EntityDisplayApp(int screenWidth, int screenHeight) : m_screenWidth(screenWidth), m_screenHeight(screenHeight) {

}

EntityDisplayApp::~EntityDisplayApp() {

}

bool EntityDisplayApp::Startup() {

	InitWindow(m_screenWidth, m_screenHeight, "EntityDisplayApp");
	SetTargetFPS(60);
	setUpEntityNSM();
	return true;
}

bool EntityDisplayApp::setUpEntityNSM()
{
	/*First, opening virtual file to read entity count*/
	fileHandle =
		CreateFileMapping(INVALID_HANDLE_VALUE, // a handle to an existing virtual file, or invalid
			nullptr, // optional security attributes
			PAGE_READWRITE, // read/write access control
			0, sizeof(int), // getting just the entity count integer first.
			L"Entities");

	if (fileHandle == nullptr)
	{
		std::cout << std::endl << "Could not create file mapping object for reading entity count: " << GetLastError() << std::endl;
		return false;
	}
	int* intHandle = (int*)MapViewOfFile(fileHandle, FILE_MAP_READ, 0, 0, sizeof(int));

	if (intHandle == nullptr)
	{
		std::cout << std::endl << "Could not get entity count from file mapping object! " << GetLastError() << std::endl;
		return false;
	}

	entityCount = *intHandle;

	std::cout << std::endl << std::string("Got entity count! Value: ") + std::to_string(entityCount) << std::endl;
	UnmapViewOfFile(intHandle);
	CloseHandle(fileHandle);

	//re-open file for reading array of entities:

	fileHandle =
		CreateFileMapping(INVALID_HANDLE_VALUE, // a handle to an existing virtual file, or invalid
			nullptr, // optional security attributes
			PAGE_READWRITE, // read/write access control
			sizeof(int), sizeof(Entity) * entityCount, // getting the entity data after the count integer
			L"Entities");

	if (fileHandle == nullptr)
	{
		std::cout << std::endl << "Could not open file mapping object to read entities: " << GetLastError() << std::endl;
		return false;
	}

	std::cout << "Successfully opened file mapping object to read entities!" << std::endl;



	return true;
}

void EntityDisplayApp::Shutdown() {

	CloseWindow();        // Close window and OpenGL context
}

void EntityDisplayApp::closeEntityNSM()
{
}

void EntityDisplayApp::Update(float deltaTime) {

}

void EntityDisplayApp::Draw() {
	BeginDrawing();

	ClearBackground(RAYWHITE);

	// draw entities
	for (auto& entity : m_entities) {
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