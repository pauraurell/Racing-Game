#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
	DynamicCamera = true;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Camera initial point of view --------------------------
	view = THIRD_PERSON;

	// Player spawn point --------------------------
	vec3 spawnPoint = {20, 3, -70};

	// Car properties ----------------------------------------
	car.chassis_size.Set(2.5, 0.9f, 4);
	car.chassis_offset.Set(0, 1.5, -1.5);
	car.glass_size.Set(0.75, 0.7f, 3.4);
	car.glass_offset.Set(0, 1.95, -1.75);
	car.prow_size.Set(1, 0.7f, 3);
	car.prow_offset.Set(0, 1.5, 2);
	car.aileron_size.Set(2.5, 0.5f, 0.75);
	car.aileron_offset.Set(0, 2.25, -3.8);
	car.mass = 500.0f;
	car.suspensionStiffness = 15.0f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.9f;
	car.maxSuspensionTravelCm = 800.0f;
	car.frictionSlip = 20.5;
	car.maxSuspensionForce = 6000.0f;

	car.car_size = car.chassis_size + car.prow_size;

	// Wheel properties ---------------------------------------
	float connection_height = 1.8f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.6f;
	float suspensionRestLength = 0.5f;

	// Don't change anything below this line ------------------

	float half_width = car.car_size.x*0.5f;
	float half_length = car.car_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.25 - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.25 + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.2 - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = true;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.2 + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = true;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(spawnPoint.x, spawnPoint.y, spawnPoint.z);

	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		if (vehicle->GetKmh() > 300) //Limit acceleration when velocity is bigger than 300 km/h
			acceleration = MAX_ACCELERATION / (vehicle->GetKmh() / 200);
		else {
			if (acceleration < 10) acceleration = 1500;
			else if (acceleration < MAX_ACCELERATION) acceleration = MAX_ACCELERATION * (vehicle->GetKmh() / 5); //Progresive acceleration for reality sensation
			else acceleration = MAX_ACCELERATION;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		if (vehicle->GetKmh() > 0)
			brake = BRAKE_POWER;
		else acceleration = -100;
	}
	else brake = 10.0f;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if (turn < TURN_DEGREES) {
			if (vehicle->GetKmh() < 60) {
				turn += TURN_DEGREES;
			}
			else turn += TURN_DEGREES / (vehicle->GetKmh() / 80);
		}
	}

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if (turn > -TURN_DEGREES) {
			if (vehicle->GetKmh() < 60) {
				turn -= TURN_DEGREES;
			}
			else turn -= TURN_DEGREES / (vehicle->GetKmh() / 80);

		}
	}

	VehiclePos = vehicle->GetPos();
	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();

	char title[80];
	sprintf_s(title, "%.1f Km/h", vehicle->GetKmh());
	App->window->SetTitle(title);

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		DynamicCamera = !DynamicCamera;
	}

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		DynamicCamera = true;
		if (view == THIRD_PERSON) { view = FIRST_PERSON; }
		else if (view == FIRST_PERSON) { view = UP_VIEW; }
		else if (view == UP_VIEW) { view = LOOK_BEHIND; }
		else if (view == LOOK_BEHIND) { view = THIRD_PERSON; }
	}

	if (DynamicCamera)
	{
		//the camera follows the car
		CameraFollowingPlayer();
	}
	

	return UPDATE_CONTINUE;
}

void  ModulePlayer::CameraPointOfView()
{
	switch (view)
	{
		case THIRD_PERSON: CameraDistance = { -12.0f, 5.0f, -10.0f }; break;
		case FIRST_PERSON: CameraDistance = { -6.f, 3.f, -5.f }; break;
		case UP_VIEW: CameraDistance = { -18.0f, 10.0f, -14.0f }; break;
		case LOOK_BEHIND: CameraDistance = { 12.0f, 6.0f, 10.0f }; break;
	}
}

void  ModulePlayer::CameraFollowingPlayer()
{
	CameraPointOfView();

	vec3 forwardVector = vehicle->GetForward(); //the vector that looks forward respect the car position
	vec3 NewCameraPosition = { VehiclePos.x + (forwardVector.x * CameraDistance.x), VehiclePos.y + (forwardVector.y + CameraDistance.y), VehiclePos.z + (forwardVector.z * CameraDistance.z) };
	vec3 CamPos = App->camera->Position + (NewCameraPosition - App->camera->Position);

	App->camera->Look(CamPos, VehiclePos);
}



