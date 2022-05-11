#include <cassert>

#include "Playfield.hpp"
#include "Tetromino.hpp"
#include "Utility.hpp"
#include "GameState.hpp"

Playfield::Playfield(sf::Texture& texture, GameState& game, SoundPlayer& soundPlayer)
: mBorder(sf::Vector2f(Tetromino::Size.x * Columns, Tetromino::Size.x * Rows))
, mGame(game)
, mSoundPlayer(soundPlayer)
, mSpritesheet(texture)
{
	mBorder.setOutlineColor(sf::Color(131, 211, 211, 255));
	mBorder.setOutlineThickness(5);
	mBorder.setFillColor(sf::Color::Transparent);
	Utility::centerOrigin(mBorder);
	mBorder.setPosition(sf::Vector2f(5.f, 5.f) + sf::Vector2f(Game::ScreenWidth / 2, Game::ScreenHeight / 2));

	mPlayfieldData.fill(TetrominoTypes::Type::None);
}

bool Playfield::update(Tetromino& tetromino, sf::Time elapsedTime)
{
	mergeTetromino(tetromino);
	return addLinesToClear(elapsedTime);
}

void Playfield::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mBorder, states);

	sf::Sprite sprite;
	sprite.setTexture(mSpritesheet);
	sprite.setScale(Tetromino::Scale);
	sf::Vector2f positionOffset = sf::Vector2f(Game::ScreenWidth / 2, Game::ScreenHeight / 2) - sf::Vector2f(Columns / 2 * Tetromino::Size.x, Rows / 2 * Tetromino::Size.x);

	for (int row = 0; row < Rows; ++row)
	{
		for (int column = 0; column < Columns; ++column)
		{
			TetrominoTypes::Type type = cellTypeByRowAndColumn(row, column);
			if (type == TetrominoTypes::None)
			{
				continue;
			}
			sprite.setTextureRect(Tetromino::textureRectByType(type));
			sprite.setPosition(sf::Vector2f(column, row) * Tetromino::Size.x + positionOffset);
			target.draw(sprite, states);
		}
	}
}

TetrominoTypes::Type Playfield::cellTypeByRowAndColumn(int row, int column) const
{
	return mPlayfieldData[row * Columns + column];
}

bool Playfield::addLinesToClear(sf::Time elapsedTime)
{
	int clearedLinesCount = 0;	
	int lineCounter = Rows - 1;

	while (lineCounter >= 0)
	{
		while (isLineFilledUp(lineCounter))
		{
			mLinesToClear.push_back(lineCounter);
			++clearedLinesCount;
			--lineCounter;
		}

		if (clearedLinesCount > 0)
		{
			mGame.updateScore(clearedLinesCount);
		}
		else
		{
			--lineCounter;
		}

		clearedLinesCount = 0;
	}

	if (hasLinesToClear())
	{
		if (mLinesToClear.size() >= 4)
		{
			mSoundPlayer.play(SoundPlayer::SoundEffect::Tetris);
		}
		else
		{
			mSoundPlayer.play(SoundPlayer::SoundEffect::Line_cleared);
		}
		return true;
	}
	return false;
}

bool Playfield::hasLinesToClear() const
{
	return mLinesToClear.size() > 0;
}

void Playfield::clearLines()
{
	int clearedLinesCount = 0;	
	int lineCounter = Rows - 1;

	while (lineCounter >= 0)
	{
		while (isLineFilledUp(lineCounter))
		{
			clearLine(lineCounter);
			++clearedLinesCount;
			--lineCounter;
		}

		if (clearedLinesCount > 0)
		{
			dropBlocks(clearedLinesCount, lineCounter);
			lineCounter = Rows - 1; 
		}

		clearedLinesCount = 0;
		--lineCounter;
	}

	mLinesToClear.clear();
}

std::vector<int> Playfield::linesToClear() const
{
	return mLinesToClear;
}

bool Playfield::isGameOver(Tetromino& tetromino) const
{
	for (auto& tetrominoBlock : tetromino.shapeData())
	{
		sf::Vector2f relPos = tetromino.relativePosition(tetrominoBlock);
		if (cellTypeByRowAndColumn(relPos.y, relPos.x) != TetrominoTypes::Type::None)
		{
			return true;
		}
	}

	return false;
}

void Playfield::mergeTetromino(Tetromino& tetromino)
{
	for (auto& tetrominoBlock : tetromino.shapeData())
	{
		sf::Vector2f relPos = tetromino.relativePosition(tetrominoBlock);
		setCellTypeByRowAndColumn(relPos.y, relPos.x, tetromino.type());
	}
}

void Playfield::setCellTypeByRowAndColumn(int row, int column, TetrominoTypes::Type type)
{
	assert(row < Rows);
	assert(column < Columns);

	mPlayfieldData[row * Columns + column] = type;	
}

bool Playfield::isLineFilledUp(int row) const
{
	for (int column = 0; column < Columns; ++column)
	{
		if (cellTypeByRowAndColumn(row, column) == TetrominoTypes::Type::None)
		{
			return false;
		}
	} 

	return true;
}

void Playfield::clearLine(int row)
{
	for (int column = 0; column < Columns; ++column)
	{
		setCellTypeByRowAndColumn(row, column, TetrominoTypes::Type::None);	
	} 
}

void Playfield::dropBlocks(int clearLinesCount, int fromRow)
{
	while (fromRow >= 0)
	{
		for (int column = 0; column < Columns; ++column)
		{
			setCellTypeByRowAndColumn(fromRow + clearLinesCount, column, cellTypeByRowAndColumn(fromRow, column));	
			setCellTypeByRowAndColumn(fromRow, column, TetrominoTypes::Type::None);	
		} 
		--fromRow;
	}
}
