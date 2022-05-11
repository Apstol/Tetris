#include <iostream>
#include <fstream>

#include "GameOverState.hpp"
#include "Utility.hpp"
#include "Game.hpp"

const sf::Time GameOverState::TimePerHighscoreBlink = sf::seconds(0.5f);

GameOverState::GameOverState(StateStack& stack, Context context)
: State(stack, context)
, mGameOverAnimation(context.spritesheet)
, mSelectedOption(GameOverOptions::PlayAgain)
, mPlayedHighscoreSound(false)
{
	mScoreText.setFont(context.font);
	mScoreText.setCharacterSize(26);
	mScoreText.setString("Score " + std::to_string(context.game.score()));
	mScoreText.setFillColor(sf::Color::Yellow);
	Utility::centerOrigin(mScoreText);
	mScoreText.setPosition(400, 100);

	mHighscoreText.setFont(context.font);
	mHighscoreText.setCharacterSize(26);
	mHighscoreText.setString("Highscore " + std::to_string(context.game.highscore()));
	mHighscoreText.setFillColor(sf::Color::Yellow);
	Utility::centerOrigin(mHighscoreText);
	mHighscoreText.setPosition(400, 160);

	sf::Text playAgainOption;
	playAgainOption.setFont(context.font);
	playAgainOption.setCharacterSize(40);
	playAgainOption.setString("Play Again");
	playAgainOption.setFillColor(sf::Color::White);
	Utility::centerOrigin(playAgainOption);
	playAgainOption.setPosition(400, 260);

	sf::Text exitOption;
	exitOption.setFont(context.font);
	exitOption.setCharacterSize(40);
	exitOption.setString("Exit");
	exitOption.setFillColor(sf::Color(211, 211, 211, 133));
	Utility::centerOrigin(exitOption);
	exitOption.setPosition(400, 320);

	mOptions = { playAgainOption, exitOption };

	mOverlay.setSize(sf::Vector2f(800.f, 600.f));
	mOverlay.setFillColor(sf::Color(0, 0, 0, 220));

	if (isNewHighscore())
	{
		context.game.saveHighscore();
		mHighscoreText.setString("New highscore! " + std::to_string(context.game.score()));
		Utility::centerOrigin(mHighscoreText);

		mScoreText.setFillColor(sf::Color(0, 0, 0, 0));
	}

	context.soundPlayer.play(SoundPlayer::SoundEffect::Overflow, true);
}


bool GameOverState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed
	    && mGameOverAnimation.isFinished())
	{
		if (event.key.code == sf::Keyboard::Up)
		{
			mSelectedOption = (mSelectedOption - 1 + mOptions.size()) % mOptions.size();
		}
		else if (event.key.code == sf::Keyboard::Down)
		{
			mSelectedOption = (mSelectedOption + 1) % mOptions.size();
		}
		else if (event.key.code == sf::Keyboard::Enter)
		{
			switch (static_cast<GameOverOptions>(mSelectedOption))
			{
				case GameOverOptions::PlayAgain:
					requestStackPop();
					requestStackPop();
					requestStackPush(States::Game);
					break;
				case GameOverOptions::Exit:
					requestStackPop();
					requestStackPop();
					break;
			}
		}
	}

	return false;
}

bool GameOverState::handleRealtimeInput(sf::Time elapsedTime)
{
	return false;
}

bool GameOverState::update(sf::Time elapsedTime)
{
	if (mGameOverAnimation.isFinished())
	{
		context().soundPlayer.stopLooped();
		
		for (sf::Text& option : mOptions)
		{
			option.setColor(sf::Color(211, 211, 211, 133));
		}

		mOptions[mSelectedOption].setColor(sf::Color::White);

		if (isNewHighscore())
		{
			if (!mPlayedHighscoreSound)
			{
				context().soundPlayer.play(SoundPlayer::SoundEffect::Highscore);
				mPlayedHighscoreSound = true;
			}

			mHighscoreBlinkingTimeElapsed += elapsedTime;
			while (mHighscoreBlinkingTimeElapsed >= TimePerHighscoreBlink)
			{
				sf::Color color = mHighscoreText.getFillColor();
				color.a = color.a == 255 ? 0 : 255;
				mHighscoreText.setFillColor(color);
				mHighscoreBlinkingTimeElapsed -= TimePerHighscoreBlink;
			}
		}
	}
	else
	{
		mGameOverAnimation.update(elapsedTime);
	}

	return false;
}

void GameOverState::draw()
{
	context().window.draw(mGameOverAnimation);

	if (mGameOverAnimation.isFinished())
	{
		context().window.draw(mOverlay);
		context().window.draw(mScoreText);
		context().window.draw(mHighscoreText);
		for (sf::Text& option : mOptions)
		{
			context().window.draw(option);
		}
	}
}

bool GameOverState::isNewHighscore()
{
	return context().game.score() > context().game.highscore();
}

