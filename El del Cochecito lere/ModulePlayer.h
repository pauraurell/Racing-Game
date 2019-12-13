#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 200.0f
#define TURN_DEGREES 12.0f * DEGTORAD
#define BRAKE_POWER 100.0f

enum cameraTypes {
	THIRD_PERSON,
	FIRST_PERSON
};
class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	void CameraFollowingPlayer();

public:

	bool DynamicCamera; 
	vec3 CameraDistance;
	vec3 VehiclePos;
	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;

private:
	cameraTypes view;
};