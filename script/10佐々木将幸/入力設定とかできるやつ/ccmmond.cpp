//-------------------------------------------------------------------------------
//ヘッダーファイル読み込み
//-------------------------------------------------------------------------------
//*******************************************************************************
//-------------------------------------------------------------------------------
//ライブラリ
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//自作ヘッダ
//-------------------------------------------------------------------------------
#include"c_base.h"
#include"c_cmmond.h"
//*******************************************************************************
void C_Move_Front::execute(CBase& actor)
{
	actor.Move_Front();
}

void C_Move_Left::execute(CBase& actor)
{
	actor.Move_Left();
}

void C_Move_Back::execute(CBase& actor)
{
	actor.Move_Back();
}

void C_Move_Right::execute(CBase& actor)
{
	actor.Move_Right();
}

void C_Move_Up::execute(CBase& actor)
{
	actor.Move_Up();
}

void C_Move_Down::execute(CBase& actor)
{
	actor.Move_Down();
}

void C_Rotation_Pitch_Plus::execute(CBase& actor)
{
	actor.Rotation_Pitch_Plus();
}

void C_Rotation_Pitch_Minus::execute(CBase& actor)
{
	actor.Rotation_Pitch_Minus();
}

void C_Rotation_Yaw_Plus::execute(CBase& actor)
{
	actor.Rotation_Yaw_Plus();
}

void C_Rotation_Yaw_Minus::execute(CBase& actor)
{
	actor.Rotation_Yaw_Minus();
}

void C_Sphere_Bound_Rotation_Pitch_Plus::execute(CBase& actor)
{
	actor.Sphere_bound_Rotation_Pitch_Plus();
}

void C_Sphere_Bound_Rotation_Pitch_Minus::execute(CBase& actor)
{
	actor.Sphere_bound_Rotation_Pitch_Minus();
}

void C_Sphere_Bound_Rotation_Yaw_Minus::execute(CBase& actor)
{
	actor.Sphere_bound_Rotation_Yaw_Minus();
}

void C_Sphere_Bound_Rotation_Yaw_Plus::execute(CBase& actor)
{
	actor.Sphere_bound_Rotation_Yaw_Plus();
}

void C_Attack::execute(CBase& actor)
{
	actor.Attack();
}