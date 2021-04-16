//libreria que controlara todo aquel objeto que sea una nave... por AHRIMANES
//los comentarios seran breves pues muchas de las cosas que se veran aqui ya han sido explicadas en otras librerias.. vease star.h
#include <JRenderer.h>
#include "bulet.h" //incluimos la libreria controladora de balas

#define PSPW 480
#define PSPH 272
#define maxEneTp1 10//definicin de cuantos enemigos del tipo1 puenden existir
#define maxEneTp2 10

class nave { //clase que descrive las propiedades de una nave deforma generica.
public:
float x,y;
float dx,dy,r;
float sx,sy;
float shotingPauseTime;
float shotingTimeElapsed;
bullet_sys bullets; //instanciamos la clase bullet_sys para crear el controlador de las balas de la nave.


	nave(void){//constructor de la nave.. podriamos poner aqui las cordenadas x y iniciales. se ejecuta cuando se crea una instancia de la clase nave.
		shotingPauseTime = 5;
		shotingTimeElapsed = 0;
		dx=0;
		dy=0;
		sx =1;
		sy =1;
		r=0;
	}
};

class naveHero:public nave{//clase que controlara las naves de tipo heroe es heredada de la clase nave por tanto posee las propiedades y metos de dicha clase + los porpios.
public:
	JQuad* img;
	JTexture* mSpriteTex; //temporal para Texturas
	JRenderer* renderer; //instancia del rendereador de jge++
	
	void update(float dt){//metodo que actualizara y controlara el estado de la nave
		if(dx<-5){dx=-5;}
		if(dx>5){dx=5;}
		if(x<0-20){x=PSPW+20;}
		if(x>PSPW+20){x=0-20;}
		if(dx>0){dx-=0.05*dt;}
		if(dx<0){dx+=0.05*dt;}
		//*****************************
		x+=dx*dt;
		y+=dy*dt;
	}
	
	void init(void){//metodo que inicialisa la nave, asigana la imagen que se usara como sprite de la nave etc.
		renderer = JRenderer::GetInstance();//instancia del rendereador de jge++
		//esto tambien tiene que hacerce de otra forms.. para explotar bien el concepto de las claces.
		x = PSPW/2;
		y = PSPH-40;
		bullets.init("img/shot_1.png");//inicialisamos el sistema de balas de la nave
	//************************	
		mSpriteTex = renderer->LoadTexture("img/ship_2.png", TEX_TYPE_USE_VRAM);//cargamos la imagen ala textura temporar
		img = new JQuad(mSpriteTex,0,0,mSpriteTex->mWidth,mSpriteTex->mHeight);//asignamos la imagen a la imagen que la controlara.
		img->SetHotSpot(img->mWidth/2,img->mHeight/2);//ponemos el punto de control al centro de la imagen
	}
	void reset(void)
	{
		x = PSPW/2;
		y = PSPH-40;
		dx = 0;
	}
	void render(void){//metodo para renderear la nave hero tipo
		renderer = JRenderer::GetInstance();//instancia del rendereador de jge++
		renderer->RenderQuad(img,x,y,r,sx*1.2,sy*1.2);
	}

};

class naveEnemyType1:public nave{//clase que controlara las naves enemigas de tipo 1 es heredada de la clase nave por tanto posee las propiedades y metos de dicha clase + los porpios.
public:
	void regen(void){//cuando una nave enemiga muere se reasigna una posicion para que vuelva a nacer.
		x=(rand()%PSPW-10)+5;
		y=(rand()%PSPH)-272;
		dx=0;
		dy=2;
	}

	void update(float dt){//se actualizan y controlan los estados de la nave.
		x+=dx*dt;
		y+=dy*dt;
		if(y>PSPH+40)
			regen();
		r+=0.1*dt;
		if(r>3.1416*2){r=0;}
	}
};

class naveEnemyType2:public nave{//clase que controlara las naves enemigas de tipo  es heredada de la clase nave por tanto posee las propiedades y metos de dicha clase + los porpios.
public:
	bool flag1,flag2;//controlaral en estado de modificacion de escala de la imagen de la nave

	void regen(void){//cuando una nave enemiga muere se reasigna una posicion para que vuelva a nacer.
		x=(rand()%PSPW-10)+5;
		y=(rand()%PSPH)-272;
		dx=0;
		dy=2;
	}
	
	void update(float dt){//se actualizan y controlan los estados de la nave.
		x+=dx*dt;
		y+=dy*dt;
	
		if(y>PSPH+40)
			regen();
		if(flag1==false){
			sx +=.2*dt;
			if(sx>=1.5){flag1 = true;}
		}else{
			sx -=.1*dt;
			if(sx<=1){flag1 = false;}
		}
		if(flag2==false){
			sy +=.2*dt;
			if(sy>=1.5){flag2 = true;}
		}else{
			sy -=.3*dt;
			if(sy<=1){flag2 = false;}
		}
	}
	void toward(double xx,double yy,int range,float dt)// funcion que determina el desplasamiento que la nave debe tener para segir al punto x,y que generalmente seran las xy del heroe.
	{
		double ddx = xx-x;
		double ddy = yy-y;
		double dist = sqrt(ddx*ddx + ddy*ddy)+0.001;
		if (dist < range)
		{
			dx = dx + ((ddx/dist)*0.05)*dt;
			dy = dy + ((ddy/dist)*0.02)*dt;
		}
	}
};

class enemySyst{//clase controladora de enemigos.
public:
	//propiedades de clase;
	naveEnemyType1 enemigosTP1[maxEneTp1];//instanciamos un array con N enemigos del tipo 1.
	naveEnemyType2 enemigosTP2[maxEneTp2];
	JTexture* mSpriteTex; //temporal para Texturas
	JRenderer* renderer; //instancia del rendereador de jge++
	JQuad* imgEnemigosTP1;//imagen del enemigo tipo 1
	JQuad* imgEnemigosTP2;
	
	void init(void){//etodo que inicialisa el sistema controlador de enemigos
			//carga de las imagenes.
			renderer = JRenderer::GetInstance();//instancia del rendereador de jge++
			mSpriteTex = renderer->LoadTexture("img/enemy01.png", TEX_TYPE_USE_VRAM);//cargamos la imagen ala textura temporar
			imgEnemigosTP1 = new JQuad(mSpriteTex,0 , 0,mSpriteTex->mWidth,mSpriteTex->mHeight);//asignamos la imagen a la imagen que la controlara.
			imgEnemigosTP1->SetHotSpot(imgEnemigosTP1->mWidth/2,imgEnemigosTP1->mHeight/2);//ponemos el punto de control al centro de la imagen

			mSpriteTex = renderer->LoadTexture("img/enemy02.png", TEX_TYPE_USE_VRAM);//cargamos la imagen ala textura temporar
			imgEnemigosTP2 = new JQuad(mSpriteTex,0 , 0,mSpriteTex->mWidth,mSpriteTex->mHeight);//asignamos la imagen a la imagen que la controlara.
			imgEnemigosTP2->SetHotSpot(imgEnemigosTP2->mWidth/2,imgEnemigosTP2->mHeight/2);//ponemos el punto de control al centro de la imagen


		for(int i =0;i<maxEneTp1;i++)
		{
			enemigosTP1[i].x=(rand()%PSPW-10)+5;//en cualquier lado
			enemigosTP1[i].y=(rand()%PSPH)-272;//con esto se generaran mas arriva de la pantalla.
			enemigosTP1[i].dy = 2; //esto tiene que hacerce de otra forma.
		}
		for(int i =0;i<maxEneTp2;i++)
		{
			enemigosTP2[i].x=(rand()%PSPW-10)+5;//en cualquier lado
			enemigosTP2[i].y=(rand()%PSPH)-272;//con esto se generaran mas arriva de la pantalla.
			enemigosTP2[i].dy = 2; //esto tiene que hacerce de otra forma.
		}
	}

	void update(float dt,float x,float y){//metodo para actualizar los estados y propiedades de las naves enemigas.
		for(int i =0;i<maxEneTp1;i++)
			enemigosTP1[i].update(dt);
		for(int i =0;i<maxEneTp2;i++){
			enemigosTP2[i].toward(x,y,300,dt);
			enemigosTP2[i].update(dt);
		}
	}
	void render(void){//metodo para renderear las naves enemiga.
		for (int i =0;i<maxEneTp1;i++)//dibujar los enemigos
			renderer->RenderQuad(imgEnemigosTP1,enemigosTP1[i].x,enemigosTP1[i].y,enemigosTP1[i].r,enemigosTP1[i].sx,enemigosTP1[i].sy);
		for (int i =0;i<maxEneTp2;i++)//dibujar los enemigos
			renderer->RenderQuad(imgEnemigosTP2,enemigosTP2[i].x,enemigosTP2[i].y,enemigosTP2[i].r,enemigosTP2[i].sx,enemigosTP2[i].sy);
	}
};