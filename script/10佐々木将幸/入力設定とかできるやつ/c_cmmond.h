#pragma once

class CBase;
class CCommand
{
public:
	virtual void execute(CBase& actor) = 0;
};

class C_Move_Front :public CCommand
{
public:
	void execute(CBase& actor);
};

class C_Move_Left :public CCommand
{
public:
	void execute(CBase& actor);
};

class C_Move_Back :public CCommand
{
public:
	void execute(CBase& actor);
};

class C_Move_Right :public CCommand
{
public:
	void execute(CBase& actor);
};

class C_Move_Up :public CCommand
{
public:
	void execute(CBase& actor);
};

class C_Move_Down :public CCommand
{
public:
	void execute(CBase& actor);
};

class C_Rotation_Pitch_Plus :public CCommand
{
public:
	void execute(CBase& actor);
};

class C_Rotation_Pitch_Minus :public CCommand
{
public:
	void execute(CBase& actor);
};

class C_Rotation_Yaw_Plus :public CCommand
{
public:
	void execute(CBase& actor);
};

class C_Rotation_Yaw_Minus :public CCommand 
{
public:
	void execute(CBase& actor);
};

class C_Sphere_Bound_Rotation_Pitch_Plus:public CCommand
{
public:
	void execute(CBase& actor);
};

class C_Sphere_Bound_Rotation_Pitch_Minus :public CCommand
{
public:
	void execute(CBase& actor);
};

class C_Sphere_Bound_Rotation_Yaw_Minus :public CCommand
{
public:
	void execute(CBase& actor);
};

class C_Sphere_Bound_Rotation_Yaw_Plus :public CCommand
{
public:
	void execute(CBase& actor);
};

class C_Attack :public CCommand
{
public:
	void execute(CBase& actor);
};