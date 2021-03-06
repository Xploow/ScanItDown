#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include "segstruct.h"
#include <SDL_image.h>
#include "pixel_operations.h"
#include "pixel_operations.c"
#include "filter.h"
#include "filter.c"
void DrawColumn(SDL_Surface *img, int beginx, int beginy,int end)
{
	int y; 
	for (y=beginy; y< end;y++)
        putpix(img,beginx,y , SDL_MapRGB(img->format, 0,255,0));
}
void DrawColumnB(SDL_Surface *img, int beginx, int beginy,int end)
{
        int y;
        for (y=beginy; y< end;y++)
        putpix(img,beginx,y , SDL_MapRGB(img->format, 255,0,0));
}
void DrawLineB(SDL_Surface *img, int beginx, int beginy,int end)
{
        int x; 
        for (x=beginx; x< end;x++)
        putpix(img,x,beginy , SDL_MapRGB(img->format, 255,0,0));
}
int segline(SDL_Surface *img,struct line *line_tab,int *sizetab
		,struct Bloc Bloc)
{
	//Variable
	int x,y ;
	int onLine=0;
	for(y=Bloc.yb ; y<=Bloc.ye ; y++)
	{
		//Parcour l'image en width, si pixel = blanc,continue, sinon
		for (x=Bloc.xb;
			(getpix(img,x,y)==SDL_MapRGB(img->format,255,255,255))
			&& (x< Bloc.xe); 
			x++);
		if(x!= Bloc.xe && onLine==0) 
		{
			//DrawLine(img,y-1);
			DrawLineB(img,Bloc.xb,y-1,Bloc.xe);				
			onLine = 1; //on est sur une ligne
			line_tab[*sizetab].yb= y-1;
		}
		if(x== Bloc.xe && onLine==1)
		{
			DrawLineB(img,Bloc.xb,y-1,Bloc.xe);
			//DrawLine(img,y);
			onLine = 0 ;
			line_tab[*sizetab].ye=y;
			*sizetab=*sizetab+ 1;
		}
	}
	return 0;
}
int segletter(SDL_Surface *img,struct line line, struct letter *letter_tab,
		int *sizetab,struct Bloc Bloc)
{
int x,y ;
int onLetter=0;
for(x=Bloc.xb ; x <Bloc.xe; x++)
{
       for (y=line.yb +1;
	   ((getpix(img,x,y)==SDL_MapRGB(img->format,255,255,255))||
	   getpix(img,x,y)==SDL_MapRGB(img->format,255,0,0)) && (y< line.ye);
				y++);
		if(onLetter==0 && y!= line.ye)
		{	
			DrawColumn(img,x,line.yb,line.ye);
			letter_tab[*sizetab].xb=x;
			onLetter=1;	
		}
		if((onLetter==1) &&(y ==line.ye))
		{
			DrawColumn(img,x,line.yb,line.ye);
			letter_tab[*sizetab].xe=x;
			*sizetab +=1;
			onLetter =0;
		}
	}
	return 0;
}
int segblocY (SDL_Surface *img, struct blocY *blocY_tab,int bloc_tabsize)
{
	int x,y;
	int onBlocy = 0;
	for(x=0 ; x<=img->w ; x++)
        {
                //Parcour l'image en width, si pixel = blanc,continue, sinon
                for (y=0;
                     (getpix(img,x,y)==SDL_MapRGB(img->format,255,255,255))
		     &&(getpix(img,x+5,y)==SDL_MapRGB(img->format,255,255,255))
		     && (y< img->h);
                                y++);
		if(onBlocy==0 && y!= img ->h)
		{
		//	DrawColumnB(img,x,0,img->h);
			onBlocy=1;
			blocY_tab[bloc_tabsize].xb=x;
			
		}
		if((y==img->h) && onBlocy==1)
		{
		//	DrawColumnB(img,x,0,img->h);
			onBlocy =0;
			blocY_tab[bloc_tabsize].xe=x;
			bloc_tabsize++;
		}	
	}
	return bloc_tabsize;
}
int segblocX (SDL_Surface *img, struct blocX *blocX_tab,int bloc_tabsize)
{
        int x,y;
        int onBlocx = 0;
        for(y=0 ; y<=img->h ; y++)
        {
                //Parcour l'image en width, si pixel = blanc,continue, sinon
                for (x=0;
                    (getpix(img,x,y)==SDL_MapRGB(img->format,255,255,255))
                     &&(getpix(img,x,y+7)==SDL_MapRGB(img->format,255,255,255))
                     &&(x< img->w);
		     x++);
		if(x!= img->w && onBlocx==0 )
		{
			//  DrawLineB(img,0,y,img->w);
			onBlocx=1;
                        blocX_tab[bloc_tabsize].yb=y;

                }
		if(x==img->w && onBlocx==1)
		{
                     //   DrawLineB(img,0,y,img->w);
                        onBlocx =0;
                        blocX_tab[bloc_tabsize].ye=y;
                        bloc_tabsize++;
                }
        }
        return bloc_tabsize;
}
int segBloc(SDL_Surface *img, struct blocX *blocX_tab,int sizeblocX,
struct blocY *blocY_tab,int sizeblocY, struct Bloc *Bloc_tab, int bloc_tabsize)
{
	int x,y;
	int i=0;
	int onBlocx=0;
	blocX_tab[sizeblocX+1].ye =3;
	sizeblocY +=0;
//	int onBlocy=0;
	for(i=0; i<=sizeblocX;i++) //On détecte y grace à segBlocx
	{
		//for(y=blocY_tab[i].xb; y<=blocY_tab[i].xe;y++)
	    for(y=0; y<=img->h;y++)
	    {
		for (x=blocY_tab[i].xb;
                    (getpix(img,x,y)==SDL_MapRGB(img->format,255,255,255))
                     &&(getpix(img,x,y+7)==SDL_MapRGB(img->format,255,255,255))
                     && (x< blocY_tab[i].xe); x++)
			if(x!= blocY_tab[i].xe && onBlocx==0)
			{
			  DrawLineB(img,blocY_tab[i].xb,y,blocY_tab[i].xe);
			  onBlocx=1;
			  Bloc_tab[bloc_tabsize].yb=y+1;
			}
			if(x==blocY_tab[i].xe && onBlocx==1)
			{
			   DrawLineB(img,blocY_tab[i].xb,y,blocY_tab[i].xe);
			   onBlocx=0;
			   Bloc_tab[bloc_tabsize].ye=y-1;
			   bloc_tabsize++;
			}
		}		
	}
	return bloc_tabsize;
}
int segBloc2(SDL_Surface *img, struct Bloc *Bloc_tab,int Bloc_size, 
	     int bloc_tabsize)
{
int x,y,j;
int onBlocy=0;
//On trace les colonnes
for(j=0; j<= Bloc_size;j++)
{
	for(x=0; x<=img->w;x++)
	{
	   for (y=Bloc_tab[j].yb;
		   (((getpix(img,x,y)==SDL_MapRGB(img->format,255,255,255))
		   &&(getpix(img,x+10,y)==SDL_MapRGB(img->format,255,255,255)))
		   ||(getpix(img,x,y)==SDL_MapRGB(img->format,255,0,0)))
		   &&(y< Bloc_tab[j].ye);
		   y++);
			if(y!= Bloc_tab[j].ye && onBlocy==0)
			{
			  DrawColumnB(img,x,Bloc_tab[j].yb,Bloc_tab[j].ye);
			  onBlocy=1;
			  Bloc_tab[bloc_tabsize].xb=x;
			}
			if(y==Bloc_tab[j].ye && onBlocy==1)
			{
			  DrawColumnB(img,x,Bloc_tab[j].yb,Bloc_tab[j].ye);
			  onBlocy=0;
			  Bloc_tab[bloc_tabsize].xe=x;
		          bloc_tabsize++;
			}
		}
	}
	return bloc_tabsize;
}
int main()
{
	//Variable
	char filename_in[256];
	char filename_out[256];
	SDL_Surface *scr ;
	//-----------------pour segline
	int size=0;
	int *sizetab=&(size);
	struct line line_tab[10000];
	//	struct line p;
	//	line_tab[0]=p;
	//----------------pour segletter
	struct letter letter_tab[10000];
	//---------------------pour segBlocy
	struct blocY blocY_tab[10000];
	//int sizeblocY=0;
	//---------------------pour segBlocx
	struct blocX blocX_tab[10000];
	//int sizeblocX=0;
	//---------------------pour segBloc
	struct Bloc Bloc_tab[10000];
	//---------------------
	struct Bloc Bloc_tab2[10000];

	/*Renseigne le nom de l'image*/
	printf("\nEntrez le nom du fichier : ");
	fgets(filename_in, sizeof filename_in, stdin);
	filename_in[strlen(filename_in) - 1] = 0;
	printf("\nEntrez le nom du fichier de sortie: ");
	fgets(filename_out, sizeof filename_out, stdin);
	filename_out[strlen(filename_out) - 1] = 0;

	SDL_Surface* bmp = IMG_Load(filename_in);
	SDL_Surface* imgforseg =IMG_Load(filename_in);
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		return 1;
	if(!(scr = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE)))
		return 2;

	grayscale(bmp);
	SDL_SaveBMP(bmp,"grayscale");
	binarization(bmp);
	SDL_SaveBMP(bmp,"binarization");
	binarization(imgforseg);

	/////////////BLOC_TAB/////////////////////


	//int sizeBloc2=0;
	Bloc_tab2[0].xb=0;
	Bloc_tab2[0].xe=bmp->w;
	Bloc_tab2[0].yb=0;
	Bloc_tab2[0].ye=bmp->h;

	///////////////////////////////////	

	int sizeblocY=	segblocY(imgforseg,blocY_tab,0);
	int sizeblocX=	segblocX(imgforseg,blocX_tab,0);
	int sizeBloc= segBloc(imgforseg,blocX_tab,sizeblocX,blocY_tab,sizeblocY
	,Bloc_tab,0);
	int sizeBloc2= segBloc2(imgforseg,Bloc_tab,sizeBloc,0);
	sizeBloc2 +=0;
	SDL_SaveBMP(imgforseg,"segbloc");
	
	int j=0;
	segline(bmp,line_tab,sizetab,Bloc_tab2[j]);
	for(int i =0; i <size; i++) 
	{	
		segletter(bmp,line_tab[i],letter_tab,sizetab,Bloc_tab2[j]);
	} 
	
	SDL_SaveBMP(bmp,filename_out);
	//SDL_BlitSurface(bmp, 0, scr, 0);
	//SDL_BlitSurface(imgforseg, 0, scr, 0);
	//SDL_Flip(scr);
	SDL_FreeSurface(bmp);
	SDL_FreeSurface(imgforseg);
	SDL_Quit();
	return 0;
	while(1) {
		SDL_Event e;
		SDL_WaitEvent(&e);
		if(e.type == SDL_QUIT)
			break;
	}	
}
