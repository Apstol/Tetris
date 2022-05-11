#include <stdexcept>
#include <iostream>
#include <fstream>

#include "Game.hpp"
#include "MenuState.hpp"
#include "GameState.hpp"
#include "GameOverState.hpp"
#include "PauseState.hpp"
#include "StateIdentifiers.hpp"
#include "State.hpp"

const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);
const int Game::ScreenWidth = 800;
const int Game::ScreenHeight = 600;

Game::Game()
: mWindow(sf::VideoMode(ScreenWidth, ScreenHeight), "Tetris")
, mScore(0)
, mHighscore(0)
, mSoundPlayer()
, mMusicPlayer()
, mStateStack(State::Context(mWindow, mFont, mSpritesheet, mSoundPlayer, mMusicPlayer, *this))
{
	if (!mFont.loadFromFile("./assets/PixelNES.otf"))
	{
		throw std::runtime_error("ERROR: Unable to load font!");
	}

	if (!mSpritesheet.loadFromFile("./assets/spritesheet.png"))
	{
		throw std::runtime_error("ERROR: Unable to load spritesheet!");
	}
	
	registerStates();
	mStateStack.pushState(States::Menu);
}

void Game::run()
{
	sf::Clock elapsedTime;
	sf::Time timeSinceLastUpdate;

	while (mWindow.isOpen())
	{
		timeSinceLastUpdate += elapsedTime.restart();
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			handleInput(TimePerFrame);
			update(TimePerFrame);

			if (mStateStack.isEmpty())
			{
				mWindow.close();
			}
		}
		render();
	}
}

void Game::handleInput(sf::Time elapsedTime)
{
	sf::Event event;

	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event);

		if (event.type == sf::Event::Closed)
		{
			mWindow.close();
		}
	}
}

void Game::update(sf::Time elapsedTime)
{
	mStateStack.update(elapsedTime);
	mSoundPlayer.removeStoppedSounds();
}

void Game::render()
{
	mWindow.clear(sf::Color::Black);
	mStateStack.draw();
	mWindow.display();
}

int Game::score() const
{
	return mScore;
}

void Game::setScore(int score)
{
	mScore = score;
}

int Game::highscore() const
{
	return mHighscore;
}

void Game::readHighscore()
{
	std::ifstream readStream;
	readStream.open("./assets/highscore.txt");

	if (!readStream.is_open())
	{
		throw std::runtime_error("ERROR: Unable to read highscore!");
	}

	int highscore;
	while (!readStream.eof())
	{
		readStream >> highscore;
	}

	readStream.close();

	mHighscore = highscore;
}

void Game::saveHighscore()
{
	std::ofstream writeStream("./assets/highscore.txt");

	if (!writeStream.is_open())
	{
		throw std::runtime_error("ERROR: Unable to write highscore!");
	}

	writeStream << mScore;
}

void Game::registerStates()
{
	mStateStack.registerState<MenuState>(States::ID::Menu);
	mStateStack.registerState<GameState>(States::ID::Game);
	mStateStack.registerState<GameOverState>(States::ID::GameOver);
	mStateStack.registerState<PauseState>(States::ID::Pause);
}
