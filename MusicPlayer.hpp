#pragma once

#include <SFML/Audio.hpp>

#include <map>
#include <string>

class MusicPlayer : public sf::NonCopyable
{
	public:
		enum MusicTheme
		{
			Menu,
			Main
		};

		MusicPlayer();

		void play(MusicPlayer::MusicTheme theme);
		void stop();
		void setVolume(float volume);
		void setPaused(bool isPaused);

	private:
		sf::Music mMusic;
		std::map<MusicPlayer::MusicTheme, std::string> mFilenames;
};
