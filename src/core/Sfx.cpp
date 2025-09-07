#include "core/Sfx.hpp"
#include "core/Console.hpp"

Sfx::Sfx(void) {
}

bool Sfx::load(const std::string& filename) {
	chunk = Mix_LoadWAV(filename.c_str());

	if(chunk == NULL) {
		console.error("[Sfx] Failed to open file: " + filename);
		return false;
	}

	console.log("[Sfx] Loaded sfx: " + filename);

	path = filename;

	return true;
}

void Sfx::unload(void) {
	if(chunk == NULL)
		return;

	console.log("[Sfx] Unloading sfx: " + path);
	Mix_FreeChunk(chunk);
	path = "";
}

void Sfx::play(int loops, int channel) {
	if(chunk == NULL)
		return;

	Mix_PlayChannel(channel, chunk, loops);
}

void Sfx::setVolume(int volume) {
	if(chunk == NULL)
		return;

	Mix_VolumeChunk(chunk, volume);
}
