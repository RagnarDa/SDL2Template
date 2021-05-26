# Fixed CMakeLists.txt for mp3 support in SDL_mixer
project(SDL_mixer C)
include_directories(include src src/codecs)

add_library(SDL2_mixer SHARED)

target_sources(SDL2_mixer PRIVATE
        src/effect_position.c src/effects_internal.c src/effect_stereoreverse.c
        src/mixer.c src/music.c src/utils.c
        src/codecs/load_aiff.c src/codecs/load_voc.c
        src/codecs/music_cmd.c src/codecs/music_flac.c
        src/codecs/music_fluidsynth.c src/codecs/music_mad.c
        src/codecs/music_mikmod.c src/codecs/music_modplug.c
        src/codecs/music_mpg123.c src/codecs/music_nativemidi.c
        src/codecs/mp3utils.c
        src/codecs/music_ogg.c src/codecs/music_opus.c
        src/codecs/music_timidity.c src/codecs/music_wav.c)