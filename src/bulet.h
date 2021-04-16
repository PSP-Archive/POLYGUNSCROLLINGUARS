//libreria para control de balas. por AHRIMANES
//esta libreria debera ser incluida en toda otra libreria o programa en que se desen usar balas.
//vease nave.h

//se omitiran cosas que son obias.. para mas informacion ver star.h

#include <JRenderer.h>
#include <vector>
#define PSPW 480
#define PSPH 272

//clase bullet para cada bala que exista en el juego existiran las propiedades de la clase.
class bullet{
public:
	float x,y,dx,dy;
	bullet(float x1,float y1,float dx1, float dy1)
	{
		x = x1;
		y = y1;
		dx = dx1;
		dy = dy1;
	}
};

//clase encargada de controlar las balas... 
//la ventaja de ser una clase es que puedes crear tantos controladores de balas como quieras...
//como por ejeplo el controlador de balas de los enemigos y el del heroe.
class bullet_sys{
public:
	JQuad* img;
	vector<bullet*> bulletlist;//vector para alojar las balas nacidas de la clase bullet..
	JRenderer* renderer;
	void init(char *imageUrl)//inicializa el controlador de las balas basicamente lo que hace es asignar la imagen que las balas van a tener...
	{
		//si queisieramos tener un opgrade de armas pos entonces solo llamamos esta funcion y cambiamos la imagen...
		JTexture* mSpriteTex; //temporal para Texturas
		renderer = JRenderer::GetInstance();//instancia del rendereador de jge++
		mSpriteTex = renderer->LoadTexture(imageUrl, TEX_TYPE_USE_VRAM);//cargamos la imagen ala textura temporar
		img = new JQuad(mSpriteTex,0,0,mSpriteTex->mWidth,mSpriteTex->mHeight);//asignamos la imagen a la imagen que la controlara.
		img->SetHotSpot(img->mWidth/2, img->mHeight/2);//ponemos el punto de control al centro de la imagen
	}
	
	void addbullet(float x,float y, float dx,float dy)//agregar una bala a la lista
	{
		bulletlist.push_back(new bullet(x,y,dx,dy));
	}

	void update(float dt)//metodo para actualizar las propiedades de las balas
	{
		for(int i=0;i<bulletlist.size();i++)
		{
			bullet* BLT = bulletlist[i];//creamos un apuntador para no tener que escrivir todo el rato bulletlist[i].x etc
			BLT->x+=BLT->dx*dt;
			BLT->y+=BLT->dy*dt;
			if(BLT->x < 0-20||BLT->x >PSPW+20||BLT->y < 0-20||BLT->y >PSPH+20){//si la bala salio de la pantalla hay que destruirla.
				BLT = NULL;//des ocupamos BLT para evitar problemas de apuntar a otrolado que no sea el correcto.."nos libramos de desbordamientos de memoria.."
				bulletlist.erase(bulletlist.begin()+i);//se ilimina la bala cuyo indice seai
			}
		}
	}
	void render(void)//funcion para renderear las balas
	{
		renderer = JRenderer::GetInstance();//instancia del rendereador de jge++
		for(int i=0;i<bulletlist.size();i++)
		{
			bullet* BLT = bulletlist[i];//creamos un apuntador para no tener que escrivir todo el rato bulletlist[i].x etc
			renderer->RenderQuad(img  ,BLT->x ,BLT->y ,0,.5,.5); //se dibuja la bala en turno
			BLT=NULL;
		}
	}
};