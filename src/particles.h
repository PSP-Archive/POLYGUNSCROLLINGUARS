//libreria que controlalra las particulas //eta libreria ha sido estraida de POLYGUNWARS . por AHRIMANES
//puede ser adaptada acualquier otra libreria grafica..
//esta libreria no la comentare mucho ya que son casos my similares a los de las estrellas... para mas informacion ver star.h

#include <JRenderer.h>//inclorporacion del sistema grafico de jge.. parapoder hacer bliteos y carga de texturas.
#define MAXPARTICLES1 200//maximo de particulas tipo1
#define MAXPARTICLES2 600//maximo de particulas tipo2
#define PSPW 480//definimos la resolucion que tiene la psp.
#define PSPH 272//definimos la resolucion que tiene la psp.

class partic//clase que representa una particula
{
	public:
	//propiedades de las particulas
	float x,y,dx,dy,speed,angle,scale,scale1,alpha;//controlan las coordenadas y desplazamientos etc de las estrellas;
	PIXEL_TYPE col;//color que tiene la particula de sa en formato ARGB(a,r,g,b)
	bool dead;//controladi la particula vive o esta muerta.
	//metodos de clase
	partic(){}//constructor default
	
	partic(float x1,float y1,float speed1,float angle1,PIXEL_TYPE col1)//Constructor personalizado se ejecuta cuando instanciamos a la clase partic..
	{
		x=x1;//asignamos las cordenada x;
		y=y1;//asignamos las cordenada y;
		speed = speed1;//asignamos la velocidad;
		angle = angle1;//asignamos el angulo en que segero la particula
		alpha = 255;//asignamos la transparencia de la particula 255 totalmente visible.
		dx =sin(angle)*speed; //determinamos la direccion x atraves del seno del angulo * la velocidad asignada.
		dy =-cos(angle)*speed;//determinamos la direccion y atraves del seno del angulo * la velocidad asignada.
		scale = sqrt(dx*dx+dy*dy)/10;//se ajusta la escala para compensar la distancia de desplasamiento.
		scale1 = 1;//escala adicional
		col = col1;//asignamos el color
		dead = false;//la particula esta viva.
	}//partic
	~partic()//destructor de clase 
	{
	}	
};

class particlesys{//clase que controla las particulas
public:
	//propiedades de las particulas
	JTexture* mSpriteTex;//textura temporal para alvergar el sprite de la particula
	JQuad* img;//conenedor del sprite de la particula
	float mTimer;//control de tiempo
	float dTimer;//control de tiemp
	JRenderer* renderer;//variable en la que se instanciara el rendereador de jge++ //esto puede ser omitido si adaptando el codigo a otra libreria grafica.
	float friccion;//friccion factor que reducira la aceleracion de las particulas
	int lastempty;//indica la ultima particula vacia.
	vector<partic*> partlist;//vector para alojar las particulas nacidad de la clase partic..
	int Pvivas;//cantidad de particulas vivas.

	double myabs(double a)//fucnion que de determina el basoluto de un numero. puede ser sustitudia por la funcion abs de la libreria csdtlib
	{
		//return sqrt(a*a);
		if(a<0)
		{
			return a*-1;
		}else{return a;}
	}
	int myrand(int min, int max)//fincion que retorna un numero aleatorio entre un rango 
	{
		return rand()%(max-min) + min;
	}
	void init()//inicialisacion del sistema de particulas
	{
		renderer = JRenderer::GetInstance();//instancia del rendereador de jge++
		mTimer = 0;//se inicializa control de tiempo..
		dTimer = 0;//se inicializa control de tiempo..
		friccion=.90;//asignamos valor de la friccion..
		//se se carga el sprite a la textura temporar para despues asignarla al contenedor. y establecer su punto medio o de control.
		mSpriteTex = JRenderer::GetInstance()->LoadTexture("img/particle1.png", TEX_TYPE_USE_VRAM);
		img = new JQuad(mSpriteTex,0,0,mSpriteTex->mWidth,mSpriteTex->mHeight);
		img->SetHotSpot(img->mWidth/2, img->mHeight/2);
		
		for(int i = 0;i<MAXPARTICLES2;i++)//bucle for para inicialisar las particulas
		{
			partlist.push_back(new partic(0,0,0,0,ARGB(0,0,0,0)));//se abregan particulas al vector
			partlist[i]->dead=true;//se asigna la propiedad dead a true para que inicialmente todas las particulas estaran inactivas.
		}
		Pvivas=0;//0 particulas vivas.
	}//init

//apartir de este munto los comentarios no seran tan detallados pues ya son cosas muy comunes.. descritas en star.h

	partic* deadpart()//funcion que determina que particula puede ser utilizada ya que esta inactiva.
	{
		int size = partlist.size();
		for (int i=0;i<size;i++)
		{
			if (partlist[i]->dead)
				return partlist[i];
		}
		return NULL;
	}//deadpart

	void newparticle(int x,int y,float speed=1,float angle=0,PIXEL_TYPE col=ARGB(255,255,0,0))//metodo que inicialisa las particulas para hacerlas activas..
	{
			partic*par = deadpart();
			if (par != NULL)
			{
				par->x=x;
				par->y=y;
				par->speed = speed;
				par->angle = angle;
				par->dx =sin(angle)*speed; 
				par->dy =-cos(angle)*speed;
				par->speed=sqrt(pow(par->dx,2)+pow(par->dy,2))/8;
				par->col = col;
				par->dead = false;
				par->scale1 = 1;
			}
	}//newparticle
	void createExplo(int x,int y,PIXEL_TYPE col=ARGB(255,255,255,255),int NPart=15, float maxspeed=10)//funcion que crea multiples particulas para simular una explocion
	{
		for(int x1=0;x1<NPart;x1++)
		{
			newparticle(x,y,myrand(int(maxspeed*.666)+1,maxspeed),rand()%360,col);
		}
	}

	void render()//funcion que rendereara las particulas..
	{
		dTimer = 0;
		
		if(Pvivas>0)
		for(int i=0;i<partlist.size();i++)
		{
			if(!partlist[i]->dead)
			{
				img->SetColor(partlist[i]->col);
				renderer->RenderQuad(img  ,partlist[i]->x ,partlist[i]->y ,partlist[i]->angle ,partlist[i]->scale1,partlist[i]->scale*2);
			}
		}
	}//render


	void update(float dt) //actualizara las propiedades de las particulas
	{
		mTimer+=dt;
		dTimer += dt;
		float udtrate = 1.0f;
		if (mTimer<=udtrate)
		{
			return;
		}
		int size = partlist.size();
		partic *PT;	
		Pvivas = 0;
		for (int i=0;i<size;i++)
		{
			PT = partlist[i]; 
			if(!PT->dead)
			{
				Pvivas++;
				PT->x +=((PT->dx*=friccion)*1.3)*(mTimer/udtrate); 
				PT->y +=((PT->dy*=friccion)*1.3)*(mTimer/udtrate);
	
				if(PT->x<0||PT->x>PSPW){PT->dx=-PT->dx;PT->x +=PT->dx;	PT->angle = atan2(PT->dx,-PT->dy);}
				if(PT->y<0||PT->y>PSPH){PT->dy=-PT->dy;PT->y +=PT->dy;	PT->angle = atan2(PT->dx,-PT->dy);}
				
				PT->scale=(myabs(PT->dx)+myabs(PT->dy))*.07;
				if(PT->scale<.3)
				{
					PT->scale1=(myabs(PT->dx)+myabs(PT->dy))*.07;
				}
				if(PT->scale<.10){PT->dead = true;}
			}
		}
		mTimer=0;
	}//update
};//class partsys
