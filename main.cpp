#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
//#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>
#include <cstdio>
#include <cstdlib>
#include "Game.h"

Game *game = nullptr;

int main(int argc, char *argv[])
{

    // retutns zero on success else non-zero
//    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
//        printf("error initializing SDL: %s\n", SDL_GetError());
//    }
//    //SDL_Window* win = SDL_CreateWindow("GAME",
//                                       SDL_WINDOWPOS_CENTERED,
//                                       SDL_WINDOWPOS_CENTERED,
//                                       1000, 1000, 0);
//
//    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, 0);
//    if (renderer)
//    {
//        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//    }
//
//
    int audio_rate;
    Uint16 audio_format;
    int audio_channels;
    int audio_buffers;
    int audio_volume = MIX_MAX_VOLUME;

    /* Initialize variables */
    audio_rate = MIX_DEFAULT_FREQUENCY;
    audio_format = MIX_DEFAULT_FORMAT;
    audio_channels = MIX_DEFAULT_CHANNELS;
    audio_buffers = 4096;

    /* Open the audio device */
    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) < 0) {
        SDL_Log("Couldn't open audio: %s\n", SDL_GetError());
        return(2);
    } else {
        Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
        SDL_Log("Opened audio at %d Hz %d bit%s %s %d bytes audio buffer\n", audio_rate,
                (audio_format&0xFF),
                (SDL_AUDIO_ISFLOAT(audio_format) ? " (float)" : ""),
                (audio_channels > 2) ? "surround" : (audio_channels > 1) ? "stereo" : "mono",
                audio_buffers);
    }

    /* Set the music volume */
    Mix_VolumeMusic(SDL_MIX_MAXVOLUME);

    /* Set the external music player, if any */
    Mix_SetMusicCMD(SDL_getenv("MUSIC_CMD"));

    Mix_Music *music = Mix_LoadMUS("strauss.mid");
    if (music == NULL) {
        SDL_Log("Couldn't load music %s", SDL_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        exit(1);
    }
    Mix_FadeInMusic(music,10,2000);

    
//
////    /* Init TTF. */
////    TTF_Init();
////    TTF_Font* font = TTF_OpenFont("../JosefinSans-Regular.ttf", 24);
////    if (font == NULL) {
////        fprintf(stderr, "error: font not found\n");
////        exit(EXIT_FAILURE);
////    }
//
//    /* Clear screen */
//    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
//    SDL_RenderClear(renderer);
//
//
////    char buff[100];
////    std::sprintf(buff, "Hello World!", 1);
////    SDL_Color textColor = { 255, 255, 255, 0 };
////    SDL_Surface* surface = TTF_RenderText_Solid(font, buff, textColor);
////    SDL_Texture * fonttexture = SDL_CreateTextureFromSurface(renderer, surface);;
////    SDL_Rect fontrect;
////    fontrect.x = 100;
////    fontrect.y = 100;
////    fontrect.w = 200;
////    fontrect.h = 200;
//
////    SDL_RenderCopy(renderer, fonttexture, NULL, &fontrect);
//
//    /* Display image */
//    IMG_Init(IMG_INIT_PNG) ;
//    SDL_Surface *image = IMG_Load("../goblin.png");
//    if(!image) {
//        printf("IMG_Load: %s\n", IMG_GetError());
//        exit(1);
//    }
//    SDL_Texture * imagetexture = SDL_CreateTextureFromSurface(renderer, image);;
//    SDL_Rect imagerect;
//    imagerect.x = 200;
//    imagerect.y = 200;
//    imagerect.w = 200;
//    imagerect.h = 200;
//    SDL_Rect srcrect;
//    const int goblinwidth = 704/11;
//    const int goblinheight = 320/5;
//    int goblinanimx = 0;
//    const int goblinanimy = 0;
//    srcrect.x = goblinwidth * goblinanimx;
//    srcrect.y = goblinheight * goblinanimy;
//    srcrect.w = goblinwidth;
//    srcrect.h = goblinheight;
//    SDL_RenderCopy(renderer, imagetexture, &srcrect, &imagerect);
//
//    /* GFX demo */
//    Sint16 vx[] = { 100, 300, 200 };
//    Sint16 vy[] = { 100, 100, 300 };
//    aapolygonColor(renderer, vx, vy, 3, 0xFFFF0000);
//    Sint16 vx2[] = { 100, 600, 200 };
//    Sint16 vy2[] = { 100, 600, 300 };
//    polygonColor(renderer, vx2, vy2, 3, 0xFFFF0000);
//
//    /* Draw */
//    SDL_RenderPresent(renderer);

    SDL_Event e;
    bool quit = false;
    while (!quit){
        const int FPS = 60; // seconds
        const int FrameDelay = 1000/FPS; // milliseconds

        Uint32 Framestart;
        int Frametime;


        game = new Game();
        game->init("Space Cleaner - Planet Pool", 1024, 640, false);

        while (game->running())
        {
            Framestart = SDL_GetTicks();
            game->handleEvents();
            game->update(1.0/FPS);
            game->render();

            Frametime = (int)SDL_GetTicks()-(int)Framestart;
            if (FrameDelay > Frametime)
            {
                SDL_Delay(FrameDelay-Frametime);
            }
        }

        game->clean();
        return 0;
    }

    if(Mix_PlayingMusic()) {
        Mix_FadeOutMusic(1500);
        SDL_Delay(1500);
    }
    if (music) {
        Mix_FreeMusic(music);
        music = NULL;
    }
   
    Mix_CloseAudio();
    SDL_Quit();

    return 0;
}