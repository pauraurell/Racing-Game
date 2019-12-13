#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Grey;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());
	chassis.color = LightBlue;

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	Cube glass(info.glass_size.x, info.glass_size.y, info.glass_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&glass.transform);
	btQuaternion q2 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset2(info.glass_offset.x, info.glass_offset.y,
	info.glass_offset.z);
	offset2 = offset2.rotate(q2.getAxis(), q2.getAngle());
	glass.color = Yellow;

	glass.transform.M[12] += offset2.getX();
	glass.transform.M[13] += offset2.getY();
	glass.transform.M[14] += offset2.getZ();

	Cube prow(info.prow_size.x, info.prow_size.y, info.prow_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&prow.transform);
	btQuaternion q3 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset3(info.prow_offset.x, info.prow_offset.y,
		info.prow_offset.z);
	offset3 = offset3.rotate(q3.getAxis(), q3.getAngle());
	prow.color = Yellow;

	prow.transform.M[12] += offset3.getX();
	prow.transform.M[13] += offset3.getY();
	prow.transform.M[14] += offset3.getZ();

	Cube aileron(info.aileron_size.x, info.aileron_size.y, info.aileron_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&aileron.transform);
	btQuaternion q4 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset4(info.aileron_offset.x, info.aileron_offset.y,
		info.aileron_offset.z);
	offset4 = offset4.rotate(q4.getAxis(), q4.getAngle());
	aileron.color = NavyBlue;

	aileron.transform.M[12] += offset4.getX();
	aileron.transform.M[13] += offset4.getY();
	aileron.transform.M[14] += offset4.getZ();


	chassis.Render();
	glass.Render();
	prow.Render();
	aileron.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}
