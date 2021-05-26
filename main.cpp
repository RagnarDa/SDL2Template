#include <SDL.h>
#include <SDL_mixer.h>


int main(int argc, char *argv[])
{

    // retutns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window* win = SDL_CreateWindow("GAME",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       1000, 1000, 0);


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

    Mix_Music *music = Mix_LoadMUS("../dungeon_castle.mp3");
    if (music == NULL) {
        SDL_Log("Couldn't load music %s", SDL_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        exit(1);
    }
    Mix_FadeInMusic(music,0,2000);



    SDL_Event e;
    bool quit = false;
    while (!quit){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = true;
            }
            if (e.type == SDL_KEYDOWN){
                quit = true;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN){
                quit = true;
            }
        }
    }
    Mix_FreeMusic(music);

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