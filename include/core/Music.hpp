#ifndef MUSIC_HPP
#define MUSIC_HPP

#include "core/Context.hpp"

class Music {
	public:
		Music(void);

		bool load(const std::string& filename);
		void unload(void);

		void play(int loops);

		static void setVolume(int volume);
		static void fadeOutMusic(int ms);
		static void haltMusic(void);

	private:
		Mix_Music *music;
		std::string path;


};

#endif
