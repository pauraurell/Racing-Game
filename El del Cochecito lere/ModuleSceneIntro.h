#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#include "PugiXml\src\pugixml.hpp"

#define MAX_SNAKE 2
#define MAX_PILES 1000

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Awake();
	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	void LapCounter();

public:
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	Cube base;
	Cube bar;
	PhysBody3D* pBase;
	PhysBody3D* pBar;

	Cube* mapPiles[MAX_PILES];

	//Laps
	PhysBody3D* checkpointBody;
	Cube checkpoint;
	int lap;

private:
	pugi::xml_document map_file;
	pugi::xml_node map_node;

	pugi::xml_node LoadMap(pugi::xml_document&) const;

	Plane* p;
};
