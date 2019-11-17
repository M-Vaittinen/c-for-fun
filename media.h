#ifndef _MEDIA_H_
#define _MEDIA_H_

struct vari {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char alpha;
};

struct sounds {
	//The music that will be played
	Mix_Music *music;

	//The sound effects that will be used
	Mix_Chunk *new_ship;
	Mix_Chunk *crash;
	Mix_Chunk *points;
};

struct piirrin {
	TTF_Font* font;
	SDL_Renderer* renderer;
};



#endif
