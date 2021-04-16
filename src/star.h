//libreria que controla estrellas. por AHRIMANES
//puede ser adaptada acualquier otra libreria grafica..
#include <JRenderer.h>//inclorporacion del sistema grafico de jge.. parapoder hacer bliteos y carga de texturas.
#include <vector>//para poder usar vectores
#define PSPW 480//definimos la resolucion que tiene la psp.
#define PSPH 272//definimos la resolucion que tiene la psp.

class star{//clase star representa 1 sola estralla..
public:
	//propiedades de la estrella
	float x,y,dx,dy,z; //controlan las coordenadas y desplazamientos de las estrellas asi como su profundidad.
	//metodos de clase
	star(void)//metodo constructor se ejecuta al crearse una instancia de la clase star. ver starSyst.
	{
		x=rand()%PSPW;//x se genera de forma aleatoria en el rango de 0 al valor PSPW
		y=rand()%PSPH;//y se genera de forma aleatoria en el rango de 0 al valor PSPW
		z=(rand()%4)+1;//aligual que las coordenadas pero ente 1 y 5 lo que significa que tendremos 5 planos de estrllas en pantalla
		//podemos jugar con estos valores para acelerar o cambie el flujo de las estrellas
		dy=4/z;//la velocidad de despalzamiento vertical de la estrella esta en fucnion del plano"z" que le toca siendo la estrlla mas rapida aqulla su velocidad es 4
		dx=0;//no hara ningun desplasamiento en x
	}
	void reset(void){//se debe ejecutar cadaves que una estrella sale de la pantalla.. ver starSyst.
		//podemos jugar con estos valores para acelerar o cambie el flujo de las estrellas
		x=rand()%PSPW;//x se genera de forma aleatoria en el rango de 0 al valor PSPW
		y=0;//ponemos la estrella hasta arriva de la pnatalla.
		//z=(rand()%4)+1;//reubicamos la estrella en algun plano. no es nescesario puesto que ya pertenece a un plano desde que se creeo..
		//dy=4/z;//tampoco es nescesario reasignar una velocidad pues ya la tiene desde que se creo.
		//dx=0;//no hara ningun desplasamiento en x
	}
};

class starSyst{//clase controladora de una lista de estrellas
public:
	//propiedades del sistema de estrellas
	vector<star*> starList;//vector para alojar las estrellas nacidad de la clase star..
	JRenderer* renderer;//variable en la que se instanciara el rendereador de jge++ //esto puede ser omitido si adaptando el codigo a otra libreria grafica.
	//metodos de clase
	void init(void){//la ejecutaremos para inicializar el sistema de estrellas.
		renderer = JRenderer::GetInstance();//instancia del rendereador de jge++
		for(int i=0;i<300;i++)//un bucle de 0 a 299 para crear 300 estrellas
			starList.push_back(new star());//de agregan estrellas a al vector.
	}
	void update(float dt){//metodo que actualisara las propiedades de las estrellas.
		for(int i=0;i<starList.size();i++)//bucle que rrecorrera todos los nodos del vector. cada nodo es una instacia de la calse star.
		{
			star* ST = starList[i];//hacemos un apuntador al nodo en i para ahorrarnoes escrivir todo el tiempo starList[i];
			ST->x+=ST->dx*dt;//actualizamos la propiedad x, la multiplicacion * dt se utiliza para compensar eltiempo 
			ST->y+=ST->dy*dt;//actualizamos la propiedad y, la multiplicacion * dt se utiliza para compensar eltiempo 
			if(ST->y>PSPH)//si la estrella salio de la pnatalla la reiniciamos..
			{
				ST->reset();//reiniciar la estrella.
			}
		}
	}
	void render(void){//netodo de rendereado de las estrellas que exiten en el vector starList
		for(int i=0;i<starList.size();i++)//bucle que rrecorrera todos los nodos del vector. cada nodo es una instacia de la calse star.
		{
			star* ST = starList[i]; //hacemos un apuntador al nodo en i para ahorrarnoes escrivir todo el tiempo starList[i];
			//este bloke plotea 5 pixeles que formans una crus para simular una estrella 
			renderer->Plot(ST->x,ST->y,ARGB(255,int(255/ST->z),int(255/ST->z),int(255/ST->z)));
			renderer->Plot(ST->x-1,ST->y,ARGB(255,int(255/ST->z),int(255/ST->z),int(255/ST->z)));
			renderer->Plot(ST->x+1,ST->y,ARGB(255,int(255/ST->z),int(255/ST->z),int(255/ST->z)));
			renderer->Plot(ST->x,ST->y-1,ARGB(255,int(255/ST->z),int(255/ST->z),int(255/ST->z)));
			renderer->Plot(ST->x,ST->y+1,ARGB(255,int(255/ST->z),int(255/ST->z),int(255/ST->z)));
		}	
	}
};