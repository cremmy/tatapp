#include <cstdio>
#include <cstdlib>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

struct CharInfo
	{
	int x, y, w, h;
	bool empty;
	};
	
int getAlpha(SDL_Surface* img, int x, int y)
	{
	int *pixs=(int *)img->pixels;
	
	return (pixs[x+y*img->w]&img->format->Amask)>>img->format->Ashift;
	}
	
#undef main
	
int main(int argc, char *argv[])
	{
	char *name;
	int w, h, cw, ch;
	int chars, charsw, charsh;
	CharInfo *info;
	
	if(argc<6)
		{
		printf("%s [obrazek] [szerokosc] [wysokosc] [szerokosc znaku] [wysokosc znaku]", argv[0]);
		
		for(int i=' '; i<256; ++i)
			{
			printf("%c", i);
			}
		printf("\n");
		
		return 0;
		}
		
	name=argv[1];
	 w=atoi(argv[2]);
	 h=atoi(argv[3]);
	cw=atoi(argv[4]);
	ch=atoi(argv[5]);
	charsw=w/cw;
	charsh=h/ch;
	chars=charsw*charsh;
	
	//printf("  [%s]: %d %d %d %d; %d, %d; %d\n", name, w, h, cw, ch, charsw, charsh, chars);
	//fflush(stdout);
	
	info=new CharInfo[chars];

/******************************************************************************/
/****************** Wczytanie obrazka *****************************************/
/******************************************************************************/	
	SDL_Surface *img;
	img=IMG_Load(name);
	
	if(!img)
		{
		printf("FAIL: Nie udalo sie wczytac \"%s\"\n", name);
		printf("FAIL: %s", IMG_GetError());
		return 1;
		}
	//fflush(stdout);
	
	for(int i=0; i<chars; ++i)
		{		
		CharInfo& ci=info[i];
		
		ci.x=(i%charsw)*cw;
		ci.y=(i/charsw)*ch;
		ci.w=cw;
		ci.h=ch;
		ci.empty=false;
		
		//printf("> %d; %d %d %d %d\n", i, ci.x, ci.y, ci.w, ci.h);
		fflush(stdout);
		
		if(i==0)
			continue;
		
		bool found;
		
		// Sprawdzanie z lewej
		found=false;
		while(!found)
			{
			for(int y=ci.y; y<ci.y+ci.h; ++y)
				if( getAlpha(img, ci.x, y)>0 )
					{
					found=true;
					break;
					}
					
			if(!found)
				{
				++ci.x;
				--ci.w;
				}
				
			if(ci.w<=0)
				{
				break;
				}
			}
			
		// Sprawdzanie z prawej
		found=false;
		while(!found)
			{
			for(int y=ci.y; y<ci.y+ci.h; ++y)
				if( getAlpha(img, ci.x+ci.w-1, y)>0 )
					{
					found=true;
					break;
					}
					
			if(!found)
				--ci.w;

			if(ci.w<=0)
				{
				break;
				}
			}

		++ci.w;
		
		if(ci.w<=0)
			{
			ci.x=0;
			ci.y=0;
			ci.w=cw;
			ci.h=ch;
			ci.empty=true;
			}
		}

/******************************************************************************/
/****************** Wypisanie *************************************************/
/******************************************************************************/
	printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	printf("<sprite name=\"%s\">\n", name);
	printf("\t<image name=\"default\">%s</image>\n", name);
	fflush(stdout);
	for(int i=0; i<chars; ++i)
		{
		printf("\t<animation name=\"%d\"><!-- %c -->\n", i, ' ' +i);
		printf("\t\t<direction name=\"down\" fps=\"0\" loopto=\"0\">\n");
		if(info[i].empty)
			printf("\t\t\t<frame x=\"%d\" y=\"%d\" w=\"%d\" h=\"%d\" offsetx=\"0\" offsety=\"0\" /><!-- empty -->\n", info[i].x, info[i].y, info[i].w, info[i].h);
		else
			printf("\t\t\t<frame x=\"%d\" y=\"%d\" w=\"%d\" h=\"%d\" offsetx=\"0\" offsety=\"0\" />\n", info[i].x, info[i].y, info[i].w, info[i].h);
		printf("\t\t</direction>\n");
		printf("\t</animation>\n");
		}
		
	printf("</sprite>\n");
	
	return 0;
	}