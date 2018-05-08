#pragma once

class RigidbodyComponent
{
public:
	vec3*	_posVec3;
	quat*	_quat;

	vec3* GetPosVec3() { return _posVec3; }
	quat* GetQuat() { return _quat; }
};