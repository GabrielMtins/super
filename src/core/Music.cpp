#include "core/Music.hpp"
#include "core/Console.hpp"

Music::Music(void) {
	path = "";
	music = NULL;
}

bool Music::load(const std::string& filename) {
	music = Mix_LoadMUS(filename.c_str());
	
	if(music == NULL) {
		console.error("[Music] Failed to load file: " + filename);
		return false;
	}

	console.log("[Music] Loaded music: " + filename);
	path = filename;

	return true;
}

void Music::unload(void) {
	if(music == NULL)
		return;

	Mix_FreeMusic(music);
	
	console.log("[Music] Unloading music: " + path);
	path = "";
}

void Music::play(int loops) {
	if(music == NULL)
		return;

	Mix_PlayMusic(music, loops);
}

void Music::setVolume(int volume) {
	Mix_VolumeMusic(volume);
}
void Music::fadeOutMusic(int ms) {
	Mix_FadeOutMusic(ms);
}

void Music::haltMusic(void) {
	Mix_HaltMusic();
}
