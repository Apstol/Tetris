#pragma once

#include <SFML/Graphics.hpp>

#include "StateStack.hpp"

class Game
{
	public:
		static const int ScreenWidth;
		static const int ScreenHeight;

		Game();
		~Game() = default;		

		void run();
		void handleInput(sf::Time elapsedTime);
		void update(sf::Time elapsedTime);
		void render();
		int score() const;
		void setScore(int score);
		int highscore() const;
		void readHighscore();
		void saveHighscore();

	private:
		void registerStates();

		static const sf::Time TimePerFrame;
		sf::RenderWindow mWindow;
		sf::Font mFont;
		sf::Texture mSpritesheet;
		SoundPlayer mSoundPlayer;
		MusicPlayer mMusicPlayer;
		StateStack mStateStack;
		int mScore;
		int mHighscore;
};
