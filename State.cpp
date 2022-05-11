#include "State.hpp"
#include "StateStack.hpp"

State::Context::Context(sf::RenderWindow& window, sf::Font& font, sf::Texture& texture, SoundPlayer& soundPlayer, MusicPlayer& musicPlayer, Game& game)
: window(window)
, font(font)
, spritesheet(texture)
, soundPlayer(soundPlayer)
, musicPlayer(musicPlayer)
, game(game)
{
}

State::State(StateStack& stack, Context context)
: mStack(stack)
, mContext(context)
{
}

State::~State()
{
}

void State::requestStackPush(States::ID stateID)
{
	mStack.pushState(stateID);
}

void State::requestStackPop()
{
	mStack.popState();
}

void State::requestStateClear()
{
	mStack.clearStates();
}

State::Context& State::context()
{
	return mContext;
}
