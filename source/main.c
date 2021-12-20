
#include <stdio.h>
#include <tonc.h>
#include "head.h"
#include "body.h"
#include "smile.h"

OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer= (OBJ_AFFINE*)obj_buffer;

u32 tid = 0, pb = 0;

	
OBJ_ATTR *snakeHead = &obj_buffer[0];

OBJ_ATTR *smile = &obj_buffer[127];

OBJ_ATTR *snakeBody[126];




bool gameOver;

//gba screen resoluiton 240x160
int width = 240;
int height = 160;

//tail
int tailX[126], tailY[126];
int nTail;

//head position, food position, score
int x, y, fruitX, fruitY, score;

//enumeration for snake direction
enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN};

enum eDirection dir;

void Setup()
{
	
	
	
	//initial parameters
	gameOver = false;
	dir = STOP;
	x = width / 2;
	y = height / 2;
	nTail = 0;
	
	//randomize food location
	/*
	fruitX = rand() % width;
	fruitY = rand() % height;
	*/
	fruitX = qran_range(0, 232);
	fruitY = qran_range(0, 152);
	
	
	score = 0;
	
	obj_set_attr(smile,
		ATTR0_SQUARE,
		ATTR1_SIZE_8,
		ATTR2_PALBANK(pb) | 127);
	
	obj_set_pos(smile,fruitX,fruitY);
	
	obj_set_attr(snakeHead,
		ATTR0_SQUARE,
		ATTR1_SIZE_8,
		ATTR2_PALBANK(pb) | tid);
	
	obj_set_pos(snakeHead, x, y);
	
	for(int i = 1; i < 127; i++){
		snakeBody[i] = &obj_buffer[i];
	}
	
	for(int i = 1; i < 127; i++)
	{
		obj_set_attr(snakeBody[i],
			ATTR0_SQUARE,
			ATTR1_SIZE_8,
			ATTR2_PALBANK(pb) | i);
	}
	
	obj_set_pos(snakeBody[0], x, y);

}

void Draw()
{
	vid_vsync();
	
	snakeHead->attr2= ATTR2_BUILD(tid, pb, 0);
	
	smile->attr2= ATTR2_BUILD(127, pb, 0);
	
	//snakeBody->attr2= ATTR2_BUILD(tid, pb, 0);
	
	obj_set_pos(snakeHead, x, y);
	
	obj_set_pos(smile, fruitX, fruitY);
	
	for(int i = 0; i < 127; i++)
		obj_hide(snakeBody[i]);

	
	
	
	//go through every element of tail and draw it
	for(int k = 1; k < nTail; k++)
	{
		//if current tailX[k] element is equal to x cooridinate AND taily[y] is equal to current y cooridinate
		//draw tail
		obj_set_pos(snakeBody[k],tailX[k],tailY[k]);
		obj_unhide(snakeBody[k], 0);
		
	}
	
	oam_copy(oam_mem, obj_buffer, (128));	

}

void Input()
{
	key_poll();
	if(key_hit(KEY_LEFT) && (dir != RIGHT))
		dir = LEFT;
	if(key_hit(KEY_RIGHT) && (dir != LEFT))
		dir = RIGHT;
	if(key_hit(KEY_UP) && (dir != DOWN))
		dir = UP;
	if(key_hit(KEY_DOWN) && (dir != UP))
		dir = DOWN;
		
	else if (key_hit(KEY_START))
		dir = STOP;

}

void Logic()
{

	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;
	for (int i = 1; i < nTail; i++)
	{
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	
	}
	
	
	switch (dir)
	{
		case LEFT:
			x -= 4;		
			break;
			
		case RIGHT:
			x += 4;		
			break;
			
		case UP:
			y -= 4;		
			break;
			
		case DOWN:
			y += 4;		
			break;
		
		case STOP:
			x = x;
			y = y;
		default:
			break;


	}
	
	//if hit wall end game (x > width || x < 0) (y > height || y < 0)
	
	if(x > 228)
		x = 16;
		
	if(x < 12)
		x = 224;
		
	if(y < 0)
		y = height;
		
	if(y > height)
		y = 0;
		
	
	
	//hit self
	for(int i = 0; i < nTail; i++)
	{
		if (tailX[i] == x && tailY[i] == y)
			gameOver = true;
	}
	
	//if eat food increase tail nTail +=1
	if(key_hit(KEY_A) || (x >= fruitX-8 && x<= fruitX+12 && y>=fruitY-8 && y <= fruitY+12))
	{
		nTail += 1;
		fruitX = qran_range(0, 232);
		fruitY = qran_range(0, 152);
		
		score++;
	}
	
	//if eat food increase score (x == fruitX && y == fruitY)
	
	
	
	
	

}

int main()
{
	
	
	//place the tiles a a 4bpp boxed snake head sprite
	//into LOW obj memory (cbb == 4)
	memcpy(&tile_mem[4][0], headTiles, headTilesLen);
	memcpy(pal_obj_mem, headPal, headPalLen);
	
	memcpy(&tile_mem[4][127], smileTiles, smileTilesLen);
	memcpy(pal_obj_mem, smilePal, smilePalLen);
	
	for(int i = 0; i < 126; i++)
	{
		memcpy(&tile_mem[4][i+1], bodyTiles, bodyTilesLen);
		memcpy(pal_obj_mem, bodyPal, bodyPalLen);
	}
	//initialize sprites
	oam_init(obj_buffer, 128);
	
	/*
	// Init interrupts and VBlank irq.
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);
	*/
	
	// Video mode , enable bg .
	REG_DISPCNT= DCNT_OBJ | DCNT_OBJ_1D | DCNT_BG0;
	
	
	
	
	
	
	
	
	Setup();
	
	
	while(1)
	{
		//VBlankIntrWait();
		if(!gameOver){
		
			Draw();
			Input();
			Logic();
		
		}
		else if(gameOver == true){
			OAM_CLEAR();
			
			oam_init(obj_buffer, 128);
			
			Setup();
			
			
			
 
				
		}
	}
	
	
	return 0;
}
