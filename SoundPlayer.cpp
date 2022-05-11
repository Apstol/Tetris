#include "SoundPlayer.hpp";

#include <cassert>
#include <iostream>

SoundPlayer::SoundPlayer()
{
	loadResource(SoundPlayer::SoundEffect::Drop, "./assets/audio/drop.wav");
	loadResource(SoundPlayer::SoundEffect::Line_cleared, "./assets/audio/line-cleared.wav");
	loadResource(SoundPlayer::SoundEffect::Levelup, "./assets/audio/levelup.wav");
	loadResource(SoundPlayer::SoundEffect::Rotation, "./assets/audio/rotation.wav");
	loadResource(SoundPlayer::SoundEffect::Overflow, "./assets/audio/overflow.wav");
	loadResource(SoundPlayer::SoundEffect::Highscore, "./assets/audio/highscore.wav");
	loadResource(SoundPlayer::SoundEffect::Tetris, "./assets/audio/tetris.wav");
	loadResource(SoundPlayer::SoundEffect::Pause, "./assets/audio/pause.wav");
}

void SoundPlayer::play(SoundPlayer::SoundEffect effect, bool loop)
{
	auto foundBuffer = mSoundBuffers.find(effect);
	assert(foundBuffer != mSoundBuffers.end());

	mSounds.push_back(sf::Sound(*(foundBuffer->second)));

	sf::Sound& sound = mSounds.back();
	sound.setLoop(loop);
	sound.play();
}

void SoundPlayer::stopLooped()
{
	for (sf::Sound& sound : mSounds)
	{
		if (sound.getLoop())
		{
			sound.stop();
		}
	}
}

void SoundPlayer::removeStoppedSounds()
{
	mSounds.remove_if([] (const sf::Sound& sound) 
	{ 
		return sound.getStatus() == sf::Sound::Stopped;
	});
}

void SoundPlayer::loadResource(SoundPlayer::SoundEffect id, const std::string& filename)
{
	std::unique_ptr<sf::SoundBuffer> resource = std::make_unique<sf::SoundBuffer>();

	if (!resource->loadFromFile(filename))
	{
		throw std::runtime_error("Failed to load sound: " + filename);
	}

	mSoundBuffers.emplace(
		std::make_pair(id, std::move(resource))
	);
}
