//=====================================
//    
//   debugcollision.h
//              created by Koki Nakano
//
//=====================================


#pragma once
#ifndef _DEBUGCOLLISION_H_
#define _DEBUGCOLLISION_H_





class CDebugCollision
{
private:
	static ID3D11ShaderResourceView* m_Texture;             //テクスチャ

public:
	static void Init();
	static void Uninit();
};


#endif