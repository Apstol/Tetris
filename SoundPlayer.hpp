#pragma once

#include <memory>
#include <map>
#include <list>

#include <SFML/Audio.hpp>

class SoundPlayer : public sf::NonCopyable
{
	public:
		enum SoundEffect
		{
			Drop,
			Levelup,
			Rotation,
			Line_cleared,
			Overflow,
			Highscore,
			Tetris,
			Pause
		};

		SoundPlayer();

		void play(SoundPlayer::SoundEffect effect, bool loop = false);
		void stopLooped();

		void removeStoppedSounds();

	private:
		void loadResource(SoundPlayer::SoundEffect id, const std::string& filename);

		std::map<SoundPlayer::SoundEffect, std::unique_ptr<sf::SoundBuffer>> mSoundBuffers;
		std::list<sf::Sound> mSounds;
};
