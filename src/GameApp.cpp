//-------------------------------------------------------------------------------------
//
// JGE++ is a hardware accelerated 2D game SDK for PSP/Windows.
//
// Licensed under the BSD license, see LICENSE in JGE root for details.
// 
// Copyright (c) 2007 James Hui (a.k.a. Dr.Watson) <jhkhui@gmail.com>
// 
//-------------------------------------------------------------------------------------

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <JAssert.h>

#include <JGE.h>
#include <JRenderer.h>
#include <JLBFont.h>

#include <JSprite.h>
#include <JFileSystem.h>

#include "GameApp.h"

//antes de leer los comentarios de este file se recomeienda leer detalladamente los comentarios den las librerias incluidas en este SRC que son las siuientes...
//como dato las librerias son aquellos archivos cuya extencion es h.
//favor de prestar especial atencion a la libreria stars.h ya que es la mas documentada y donde se explican la mayoria de los fragmentos de codigo.
#include "particles.h"//incluimos la libreria que contiene definiciones de clases particulas.
#include "nave.h"//incluimos la libreria que contiene definiciones de clases naves.
#include "star.h"//incluimos la libreria que contiene definiciones de clases estrellas.


#define PSPW 480//definimos resolucin del psp
#define PSPH 272

//instanciamos las clases para tener accesos asus propiedades y metodos.
naveHero heroPlayer1;
//naveHero heroPlayer2; //si asi lo quisieramos podriamos tener 2 o mas heroes en pantalla... imagina un ad-hock o simplemente uno controla 1 nave son el pad y otro con los botones.. o mejor ahun: naves auxiliaras con AI..
//todo es cuestion de imaginacion y creatividad.. y coco.
enemySyst enemigos;
particlesys particles;
starSyst stars;

//-------------------------------------------------------------------------------------
// Constructor. Variables can be initialized here.
//
//-------------------------------------------------------------------------------------
GameApp::GameApp()
{

	GAMEPUSED_BY_SYSYTEM=false;

}


//-------------------------------------------------------------------------------------
// Destructor.
//
//-------------------------------------------------------------------------------------
GameApp::~GameApp()
{

}


//-------------------------------------------------------------------------------------
// This is the init callback function. You should load and create your in-game 
// resources here.
// 
//-------------------------------------------------------------------------------------
void GameApp::Create()
{
	//inicialisamos la semmilla random.
	srand( (unsigned)JGE::GetInstance()->GetDelta());//generamos la semilla del random.
	
	//inicialisamos nuestros controladores de clases.
	heroPlayer1.init();
	enemigos.init();
	particles.init();
	stars.init();
}


//-------------------------------------------------------------------------------------
// This is the clean up callback function. You should delete all your in-game 
// resources, for example texture and quads, here.
// 
//-------------------------------------------------------------------------------------
void GameApp::Destroy()
{

}


//-------------------------------------------------------------------------------------
// This is the update callback function and is called at each update frame
// before rendering. You should update the game logic here.
//
//-------------------------------------------------------------------------------------
void GameApp::Update()
{
	if(GAMEPUSED_BY_SYSYTEM){return;}
	
	JGE* engine = JGE::GetInstance();

	// do a screen shot when the TRIANGLE button is pressed
	if (engine->GetButtonClick(PSP_CTRL_TRIANGLE))		
	{
		char s[80];

		// save screen shot to root of Memory Stick 
		sprintf(s, "ms0:/screenshot.png");				
		JRenderer::GetInstance()->ScreenShot(s);
	}

	// exit when the CROSS button is pressed
	if (engine->GetButtonClick(PSP_CTRL_RTRIGGER))	
	{
		engine->End();
		return;
	}


	//if (engine->GetButtonClick(PSP_CTRL_CROSS))	
	//{
	//	heroPlayer1.bullets.addbullet(heroPlayer1.x,heroPlayer1.y-20,0,-6);//establecemos las cordenadas para una bala y su velocidad.
	//}
	
	float dt = engine->GetDelta();		// Get time elapsed since last update.
	
	//
	// Your updating code here...
	//

#ifdef WIN32
	dt=1; // caundo compilamos a win no hace falta controlar el tiempo..
	heroPlayer1.shotingTimeElapsed++;
#else
	dt*=50; // esta valor determina el lag que meteremos en el psp para relentizar los codigos de update.
	heroPlayer1.shotingTimeElapsed+=dt;//se actualiza la pausa en los disparos.
#endif	
	//controles de la nave..
	if (engine->GetButtonState(PSP_CTRL_LEFT))	
	{
		heroPlayer1.dx-=0.5*dt;
	}
	if (engine->GetButtonState(PSP_CTRL_RIGHT))	
	{
		heroPlayer1.dx+=0.5*dt;
	}
	if (engine->GetButtonState(PSP_CTRL_CROSS))	//monitoreo del disparador
	{
		if(heroPlayer1.shotingTimeElapsed>heroPlayer1.shotingPauseTime){
			heroPlayer1.bullets.addbullet(heroPlayer1.x,heroPlayer1.y-10,0,-6);//establecemos las cordenadas para una bala y su velocidad.
			heroPlayer1.shotingTimeElapsed = 0;
		}
	}

	//buscar colisiones heroe con enemigos tipo 1.
	for(int i =0;i<maxEneTp1;i++){
		if(collide(heroPlayer1.x,heroPlayer1.y,heroPlayer1.img->mHotSpotX/2,enemigos.enemigosTP1[i].x,enemigos.enemigosTP1[i].y,enemigos.imgEnemigosTP1->mHotSpotX/2)){
			particles.createExplo(heroPlayer1.x,heroPlayer1.y,ARGB(255,rand()%255,rand()%255,rand()%255),100,30);
			particles.createExplo(enemigos.enemigosTP1[i].x,enemigos.enemigosTP1[i].y,ARGB(255,rand()%255,rand()%255,rand()%255),20);
			enemigos.enemigosTP1[i].regen();
			heroPlayer1.reset();
		}
		//buscar colisiones balas del heroe con enemigos tipo 1.
		for(int ii=0;ii<heroPlayer1.bullets.bulletlist.size();ii++){
			if(collide(heroPlayer1.bullets.bulletlist[ii]->x,heroPlayer1.bullets.bulletlist[ii]->y,heroPlayer1.bullets.img->mHotSpotX/2,enemigos.enemigosTP1[i].x,enemigos.enemigosTP1[i].y,enemigos.imgEnemigosTP1->mHotSpotX/2)){
				particles.createExplo(heroPlayer1.bullets.bulletlist[ii]->x,heroPlayer1.bullets.bulletlist[ii]->y,ARGB(255,255,150,0),20);
				particles.createExplo(enemigos.enemigosTP1[i].x,enemigos.enemigosTP1[i].y,ARGB(255,rand()%255,rand()%255,rand()%255),20);
				heroPlayer1.bullets.bulletlist.erase(heroPlayer1.bullets.bulletlist.begin()+ii);
				enemigos.enemigosTP1[i].regen();
			}
		}
	}
	//buscar colisiones heroe con enemigos tipo 1.
	for(int i =0;i<maxEneTp2;i++){
		if(collide(heroPlayer1.x,heroPlayer1.y,heroPlayer1.img->mHotSpotX/2,enemigos.enemigosTP2[i].x,enemigos.enemigosTP2[i].y,enemigos.imgEnemigosTP2->mHotSpotX/2)){
			particles.createExplo(heroPlayer1.x,heroPlayer1.y,ARGB(255,rand()%255,rand()%255,rand()%255),100,30);
			particles.createExplo(enemigos.enemigosTP2[i].x,enemigos.enemigosTP2[i].y,ARGB(255,rand()%255,rand()%255,rand()%255),20);
			enemigos.enemigosTP2[i].regen(); 
			heroPlayer1.reset();
		}
		//buscar colisiones balas del heroe con enemigos tipo 2.
		for(int ii=0;ii<heroPlayer1.bullets.bulletlist.size();ii++){
			if(collide(heroPlayer1.bullets.bulletlist[ii]->x,heroPlayer1.bullets.bulletlist[ii]->y,heroPlayer1.bullets.img->mHotSpotX/2,enemigos.enemigosTP2[i].x,enemigos.enemigosTP2[i].y,enemigos.imgEnemigosTP2->mHotSpotX/2)){
				particles.createExplo(heroPlayer1.bullets.bulletlist[ii]->x,heroPlayer1.bullets.bulletlist[ii]->y,ARGB(255,255,150,0),20);
				particles.createExplo(enemigos.enemigosTP2[i].x,enemigos.enemigosTP2[i].y,ARGB(255,rand()%255,rand()%255,rand()%255),20);
				heroPlayer1.bullets.bulletlist.erase(heroPlayer1.bullets.bulletlist.begin()+ii);
				enemigos.enemigosTP2[i].regen();
			}
		}
	}
	
	


	//todas nuestras funciones de update deberantener como parametro dt. para tener control del tiempo.
	heroPlayer1.update(dt);
	heroPlayer1.bullets.update(dt);
	enemigos.update(dt,heroPlayer1.x,heroPlayer1.y);
	particles.update(dt);
	//particulas para la cola del heroe.
	if(particles.mTimer==0)particles.newparticle(heroPlayer1.x,heroPlayer1.y+10,4,3.1416+(((rand()%200)-100)*0.002),ARGB(255,0,100,255));
	stars.update(dt);
}
//-------------------------------------------------------------------------------------
// All rendering operations should be done in Render() only.
// 
//-------------------------------------------------------------------------------------
void GameApp::Render()
{
	if(GAMEPUSED_BY_SYSYTEM){return;}
	// get JRenderer instance
	JRenderer* renderer = JRenderer::GetInstance();		

	// clear screen to black
	//renderer->ClearScreen(ARGB(0,0,0,0));
	//
	// Your rendering code here...
	//
	//poner en modo blend normal
	renderer->SetTexBlend(BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA);
	
	renderer->FillRect(0,0,PSPW,PSPH,ARGB(200,0,0,0));

	stars.render();//renderisamos lasestrellas primero pues son las que deben estar en el plano mas bajo.

	//poner en modo lightblend para que brillen..
	renderer->SetTexBlend(BLEND_SRC_ALPHA, BLEND_ONE);
	


	heroPlayer1.render();//dibujar la nave del heroe
	heroPlayer1.bullets.render();//dibujar las bajas del heroe
	enemigos.render();
	particles.render();
}


//-------------------------------------------------------------------------------------
// This function is called when the system wants to pause the game. You can set a flag
// here to stop the update loop and audio playback.
//
//-------------------------------------------------------------------------------------
void GameApp::Pause()
{
	GAMEPUSED_BY_SYSYTEM=true;
}


//-------------------------------------------------------------------------------------
// This function is called when the game returns from the pause state.
//
//-------------------------------------------------------------------------------------
void GameApp::Resume()
{
	GAMEPUSED_BY_SYSYTEM=false;
}

int GameApp::collide(int Hotx1,int Hoty1,int radius1,int Hotx2,int Hoty2, int radius2)//funcion para detectar colicion cuadrada..
//se recomienda utilizar colicion cuadrada ya que una colision radial consume mas tiempo...
{
	int Area1 = radius1*2;
	int Area2 = radius2*2;
	Hotx1 -=radius1;
	Hoty1 -=radius1;
	Hotx2 -=radius2;
	Hoty2 -=radius2;
	if(((Hotx1+Area1)>Hotx2)&&((Hoty1+Area1)>Hoty2)&&((Hotx2+Area2)>Hotx1)&&((Hoty2+Area2)>Hoty1))
		return 1;
	return 0;
}