#include "TetrominoPreview.hpp"
#include "Playfield.hpp"

TetrominoPreview::TetrominoPreview(TetrominoTypes::Type type, sf::Texture& texture, Playfield& playfield, SoundPlayer& soundPlayer)
: Tetromino(type, texture, playfield, soundPlayer)
{
	mAbsolutePosition = blockOffset() + sf::Vector2f(Playfield::Columns + 2, Playfield::Rows / 2);
}

sf::Vector2f TetrominoPreview::blockOffset() const
{
	switch(mType)
	{
		case TetrominoTypes::Type::I:
		case TetrominoTypes::Type::J:
		case TetrominoTypes::Type::L:
		case TetrominoTypes::Type::O:
			return sf::Vector2f(0.f, 2.f);
			break;
		case TetrominoTypes::Type::T:
		case TetrominoTypes::Type::Z:
			return sf::Vector2f(0.f, 1.f);
			break;
		case TetrominoTypes::Type::S:
			return sf::Vector2f(0.f, 0.f);
			break;
	}
}
