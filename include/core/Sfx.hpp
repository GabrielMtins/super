#ifndef SFX_HPP
#define SFX_HPP

#include "core/Context.hpp"

class Sfx {
	public:
		Sfx(void);

		bool load(const std::string& filename);
		void unload(void);

		void play(int loops, int channel);
		void setVolume(int volume);

	private:
		Mix_Chunk *chunk;
		std::string path;
};

#endif
