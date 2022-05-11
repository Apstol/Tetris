#include "GameState.hpp"
#include "Random.hpp"
#include "Game.hpp"

#include <iostream>
#include <algorithm>

GameState::GameState(StateStack& stack, Context context)
: State(stack, context)
, mLevel(0)
, mCurrentLinesCount(0)
, mTotalLinesCount(0)
, mPlayfield(context.spritesheet, *this, context.soundPlayer)
, mClearLinesAnimation(mPlayfield)
{
	context.game.setScore(0);
	context.game.readHighscore();

	mScoreText.setString("score\n" + std::to_string(context.game.score()));
	mScoreText.setFont(context.font);
	mScoreText.setCharacterSize(30);
	mScoreText.setFillColor(sf::Color::White);
	mScoreText.setPosition(sf::Vector2f(400.f, 300.f) + sf::Vector2f(5.f, 0.f) * 2.f + sf::Vector2f((10.f * 20.f) / 2, -(21.f * 20.f) / 2));
	sf::FloatRect scoreTextBounds = mScoreText.getGlobalBounds();

	mLevelText.setString("level\n" + std::to_string(mLevel));
	mLevelText.setFont(context.font);
	mLevelText.setCharacterSize(30);
	mLevelText.setFillColor(sf::Color::White);
	mLevelText.setPosition(scoreTextBounds.left, scoreTextBounds.top + scoreTextBounds.height);
	sf::FloatRect levelTextBounds = mLevelText.getGlobalBounds();

	mLinesText.setString("lines\n" + std::to_string(mTotalLinesCount));
	mLinesText.setFont(context.font);
	mLinesText.setCharacterSize(30);
	mLinesText.setFillColor(sf::Color::White);
	mLinesText.setPosition(levelTextBounds.left, levelTextBounds.top + levelTextBounds.height);
	sf::FloatRect linesTextBounds = mLinesText.getGlobalBounds();

	mNextPieceText.setString("next");
	mNextPieceText.setFont(context.font);
	mNextPieceText.setCharacterSize(30);
	mNextPieceText.setFillColor(sf::Color::White);
	mNextPieceText.setPosition(linesTextBounds.left, linesTextBounds.top + linesTextBounds.height);
	
	setTimePerUpdate();
	setNextTetrominoType();
	spawnTetromino();
	
	context.musicPlayer.play(MusicPlayer::MusicTheme::Main);
}

bool GameState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::S)
		{
			mActiveTetromino->rotate();
		}
		else if (event.key.code == sf::Keyboard::Left)
		{
			mActiveTetromino->move(sf::Vector2f(-1.f, 0.f));
		}
		else if (event.key.code == sf::Keyboard::Right)
		{
			mActiveTetromino->move(sf::Vector2f(1.f, 0.f));
		}
		else if (event.key.code == sf::Keyboard::Space)
		{
			mActiveTetromino->hardDrop();
		}
		else if (event.key.code == sf::Keyboard::P)
		{
			requestStackPush(States::Pause);
		}
	}

	return true;
}

bool GameState::handleRealtimeInput(sf::Time elapsedTime)
{
	return true;
}

bool GameState::update(sf::Time elapsedTime)
{
	mTimeSinceLastUpdate += elapsedTime;

	if (mPlayfield.hasLinesToClear())
	{
		if (!mClearLinesAnimation.isFinished())
		{
			mClearLinesAnimation.update(elapsedTime);
			return true;
		}
		else
		{
			mClearLinesAnimation.restart();
			mPlayfield.clearLines();
			spawnTetromino();
		}
	}

	while (mTimeSinceLastUpdate > mTimePerUpdate)
	{
		mTimeSinceLastUpdate -= mTimePerUpdate;

		mActiveTetromino->move(sf::Vector2f(0.f, 1.f));
		mActiveTetromino->update(mTimePerUpdate);

		if (mActiveTetromino->isLocked())
		{
			if (mPlayfield.update(*mActiveTetromino, elapsedTime))
			{
				return true;
			}
			spawnTetromino();
		}
	}

	if (mPlayfield.isGameOver(*mActiveTetromino))
	{
		context().musicPlayer.stop();
		requestStackPush(States::GameOver);
	}

	return true;
}

void GameState::draw()
{
	context().window.draw(*mActiveTetromino);
	context().window.draw(mPlayfield);

	mScoreText.setString("score\n" + std::to_string(context().game.score()));
	context().window.draw(mScoreText);

	mLevelText.setString("level\n" + std::to_string(mLevel));
	context().window.draw(mLevelText);

	mLinesText.setString("lines\n" + std::to_string(mTotalLinesCount));
	context().window.draw(mLinesText);

	context().window.draw(mNextPieceText);

	context().window.draw(*mNextTetrominoPreview);

	if (mPlayfield.hasLinesToClear())
	{
		context().window.draw(mClearLinesAnimation);
	}
}

void GameState::updateScore(int clearedLinesCount)
{
	int scoreModifier;

	switch (clearedLinesCount)
	{
		case 1:
			scoreModifier = 40;
			break;
		case 2:
			scoreModifier = 100;
			break;
		case 3:
			scoreModifier = 300;
			break;
		case 4:
			scoreModifier = 1200;
			break;
		default:
			scoreModifier = 40;
			break;
	}

	context().game.setScore(context().game.score() + scoreModifier * (mLevel + 1)); 

	incrementLevel(clearedLinesCount);
}

void GameState::setNextTetrominoType()
{
	mNextPieceType = static_cast<TetrominoTypes::Type>(Random::get(TetrominoTypes::I, TetrominoTypes::Count - 1));
}

void GameState::spawnTetromino()
{
	mActiveTetromino.reset(new Tetromino(mNextPieceType, context().spritesheet, mPlayfield, context().soundPlayer));
	setNextTetrominoType();
	createNextTetrominoPreview();
}

void GameState::createNextTetrominoPreview()
{
	mNextTetrominoPreview.reset(new TetrominoPreview(mNextPieceType,  context().spritesheet, mPlayfield, context().soundPlayer));
}

void GameState::setTimePerUpdate()
{
	if (mLevel == 0)
	{
		mTimePerUpdate = sf::seconds(1.f);
	}
	else if (mLevel >= 1 && mLevel <= 8) 
	{
		mTimePerUpdate = sf::seconds(1.f - 1.f * (0.1 * mLevel));
	}
	else if (mLevel == 9) 
	{
		mTimePerUpdate = sf::seconds(1.f - 1.f * (0.1 * 8) - 0.01);
	}
	else if (mLevel >= 10 && mLevel <= 12) 
	{
		mTimePerUpdate = sf::seconds(1.f - 1.f * (0.1 * 8) - 0.04);
	}
	else if (mLevel >= 13 && mLevel <= 15) 
	{
		mTimePerUpdate = sf::seconds(1.f - 1.f * (0.1 * 8) - 0.07);
	}
	else if (mLevel >= 16 && mLevel <= 18) 
	{
		mTimePerUpdate = sf::seconds(1.f - 1.f * (0.1 * 8) - 0.10);
	}
	else if (mLevel >= 19 && mLevel <= 28) 
	{
		mTimePerUpdate = sf::seconds(1.f - 1.f * (0.1 * 8) - 0.13);
	}
	else if (mLevel >= 29) 
	{
		mTimePerUpdate = sf::seconds(1.f - 1.f * (0.1 * 8) - 0.16);
	}
}

void GameState::incrementLevel(int clearedLinesCount)
{
	mTotalLinesCount += clearedLinesCount;
	mCurrentLinesCount += clearedLinesCount;
	if (mCurrentLinesCount >= linesBeforeLevelUp())
	{
		mCurrentLinesCount = 0;
		++mLevel;
		setTimePerUpdate();
		context().soundPlayer.play(SoundPlayer::SoundEffect::Levelup);
	}
}

int GameState::linesBeforeLevelUp()
{
	if (mLevel >= 0 && mLevel <= 8)
	{
		return mLevel * 10 + 10;
	}
	else
	{
		return std::max(100, mLevel * 10 - 50);
	}
}
