#include<allegro.h>
#include<stdio.h>

#define MAXLEVELS 3
#define BRICKSiZE 40

int x,y,dx,dy;
int pix,piy;
int points;
int lives;
int clevel;
bool gameover;
char level[100][100];
BITMAP *dave_neutral;
BITMAP *dave_left;
BITMAP *dave_right;
BITMAP *fire;
BITMAP *door;
BITMAP *diamond;
BITMAP *brick;
int textout_hack( BITMAP *bmp , FONT* font , int x , int y , double multiplier , char *msg , int color )
{
  BITMAP *tmp;
  tmp = create_bitmap( text_length(font, msg), text_height(font));
  if(!tmp)
    return FALSE; 
  clear_to_color( tmp , makecol( 0 , 0 , 0) );
  textout_ex( tmp, font, msg, 0, 0, color,-1) ;
  masked_stretch_blit( tmp , bmp , 0 , 0 , tmp -> w , tmp -> h , x , y , (int)(tmp -> w * multiplier), (int)(tmp -> h * multiplier)); 
  destroy_bitmap( tmp );
  return TRUE;
}

void display_welcome()
{
  clear_to_color(screen,makecol(0,0,0));
  textout_hack(screen,font,170,300,3,"Welcome..",makecol(255,255,255));
  textout_hack(screen,font,80,400,2,"Press any key to continue..",makecol(255,255,255));
  readkey();
}


void display_lvl()
{
  clear_to_color(screen,makecol(0,0,0));
  textout_hack(screen,font,100,300,3,"Congratulations",makecol(255,255,255));
  textout_hack(screen,font,80,400,2,"Press any key to continue..",makecol(255,255,255));
  //  rest(30000);
  readkey();
}

void display_over()
{
  clear_to_color(screen,makecol(0,0,0));
  textout_hack(screen,font,150,300,3,"Game Over",makecol(255,255,255));
  textout_hack(screen,font,80,400,2,"Press any key to continue..",makecol(255,255,255));
  //  rest(30000);
  readkey();
}


void load_level(int lvl,bool load)
{
  FILE *f;
  char filename[10];
  char ch;
  int i,j;
  sprintf(filename,"level%d.txt",lvl);
  fprintf(stderr,"%s",filename);
  f=fopen(filename,"r");
  for(i=0;i<13;i++)
    {
      for(j=0;j<15;j++)
	{
	  fscanf(f,"%c",&ch);
	  //  printf("%c",ch);
	  if(ch=='\n')
	    {
	      j--;
	      continue;
	    }
	  else
	    {
	      if(ch=='S')
		{
		  x=j;
		  y=i;
		}
	      if(load)
		{
		  level[i][j]=ch;
		  /*if(level[i][j]=='S'){
		    x=j;
		    y=i;
		  }
		  if(level[i][j]=='E'){
		    dx=j;
		    dx=i;
		  } */
		}
	    }
	}
    }
}

void display(int d);

int playgame()
{
  int o=0; //orientation
  int time=0;
  bool downallow=true;
  bool jumping=false;
  bool upallow=true;
  bool rallow=true;
  bool lallow=true;
  bool rmov=false;
  bool lmov=false;
  int ret;
  bool levelover=false;
  while ( !levelover && !gameover){
    //	rectfill( screen, 50, 105, 100, 110, makecol( 12, 34, 200));
    //	line( screen, 0, 120, 150, 120, makecol( 255, 0, 0));
    
    display(o);
    o=0;
    clear_keybuf();
    acquire_screen();
    //  rectfill(screen,40*x+pix,40*y+piy, 40*x+40+pix,40*y+40+piy, makecol( 0, 0, 0));        
    //textout_ex( screen, font, " ", 40*x+pix, 40*y+piy, makecol( 0, 0, 0), makecol( 0, 0, 0) );
    
    if(key[KEY_ESC])
      gameover=true;

    if(level[y][x]=='E'||(pix>0 && level[y][x+1]=='E')||(piy>0 && level[y+1][x]=='E'))
      {
	levelover=true;
	ret=1;
	//	display_lvl();
	//break;
      }
    if(level[y][x]=='F'||(pix>0 && level[y][x+1]=='F')||(piy>0 && level[y+1][x]=='F'))
      {
	levelover=true;
	ret=0;
      }

    if(level[y][x]=='o')
      {
	points+=10;
	//	clear_to_color(screen,makecol(0,0,0));
	level[y][x]='.';
      }
    if((pix!=0 && level[y][x+1]=='o'))
      {
	points+=10;
	//	clear_to_color(screen,makecol(0,0,0));
	level[y][x+1]='.';
      }
    if( (piy!=0 && level[y+1][x]=='o'))
      {
	points+=10;
	//	clear_to_color(screen,makecol(0,0,0));
	level[y+1][x]='.';
      }
    if(!jumping && downallow){
      piy++;
      if(piy==40){
	piy=0;
	y++;
      }
    }
    if(jumping && upallow){
      time++;
      piy--;
      if(piy==-1){
	piy=39;
	y--;
      }
      if(time==81){
	time=0;
	jumping=false;
      }
    }
    if(rmov){
      pix++;
      o=1;
      if(pix==40){
	pix=0;
	x++;
      }
      rmov=false;
    }
    if(lmov){
      pix--;
      o=-1;
      if(pix==-1){
	pix=39;
	x--;
      }
      lmov=false;
    }
    if(level[y+1][x]=='#'||(pix>0&&level[y+1][x+1]=='#'))downallow=false;
    else downallow=true;
    if(x==1 && y==5)
      printf("Here I am !!\n");
    if(piy==0&&(level[y-1][x]=='#'||(pix>0 && pix<39 && level[y-1][x+1]=='#'|| (pix==0 &&level[y-1][x-1]=='#')))){
      if(x==1 && y==5)
	printf("Here I am world!!\n");
      upallow=false;
      jumping=false;
      //time=0;
    }
    else upallow=true;
    if(pix==0&&(level[y][x+1]=='#'||(piy>0  && level[y+1][x+1]=='#')))rallow=false;
    else rallow=true;
    if(pix==0&&(level[y][x-1]=='#'||(piy>0  && level[y+1][x-1]=='#')))lallow=false;
    else lallow=true;
    
    if (key[KEY_UP] && !downallow ) 
      {
	jumping=true;
	downallow=true;
      }
    else if (key[KEY_RIGHT] && rallow) rmov=true;
    else if (key[KEY_LEFT] && lallow) lmov=true;
    
    
    printf("X %d %d Y %d %d\n",x,pix,y,piy);
    
    //textout_ex( screen, font, "@", 40*x+pix, 40*y+piy, makecol( 255,255, 255), makecol( 0, 0, 0) );
    
    release_screen();
    
    rest(1);
    
  }
  return ret;
}

int main(){
  allegro_init();
  install_keyboard();
  set_gfx_mode( GFX_AUTODETECT,600,600, 0, 0);
  install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
  set_volume(255, 255);
  display_welcome();
  //readkey();
  dave_left = load_bitmap( "dave_left.bmp", NULL);
  dave_right = load_bitmap( "dave_right.bmp", NULL);
  dave_neutral = load_bitmap("dave_neutral.bmp",NULL);
  fire = load_bitmap("fire.bmp",NULL);
  door = load_bitmap("door.bmp",NULL);
  diamond = load_bitmap("diamond.bmp",NULL);
  brick = load_bitmap("brick.bmp",NULL);
  pix=4;
  piy=2;
  points=0;
  lives=3;
  clevel=1;
  gameover=false;
  bool load=true;
  while(clevel<=MAXLEVELS && !gameover && lives>0)
    {
      
      load_level(clevel,load);
      if(playgame()==1)
	{
	  clevel++;
	  display_lvl();
	  load=true;
	}
      else
	{
	  rest(3000);
	  lives--;
	  load=false;
	}
    }
  if(lives==0 || gameover)
    {
      display_over();
    }
  else
    display_lvl();
  // readkey();
  //display_lvl();
  return 0;
}
END_OF_MAIN(); 

void display(int d){
  int x1,y1;
  BITMAP *buffer;
  char line[50];

  //SAMPLE *son ;
  //son= load_sample("walk.wav"); // or .voc
  buffer = create_bitmap(SCREEN_W,SCREEN_H);
  if(d==0)
    draw_sprite(buffer,dave_neutral,BRICKSiZE*x+pix,BRICKSiZE*y+piy);
  else if(d==1)
    {
      draw_sprite(buffer,dave_right,BRICKSiZE*x+pix,BRICKSiZE*y+piy);
      //  if (son!=0) play_sample(son, 255, 128, 1000, 0);
    }
  else if(d==-1)
    {
      draw_sprite(buffer,dave_left,BRICKSiZE*x+pix,BRICKSiZE*y+piy);
      // if (son!=0) play_sample(son, 255, 128, 1000, 0);
    }
  for(y1=0;y1<13;y1++){
    for(x1=0;x1<15;x1++){
      if(level[y1][x1]=='#'){
	draw_sprite(buffer,brick,BRICKSiZE*x1,BRICKSiZE*y1);
	//rectfill(buffer,BRICKSiZE*x1,BRICKSiZE*y1, BRICKSiZE*x1+BRICKSiZE,BRICKSiZE*y1+BRICKSiZE, makecol( 255, 0, 0));
      }
      if(level[y1][x1]=='o'){
	draw_sprite(buffer,diamond,BRICKSiZE*x1,BRICKSiZE*y1);
	//triangle(buffer,BRICKSiZE*x1,BRICKSiZE*y1,BRICKSiZE*x1+20,BRICKSiZE*y1+BRICKSiZE,BRICKSiZE*x1+BRICKSiZE,BRICKSiZE*y1,makecol(0,100,255));
      }
      if(level[y1][x1]=='E'){
	//rectfill(buffer,BRICKSiZE*x1,BRICKSiZE*y1,BRICKSiZE*x1+BRICKSiZE,BRICKSiZE*y1+BRICKSiZE,makecol(255,255,0));
	draw_sprite(buffer,door,BRICKSiZE*x1,BRICKSiZE*y1);
      }
      if(level[y1][x1]=='F'){
	//rectfill(buffer,BRICKSiZE*x1,BRICKSiZE*y1,BRICKSiZE*x1+BRICKSiZE,BRICKSiZE*y1+BRICKSiZE,makecol(0,255,100));
	draw_sprite(buffer,fire,BRICKSiZE*x1,BRICKSiZE*y1);
      }
    }
  }
  //rectfill(buffer,BRICKSiZE*x+pix,BRICKSiZE*y+piy, BRICKSiZE*x+BRICKSiZE+pix,BRICKSiZE*y+BRICKSiZE+piy, makecol(255,255, 255));
  sprintf(line,"Points: %3d Level: %d Lives: %d",points,clevel,lives);
  textout_hack(buffer,font,0,BRICKSiZE*13+20,2.5,line,makecol(0,100,255));
  blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
}

/*
void brick(int y1, int x1){
//	acquire_screen();
	rectfill( screen, x1, y1, x1+10, y1+10, makecol( 255, 0, 0));
//	release_screen();*/
