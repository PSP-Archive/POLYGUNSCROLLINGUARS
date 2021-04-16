//-------------------------------------------------------------------------------------
//
// JGE++ is a hardware accelerated 2D game SDK for PSP/Windows.
//
// Licensed under the BSD license, see LICENSE in JGE root for details.
// 
// Copyright (c) 2007 James Hui (a.k.a. Dr.Watson) <jhkhui@gmail.com>
// 
//-------------------------------------------------------------------------------------

#ifndef _GAMEAPP_H_
#define _GAMEAPP_H_

#include <JApp.h>

class GameApp:	public JApp
{
private:
	//JTexture *mTexture;
public:
	GameApp();
	virtual ~GameApp();
	virtual void Create();
	virtual void Destroy();
	virtual void Update();
	virtual void Render();
	virtual void Pause();
	virtual void Resume();
	virtual int collide(int Hotx1,int Hoty1,int radius1,int Hotx2,int Hoty2, int radius2);//definicion virtual del checador de coliciones.
	bool GAMEPUSED_BY_SYSYTEM;
};

#endif
