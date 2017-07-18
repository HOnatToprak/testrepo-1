#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<allegro5\allegro.h>
#include<allegro5\allegro_font.h>
#include<allegro5\allegro_image.h>
#include<allegro5\allegro_primitives.h>
#include<allegro5\allegro_ttf.h>
#include<allegro5\allegro_audio.h>
#include<allegro5\allegro_acodec.h>
#include<string.h>

#define SCREENWIDTH 1280
#define SCREENHEIGHT 720
const float FPS = 60;
float resizeduck = SCREENWIDTH / 400;
float resizedog = SCREENHEIGHT / 250;
enum BitmapCentering { CENTER = -100000, SAME };

struct BITMAP
{
	ALLEGRO_BITMAP *ptrbitmap;
	int sourceX;
	int sourceY;
	int frameX;
	int frameY;
	int locX;
	int locY;
	int desX;
	int desY;
	int rotateflag;
};

struct duck
{
//variable
float spawnx;
float spawny;
float locx;
float locy;
float boundx;
float boundy;
float speed;
float dirx;
float diry;
bool dirflagx;
bool dirflagy;
bool isalive;
//sprite
ALLEGRO_BITMAP *sprite;
int startx;
int starty;
int framex;
int framey;
int maxFrame;
int curFrame;
int frameCount;
int frameDelay;
//deatheffects  -- to wait effects done to pass level
bool shotflag;
bool shotflag2;
int deathcount;
};

struct dog
{
//variable
float spawnx;
float spawny;
float locx;
float locy;
float speed;
bool isalive;
float boundx;
float boundy;
//sprite
ALLEGRO_BITMAP *sprite;
int startx;
int starty;
int framex;
int framey;
int maxFrame;
int curFrame;
int frameCount;
int frameDelay;
int smellCounter;
int smellDelay;
};



//Prototypes
void randdir(float *dirx, float *diry); //random direction assign
void randspwn(int cnt, duck duck); // random spawn point assign

void initduck(duck duck[], int ducknum, ALLEGRO_BITMAP *spritesheet);
void drawduck(duck duck[], int ducknum, bool *isdogalive);
void updateduck(duck duck[], int ducknum, bool *isdogalive);
void killduck(duck duck[], int ducknum, ALLEGRO_MOUSE_STATE state, int aimx, int aimy, ALLEGRO_SAMPLE *gotduck, ALLEGRO_SAMPLE_ID *PLAYING, bool mute);
void drawdog(dog *dog, int ducknum);
void initdog(dog *doggy, ALLEGRO_BITMAP *spritesheet);
void updatedog(dog *dog, int * timecountdown, int * bulletnum, int * currentlevel);
void drawcrosshair(BITMAP *GetBitmap, int mouseX, int mouseY);
struct BITMAP* newBITMAP(const char FILENAME[], int sourceX, int sourceY, int frameX, int frameY, int locX, int locY, int desX, int desY, int rotateflag);
void drawbitmap(BITMAP* thisbitmap);
bool areducksdead(duck duck[], int ducknum);


int main() {
	srand(time(NULL));

	if (!al_init()) {
		printf("Allegro initialization failed!");
		return -1;
	}
	if (!al_install_keyboard()) {
		printf("Keyboard installation failed!");
		al_rest(5);
		return -1;
	}
	if (!al_install_mouse()) {
		printf("Mouse installation failed!");
		al_rest(5);
		return -1;
	}
	if (!al_install_audio()) {
		printf("Keyboard installation failed!");
		al_rest(5);
		return -1;
	}
	if (!al_init_image_addon()) {
		printf("Image addon initialization failed!");
		al_rest(5);
		return -1;
	}
	if (!al_init_font_addon()) {
		printf("Font addon initialization failed!");
		al_rest(5);
		return -1;
	}
	if (!al_init_primitives_addon()) {
		printf("Primitives addon initialization failed!");
		al_rest(5);
		return -1;
	}
	if (!al_init_ttf_addon()) {
		printf("TTF addon initialization failed!");
		al_rest(5);
		return -1;
	}
	if (!al_init_acodec_addon()) {
		printf("Audio Codec addon initialization failed!");
		al_rest(5);
		return -1;
	}
	enum GameStates { GAMESTATE_MENU, GAMESTATE_SCORELIST, GAMESTATE_PLAY, GAMESTATE_EXIT,GAMEMODE_A,GAMEMODE_B };
	int GAME_STATE = GAMESTATE_MENU;


	ALLEGRO_COLOR WHITE = al_map_rgb(255, 255, 255);
	ALLEGRO_COLOR BLACK = al_map_rgb(0, 0, 0);
	ALLEGRO_COLOR ORANGE = al_map_rgb(255, 165, 0);
	ALLEGRO_COLOR TURQOISE = al_map_rgb(64, 224, 208);
	ALLEGRO_COLOR RED = al_map_rgb(255, 0, 0);
	ALLEGRO_COLOR GREEN = al_map_rgb(0, 255, 0);
	ALLEGRO_COLOR BLUE = al_map_rgb(0, 0, 255);
	ALLEGRO_COLOR PURPLE = al_map_rgb(138, 43, 226);
	ALLEGRO_FONT *arcade = al_load_ttf_font("ARCADECLASSIC.TTF", 40, 0);

	ALLEGRO_DISPLAY *GAMEDISP = al_create_display(SCREENWIDTH, SCREENHEIGHT);

	ALLEGRO_EVENT_QUEUE *EVQUEUE_1 = al_create_event_queue();
	ALLEGRO_EVENT EV;

	ALLEGRO_TIMER *TIMER = al_create_timer(1 / FPS);
	bool redrawflag = true;

	ALLEGRO_MOUSE_STATE MOUSESTATE;
	bool canthold = false;

	ALLEGRO_BITMAP *spritesheet = al_load_bitmap("duckhunt_various_sheet.png");
	BITMAP *BACKGROUND = newBITMAP("stage.png", 0, 0, SAME, SAME, 0, 0, SCREENWIDTH, SCREENHEIGHT, 0);
	BITMAP *MENUBITMAP = newBITMAP("Duck_Hunt_Logo.png", 0, 0, SAME, SAME, 268, 75, SAME, SAME, 0);
	BITMAP *CROSSHAIR = newBITMAP("aim.png", 0, 0, SAME, SAME, 0, 0, SAME, SAME, 0);
	BITMAP *MUTE = newBITMAP("mute_0.png", 0, 0, SAME, SAME,SCREENWIDTH-70,10,SAME,SAME,0);
	BITMAP *UNMUTE = newBITMAP("unmute_0.png", 0, 0, SAME, SAME,SCREENWIDTH-70, 10, SAME, SAME, 0);
	bool mute = false;
	ALLEGRO_SAMPLE *SHOTSOUND = al_load_sample("Duck Hunt SFX (1).wav");
	ALLEGRO_SAMPLE *GAMESTARTED = al_load_sample("Duck Hunt SFX (10).wav");
	bool gamestartedflag = true;
	ALLEGRO_SAMPLE *GOTDUCK = al_load_sample("Duck Hunt SFX (9).wav");
	ALLEGRO_SAMPLE *LOSTSOUND = al_load_sample("Duck Hunt SFX (12).wav");
	ALLEGRO_SAMPLE_ID PLAYING;

	int width_gameA = al_get_text_width(arcade, "GAME   A          ONE DUCK");
	int width_gameB = al_get_text_width(arcade, "GAME   B          ONE DUCK");
	int width_exit = al_get_text_width(arcade, "EXIT");
	int height_arcade = al_get_font_line_height(arcade);

	

	int ducknum = 2;
	duck duck[2];
	initduck(duck, ducknum, spritesheet);

	dog doggy;
	initdog(&doggy, spritesheet);

	int currentmode = GAMEMODE_B;

	int timecountdown = 30;
	int currentlevel = 1;
	int bulletcounter = 6;
	int lostgameframecount = 120;

	int toplvlgameA = 0;
	int toplvlgameB = 0;







	al_reserve_samples(6);
	al_register_event_source(EVQUEUE_1, al_get_display_event_source(GAMEDISP));
	al_register_event_source(EVQUEUE_1, al_get_timer_event_source(TIMER));
	al_register_event_source(EVQUEUE_1, al_get_keyboard_event_source());
	al_hide_mouse_cursor(GAMEDISP);

	al_start_timer(TIMER);
	while (GAME_STATE != GAMESTATE_EXIT)
	{

		al_wait_for_event(EVQUEUE_1, &EV);
		if (EV.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { GAME_STATE = GAMESTATE_EXIT; }
		else if (GAME_STATE == GAMESTATE_MENU)
		{
			if (EV.type == ALLEGRO_EVENT_KEY_DOWN) {
				if (EV.keyboard.keycode == ALLEGRO_KEY_ESCAPE) { GAME_STATE = GAMESTATE_EXIT; }
			}
			else if (EV.type == ALLEGRO_EVENT_TIMER)
			{
				al_get_mouse_state(&MOUSESTATE);
				if(al_mouse_button_down(&MOUSESTATE, 1) && MOUSESTATE.x+CROSSHAIR->desX>269 && MOUSESTATE.x<269+width_gameA&&MOUSESTATE.y+CROSSHAIR->desY > 501&&MOUSESTATE.y<501+height_arcade)
				{
					if(!mute)
						al_play_sample(GAMESTARTED, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, &PLAYING);
					al_set_timer_count(TIMER, 0);
					ducknum = 1;
					GAME_STATE = GAMESTATE_PLAY;
					currentmode = GAMEMODE_A;
				}
				if (al_mouse_button_down(&MOUSESTATE, 1) && MOUSESTATE.x + CROSSHAIR->desX>269 && MOUSESTATE.x<269 + width_gameB&&MOUSESTATE.y + CROSSHAIR->desY > 501+50 && MOUSESTATE.y<501+50 + height_arcade)
				{
					if(!mute)
						al_play_sample(GAMESTARTED, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, &PLAYING);
					al_set_timer_count(TIMER,0);
					ducknum = 2;
					GAME_STATE = GAMESTATE_PLAY;
					currentmode = GAMEMODE_B;
				}
				if (al_mouse_button_down(&MOUSESTATE, 1) && MOUSESTATE.x + CROSSHAIR->desX>269 && MOUSESTATE.x<269 + width_exit&&MOUSESTATE.y + CROSSHAIR->desY > 501 + 100 && MOUSESTATE.y<501 + 100 + height_arcade)
				{
					GAME_STATE = GAMESTATE_EXIT;
				}
				if (!canthold&&al_mouse_button_down(&MOUSESTATE, 1) && MOUSESTATE.x + CROSSHAIR->desX>MUTE->locX && MOUSESTATE.x<MUTE->locX + MUTE->frameX&&MOUSESTATE.y + CROSSHAIR->desY > MUTE->locY && MOUSESTATE.y<MUTE->locY + MUTE->frameY)
				{
					mute = !mute;
					canthold = true;
				}
				if (!al_mouse_button_down(&MOUSESTATE, 1))
					canthold = false;
				redrawflag = true;
			}
			if (al_is_event_queue_empty(EVQUEUE_1) && redrawflag) 
			{
				al_clear_to_color(BLACK);
				al_draw_text(arcade, ORANGE, 269, 501, ALLEGRO_ALIGN_LEFT, "GAME   A          ONE DUCK");
				al_draw_text(arcade, ORANGE, 269, 501 + 50, ALLEGRO_ALIGN_LEFT, "GAME   B          TWO DUCKS");
				al_draw_text(arcade, ORANGE, 269, 501 + 100, ALLEGRO_ALIGN_LEFT, "EXIT");
				al_draw_textf(arcade, TURQOISE, 778, 501, ALLEGRO_ALIGN_LEFT, "TOP  LEVEL     %d", toplvlgameA);
				al_draw_textf(arcade, TURQOISE, 778,501+50, ALLEGRO_ALIGN_LEFT, "TOP  LEVEL     %d", toplvlgameB);
				if (mute == false)
					drawbitmap(UNMUTE);
				else
					drawbitmap(MUTE);
				drawbitmap(MENUBITMAP);
				drawcrosshair(CROSSHAIR, MOUSESTATE.x, MOUSESTATE.y);
				al_flip_display();
				redrawflag = false;
			}
		}
		else if(GAME_STATE == GAMESTATE_PLAY)
		{
			if (EV.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
				bulletcounter = 6; timecountdown = 30; initduck(duck, 2, spritesheet); initdog(&doggy, spritesheet); GAME_STATE = GAMESTATE_MENU; 						
				switch (currentmode) {
				case GAMEMODE_A:if (currentlevel - 1 > toplvlgameA) { toplvlgameA = currentlevel - 1; }break;
				case GAMEMODE_B:if (currentlevel - 1 > toplvlgameB) { toplvlgameB = currentlevel - 1; }break;
				}
				currentlevel = 1;
				if(!mute)
					al_play_sample(LOSTSOUND, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, &PLAYING);
			}
			else
			{
				if (EV.type == ALLEGRO_EVENT_TIMER)
				{
					
					if (al_get_timer_count(TIMER) % 60 == 0 && !areducksdead(duck,ducknum) && bulletcounter >= 0 && timecountdown >= 0) { timecountdown--; }
					al_get_mouse_state(&MOUSESTATE);
						updatedog(&doggy, &timecountdown,&bulletcounter, &currentlevel);
						updateduck(duck, ducknum, &doggy.isalive);
					

					redrawflag = true;
				}
				if (al_mouse_button_down(&MOUSESTATE, 1) && !canthold)
				{
					if (bulletcounter > 0 && timecountdown > 0 && !areducksdead(duck,ducknum)) {
						bulletcounter--;
						killduck(duck, ducknum, MOUSESTATE, CROSSHAIR->desX, CROSSHAIR->desY,GOTDUCK,&PLAYING,mute);
						if(!mute)
							al_play_sample(SHOTSOUND, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, &PLAYING);
					}
					canthold = true;
				}
				if (!al_mouse_button_down(&MOUSESTATE, 1))
				{
					canthold = false;
				}
				if (redrawflag&&al_is_event_queue_empty(EVQUEUE_1))
				{
					redrawflag = false;
					drawbitmap(BACKGROUND);
					drawduck(duck, ducknum, &doggy.isalive);
					if(!mute)
					if ((bulletcounter <= 0 || timecountdown <= 0) && lostgameframecount == 120 && !areducksdead(duck, ducknum)) {
						al_play_sample(LOSTSOUND,1,0,1,ALLEGRO_PLAYMODE_ONCE,&PLAYING);

					}
					if ((bulletcounter <= 0 || timecountdown <= 0) && lostgameframecount > 0&&!areducksdead(duck,ducknum)) {
						lostgameframecount--;
						al_draw_text(arcade, RED, SCREENWIDTH / 2, 200, 0, "YOU LOST");
						}
					else if((bulletcounter <= 0 || timecountdown <= 0) && lostgameframecount <= 0 && !areducksdead(duck, ducknum))
					{
						switch (currentmode) {
						case GAMEMODE_A:if (currentlevel-1 > toplvlgameA) { toplvlgameA = currentlevel-1; }break;
						case GAMEMODE_B:if (currentlevel-1 > toplvlgameB) { toplvlgameB = currentlevel-1; }break;

						}
						currentlevel = 1;
						initduck(duck,2, spritesheet);
						initdog(&doggy, spritesheet);
						lostgameframecount = 120;
						GAME_STATE = GAMESTATE_MENU;
						bulletcounter = 6;
						timecountdown = 30;
						gamestartedflag = true;
					}
					drawdog(&doggy, ducknum);
					al_draw_textf(arcade, PURPLE, 15, 15, ALLEGRO_ALIGN_LEFT, "LEVEL   %d", currentlevel);
					al_draw_textf(arcade, BLUE, SCREENWIDTH - 500, SCREENHEIGHT - 100, ALLEGRO_ALIGN_LEFT, "BULLETS REMAINING     %d", bulletcounter);
					al_draw_textf(arcade, GREEN, (float)(SCREENWIDTH / 2), (float)(15), ALLEGRO_ALIGN_CENTRE, "%d SECONDS LEFT", timecountdown);
					drawcrosshair(CROSSHAIR, MOUSESTATE.x, MOUSESTATE.y);
					al_flip_display();
					al_clear_to_color(BLACK);
				}
			}
		}

	}
	al_destroy_bitmap(spritesheet); al_destroy_bitmap(BACKGROUND->ptrbitmap); al_destroy_bitmap(MENUBITMAP->ptrbitmap);
	al_destroy_timer(TIMER);
	al_destroy_sample(SHOTSOUND);
	al_destroy_event_queue(EVQUEUE_1);
	al_destroy_display(GAMEDISP);


	return 0;
}
struct BITMAP* newBITMAP(const char FILENAME[], int sourceX, int sourceY, int frameX, int frameY, int locX, int locY, int desX, int desY, int rotateflag)
{
	BITMAP *thisbitmap = (BITMAP*)malloc(sizeof(BITMAP));
	thisbitmap->ptrbitmap = al_load_bitmap(FILENAME);
	if (thisbitmap->ptrbitmap == NULL) {
		printf("BITMAP init failed");
	}
	thisbitmap->sourceX = sourceX;
	thisbitmap->sourceY = sourceY;

	if (frameX == SAME) { thisbitmap->frameX = al_get_bitmap_width(thisbitmap->ptrbitmap); }
	else { thisbitmap->frameX = frameX; }
	if (frameY == SAME) { thisbitmap->frameY = al_get_bitmap_height(thisbitmap->ptrbitmap); }
	else { thisbitmap->frameY = frameY; }
	if (locX == CENTER) { thisbitmap->locX = (SCREENWIDTH - thisbitmap->frameX) / 2.0; }
	else { thisbitmap->locX = locX; }
	if (locY == CENTER) { thisbitmap->locY = (SCREENHEIGHT - thisbitmap->frameY) / 2.0; }
	else { thisbitmap->locY = locY; }
	if (desX == SAME) { thisbitmap->desX = thisbitmap->frameX; }
	else { thisbitmap->desX = desX; }
	if (desY == SAME) { thisbitmap->desY = thisbitmap->frameY; }
	else { thisbitmap->desY = desY; }
	thisbitmap->rotateflag = rotateflag;
	printf("\n\n\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n", thisbitmap->sourceX, thisbitmap->sourceY, thisbitmap->frameX, thisbitmap->frameY, thisbitmap->locX, thisbitmap->locY, thisbitmap->desX, thisbitmap->desY);
	return thisbitmap;
}
void drawbitmap(BITMAP *thisbitmap)
{
	al_draw_scaled_bitmap(thisbitmap->ptrbitmap, thisbitmap->sourceX, thisbitmap->sourceY, thisbitmap->frameX, thisbitmap->frameY, thisbitmap->locX, thisbitmap->locY, thisbitmap->desX, thisbitmap->desY, thisbitmap->rotateflag);
}
void drawcrosshair(BITMAP *GetBitmap, int mouseX, int mouseY)
{
	GetBitmap->locX = mouseX; GetBitmap->locY = mouseY;
	al_draw_bitmap(GetBitmap->ptrbitmap, GetBitmap->locX, GetBitmap->locY, GetBitmap->rotateflag);
}
void randdir(float *dirx, float *diry) {
	*dirx = (((rand() % 80) + 10) / (float)100);
	*diry = 1 - (*dirx);
}
void randspwn(int cnt, duck duck[]) {
	int rnd = rand() % 3;
	enum spawnposition
	{
		left, top, right, bottom
	};
	//left
	if (rnd == left) {
		duck[cnt].spawnx = (int)(-duck[cnt].framex * resizeduck);
		duck[cnt].spawny = rand() % (int)(0.7*SCREENHEIGHT);
	}
	//top
	else if (rnd == top) {
		duck[cnt].spawnx = (int)((rand() % (SCREENWIDTH + 2 * (int)(duck[cnt].framex * resizeduck))) - duck[cnt].framex * resizeduck);
		duck[cnt].spawny = int(-(duck[cnt].framey * resizeduck));
	}
	//bottom
	/*else if (rnd == bottom) {
	*x = (rand() % (screenw + 2 * (duck_size))) - duck_size;
	*y = screenh + duck_size;
	}*/
	//right
	else if (rnd == right) {
		duck[cnt].spawnx = (int)(SCREENWIDTH + duck[cnt].framex * resizeduck);
		duck[cnt].spawny = rand() % (int)(0.7*SCREENHEIGHT);
	}
}
void initdog(dog *doggy , ALLEGRO_BITMAP *spritesheet) {
	//sprite
	
		doggy->smellDelay = 5;
		doggy->sprite = spritesheet;
		doggy->framex = 59;
		doggy->framey = 53;
		doggy->startx = 5;
		doggy->starty = 0;
		doggy->maxFrame = 5;
		doggy->frameDelay = 5;
	

	doggy->curFrame = 0;
	doggy->frameCount = 0;
	doggy->smellCounter = 0;

	//variables
	doggy->boundx = doggy->framex;
	doggy->boundy = resizedog * doggy->framey;
	doggy->spawnx = -doggy->boundx;
	doggy->spawny = 11.0 / 12.0 * SCREENHEIGHT - doggy->boundy;
	doggy->isalive = false;
	doggy->locx = doggy->spawnx;
	doggy->locy = doggy->spawny;
	doggy->speed = SCREENWIDTH / 400;


}
void initduck(duck duck[], int ducknum, ALLEGRO_BITMAP *spritesheet) {
	int colorcnt = 0;
	for (int cnt = 0; cnt < ducknum; cnt++, colorcnt++)
	{
		//bitmap
			duck[cnt].sprite = spritesheet;
			duck[cnt].startx = colorcnt * 130;
			duck[cnt].starty = 119;
			duck[cnt].framex = 37;
			duck[cnt].framey = 37;
			duck[cnt].maxFrame = 3;
			duck[cnt].frameDelay = 10;
		
		duck[cnt].curFrame = 0;
		duck[cnt].frameCount = 0;

		if (colorcnt >= 2)
			colorcnt = 0;

		//variables
		randdir(&duck[cnt].dirx, &duck[cnt].diry);
		duck[cnt].speed = 3;
		duck[cnt].dirflagx = rand() % 2;
		duck[cnt].dirflagy = rand() % 2;
		randspwn(cnt, duck);
		duck[cnt].locx = duck[cnt].spawnx;
		duck[cnt].locy = duck[cnt].spawny;
		duck[cnt].isalive = true;
		duck[cnt].boundx = 28 * resizeduck;
		duck[cnt].boundy = 28 * resizeduck;

		//death
		duck[cnt].deathcount = 0;
		duck[cnt].shotflag = false;
		duck[cnt].shotflag2 = false;
	}
}
void drawduck(duck duck[], int ducknum, bool *isdogalive) {
	for (int cnt = 0; cnt < ducknum; cnt++)
	{
		if (duck[cnt].isalive)
		{
			if (++duck[cnt].frameCount >= duck[cnt].frameDelay)
			{
				duck[cnt].frameCount = 0;
				if (++duck[cnt].curFrame >= duck[cnt].maxFrame)
				{
					duck[cnt].curFrame = 0;
				}
			}
			if (duck[cnt].dirx < 0.33 && duck[cnt].dirflagy == 0)
			{
				al_draw_scaled_bitmap(duck[cnt].sprite, duck[cnt].startx + duck[cnt].curFrame*duck[cnt].framex, duck[cnt].starty + 2 * duck[cnt].framey, duck[cnt].framex, duck[cnt].framey, duck[cnt].locx, duck[cnt].locy, resizeduck* duck[cnt].framex, resizeduck * duck[cnt].framey, 2);
			}
			else if (duck[cnt].dirx < 0.33 && duck[cnt].dirflagy == 1)
			{
				al_draw_scaled_bitmap(duck[cnt].sprite, duck[cnt].startx + duck[cnt].curFrame*duck[cnt].framex, duck[cnt].starty + 2 * duck[cnt].framey, duck[cnt].framex, duck[cnt].framey, duck[cnt].locx, duck[cnt].locy, resizeduck* duck[cnt].framex, resizeduck * duck[cnt].framey, 0);
			}
			else if (duck[cnt].dirx > 0.66 && duck[cnt].dirflagx == 0)
			{
				al_draw_scaled_bitmap(duck[cnt].sprite, duck[cnt].startx + duck[cnt].curFrame*duck[cnt].framex, duck[cnt].starty + 0 * duck[cnt].framey, duck[cnt].framex, duck[cnt].framey, duck[cnt].locx, duck[cnt].locy, resizeduck* duck[cnt].framex, resizeduck * duck[cnt].framey, 0);
			}
			else if (duck[cnt].dirx > 0.66 && duck[cnt].dirflagx == 1)
			{
				al_draw_scaled_bitmap(duck[cnt].sprite, duck[cnt].startx + duck[cnt].curFrame*duck[cnt].framex, duck[cnt].starty + 0 * duck[cnt].framey, duck[cnt].framex, duck[cnt].framey, duck[cnt].locx, duck[cnt].locy, resizeduck* duck[cnt].framex, resizeduck * duck[cnt].framey, 1);
			}
			else if (duck[cnt].dirx >= 0.33 && duck[cnt].dirflagx == 0 && duck[cnt].dirflagy == 0)
			{
				al_draw_scaled_bitmap(duck[cnt].sprite, duck[cnt].startx + duck[cnt].curFrame*duck[cnt].framex, duck[cnt].starty + 1 * duck[cnt].framey, duck[cnt].framex, duck[cnt].framey, duck[cnt].locx, duck[cnt].locy, resizeduck* duck[cnt].framex, resizeduck * duck[cnt].framey, 2);
			}
			else if (duck[cnt].dirx >= 0.33 && duck[cnt].dirflagx == 0 && duck[cnt].dirflagy == 1)
			{
				al_draw_scaled_bitmap(duck[cnt].sprite, duck[cnt].startx + duck[cnt].curFrame*duck[cnt].framex, duck[cnt].starty + 1 * duck[cnt].framey, duck[cnt].framex, duck[cnt].framey, duck[cnt].locx, duck[cnt].locy, resizeduck* duck[cnt].framex, resizeduck * duck[cnt].framey, 0);
			}
			else if (duck[cnt].dirx >= 0.33 && duck[cnt].dirflagx == 1 && duck[cnt].dirflagy == 0)
			{
				al_draw_scaled_bitmap(duck[cnt].sprite, duck[cnt].startx + duck[cnt].curFrame*duck[cnt].framex, duck[cnt].starty + 1 * duck[cnt].framey, duck[cnt].framex, duck[cnt].framey, duck[cnt].locx, duck[cnt].locy, resizeduck* duck[cnt].framex, resizeduck * duck[cnt].framey, 3);
			}
			else if (duck[cnt].dirx >= 0.33 && duck[cnt].dirflagx == 1 && duck[cnt].dirflagy == 1)
			{
				al_draw_scaled_bitmap(duck[cnt].sprite, duck[cnt].startx + duck[cnt].curFrame*duck[cnt].framex, duck[cnt].starty + 1 * duck[cnt].framey, duck[cnt].framex, duck[cnt].framey, duck[cnt].locx, duck[cnt].locy, resizeduck* duck[cnt].framex, resizeduck * duck[cnt].framey, 1);
			}

		}
		else
		{
			if (++duck[cnt].deathcount <= 5 * duck[cnt].frameDelay)
			{
				al_draw_scaled_bitmap(duck[cnt].sprite, duck[cnt].startx + 0 * duck[cnt].framex, duck[cnt].starty + 3 * duck[cnt].framey, duck[cnt].framex, duck[cnt].framey, duck[cnt].locx, duck[cnt].locy, resizeduck * duck[cnt].framex, resizeduck * duck[cnt].framey, 0);
			}
			else if (duck[cnt].locy <= 0.7*SCREENHEIGHT - duck->boundy / 2)
			{
				duck[cnt].shotflag = true;
				al_draw_scaled_bitmap(duck[cnt].sprite, duck[cnt].startx + 1 * duck[cnt].framex, duck[cnt].starty + 3 * duck[cnt].framey, duck[cnt].framex, duck[cnt].framey, duck[cnt].locx, duck[cnt].locy, resizeduck * duck[cnt].framex, resizeduck * duck[cnt].framey, 0);
			}
			else
			{
				duck[cnt].shotflag2 = true;
			}
		}
	}

}
void updateduck(duck duck[], int ducknum, bool *isdogalive) {
	bool lvlpassflag = true;
	static bool lvlpassflag2 = true;
	for (int cnt = 0; cnt < ducknum; cnt++)
	{
		if (duck[cnt].isalive || !duck[cnt].shotflag2) {
			lvlpassflag = false;
			break;
		}
	}
	if (lvlpassflag && lvlpassflag2) {
		*isdogalive = true;
		lvlpassflag2 = false;
	}


	if (lvlpassflag && !*isdogalive) {
		for (int cnt = 0; cnt < ducknum; cnt++) {
			randspwn(cnt, duck);
			duck[cnt].locx = duck[cnt].spawnx;
			duck[cnt].locy = duck[cnt].spawny;
			duck[cnt].deathcount = 0;
			duck[cnt].isalive = true;
			duck[cnt].speed *= 1.7;
			duck[cnt].shotflag = false;
			duck[cnt].shotflag2 = false;
		}
		lvlpassflag2 = true;
	}
	for (int cnt = 0; cnt < ducknum; cnt++) {
		if (duck[cnt].isalive) {
			if (duck[cnt].locx <= 0)
				duck[cnt].dirflagx = 0;
			else if (duck[cnt].locx >= SCREENWIDTH - duck[cnt].boundx)
				duck[cnt].dirflagx = 1;
			if (duck[cnt].locy <= 0)
				duck[cnt].dirflagy = 0;
			else if (duck[cnt].locy >= 0.7*SCREENHEIGHT - duck[cnt].boundy)
				duck[cnt].dirflagy = 1;
			if (duck[cnt].dirflagx == 0)
				duck[cnt].locx += duck[cnt].speed*duck[cnt].dirx;
			else
				duck[cnt].locx -= duck[cnt].speed*duck[cnt].dirx;
			if (duck[cnt].dirflagy == 0)
				duck[cnt].locy += duck[cnt].speed*(duck[cnt].diry);
			else
				duck[cnt].locy -= duck[cnt].speed*(duck[cnt].diry);
		}
		else if (duck[cnt].shotflag)
		{
			duck[cnt].locy += 10;
		}
	}

}
void killduck(duck duck[], int ducknum, ALLEGRO_MOUSE_STATE state, int aimx, int aimy, ALLEGRO_SAMPLE *gotduck, ALLEGRO_SAMPLE_ID *PLAYING, bool mute) {
	for (int cnt = 0; cnt < ducknum; cnt++)
	{
		if (state.x + aimx > duck[cnt].locx&&state.x<duck[cnt].locx + duck[cnt].boundx&&state.y + aimy>duck[cnt].locy&&state.y < duck[cnt].locy + duck[cnt].boundy)
		{
			duck[cnt].isalive = false;
			if(!mute)
				al_play_sample(gotduck, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, PLAYING);
		}
		randdir(&duck[cnt].dirx, &duck[cnt].diry);
		duck[cnt].dirflagx = rand() % 2;
		duck[cnt].dirflagy = rand() % 2;
	}
}
void drawdog(dog *dog , int ducknum)
{
	if (dog->isalive)
	{
		if (++dog->frameCount >= dog->frameDelay)
		{
			dog->frameCount = 0;
			if (++dog->curFrame >= dog->maxFrame)
			{
				dog->curFrame = 0;
				if (dog->locx >= (SCREENWIDTH / 2 - resizedog * dog->framex))
					dog->smellCounter++;
			}
		}
		if (dog->smellCounter == 0)
			al_draw_scaled_bitmap(dog->sprite, dog->startx + dog->curFrame * dog->framex, dog->starty + 0 * dog->framey, dog->framex, dog->framey, dog->locx, dog->locy, resizedog*dog->framex, resizedog*dog->framey, 0);
		else if (dog->smellCounter < 3)
		{
			al_draw_scaled_bitmap(dog->sprite, dog->startx, dog->starty + 1 * dog->framey, dog->framex, dog->framey, dog->locx, dog->locy, resizedog*dog->framex, resizedog*dog->framey, 0);
		}
		else if (dog->smellCounter<4)
		{
			al_draw_scaled_bitmap(dog->sprite, dog->startx + 1 * dog->framex, dog->starty + 1 * dog->framey, dog->framex, dog->framey, dog->locx, dog->locy - resizedog*dog->framey*0.5, resizedog*dog->framex, resizedog*dog->framey, 0);
		}
		else  if (dog->smellCounter<5)
		{
			al_draw_scaled_bitmap(dog->sprite, dog->startx + 2 * dog->framex, dog->starty + 1 * dog->framey, dog->framex, dog->framey, dog->locx, dog->locy - resizedog*dog->framey*0.5, resizedog*dog->framex, resizedog*dog->framey, 0);
		}
		else if (dog->smellCounter<10) {}
		else if (dog->smellCounter < 13)
		{
			al_draw_scaled_bitmap(dog->sprite, dog->startx + 3 * dog->framex, dog->starty + 1 * dog->framey, dog->framex, dog->framey, dog->locx + (resizedog*dog->framex) / 2, dog->locy - 0.7*resizedog*dog->framey, resizedog*dog->framex, resizedog*dog->framey, 0);
		}
		else if (dog->smellCounter < 19)
		{
			if (ducknum == 2) {
				al_draw_scaled_bitmap(dog->sprite, dog->startx + 5 * dog->framex, dog->starty + 1 * dog->framey, 1.4*dog->framex, dog->framey, dog->locx + (resizedog*dog->framex) / 4, dog->locy - 0.7*resizedog*dog->framey, 1.4*resizedog*dog->framex, resizedog*dog->framey, 0);
			}
			else{
				al_draw_scaled_bitmap(dog->sprite, dog->startx + 5 * dog->framex, dog->starty + 0 * dog->framey, 1.4*dog->framex, dog->framey, dog->locx + (resizedog*dog->framex) / 4, dog->locy - 0.7*resizedog*dog->framey, 1.4*resizedog*dog->framex, resizedog*dog->framey, 0);
			}
		}



	}
}
void updatedog(dog *dog,int * timecountdown , int * bulletnum , int * currentlevel)
{
	if (dog->isalive)
	{
		if (dog->smellCounter == 0)
		{
			dog->locx += dog->speed;
		}
		else if (dog->smellCounter < 4) {}
		else if (dog->smellCounter < 5) { dog->locy = dog->locy -3; }
		else if (dog->smellCounter < 6) { dog->locy = dog->locy +2; }
		else if (dog->smellCounter < 19)
		{

		}
		else
		{
			(*currentlevel)++;
			*timecountdown = 30;
			*bulletnum = 5;
			dog->isalive = false;
			dog->locx = dog->spawnx;
			dog->locy = dog->spawny;
			dog->smellCounter = 0;
		}
	}
}
bool areducksdead(duck duck[], int ducknum)
{
	bool aredead = true;
	for(int cnt =0;cnt<ducknum;cnt++)
		if(duck[cnt].isalive)
		{
			aredead = false;
			break;
		}
	return aredead;
}
