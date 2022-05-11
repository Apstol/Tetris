#include "MusicPlayer.hpp";

MusicPlayer::MusicPlayer()
: mMusic()
, mFilenames()
{
	mFilenames[MusicPlayer::MusicTheme::Menu] = "./assets/audio/menu.ogg";
	mFilenames[MusicPlayer::MusicTheme::Main] = "./assets/audio/main-theme.ogg";
}

void MusicPlayer::play(MusicPlayer::MusicTheme theme)
{
	const std::string& filename = mFilenames[theme];

	if (!mMusic.openFromFile(filename))
	{
		throw std::runtime_error("ERROR: cannot load music " + filename);
	}

	mMusic.setVolume(100.f);
	mMusic.setLoop(true);
	mMusic.play();
}

void MusicPlayer::stop()
{
	mMusic.stop();
}

void MusicPlayer::setVolume(float volume)
{
	mMusic.setVolume(volume);
}

void MusicPlayer::setPaused(bool isPaused)
{
	if (isPaused)
	{
		mMusic.pause();
	}
	else
	{
		mMusic.play();
	}
}

