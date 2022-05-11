#include "PauseState.hpp"
#include "Utility.hpp"

const sf::Time PauseState::TimePerTextBlink = sf::seconds(1.f);

PauseState::PauseState(StateStack& stack, Context context)
: State(stack, context)
{
	mOverlay.setSize(sf::Vector2f(800.f, 600.f));
	mOverlay.setFillColor(sf::Color(0, 0, 0, 220));

	mText.setFont(context.font);
	mText.setCharacterSize(40);
	mText.setString("Pause");
	mText.setFillColor(sf::Color::White);
	Utility::centerOrigin(mText);
	mText.setPosition(400, 300);

	context.musicPlayer.setVolume(20.f);
	context.soundPlayer.play(SoundPlayer::SoundEffect::Pause);
}

PauseState::~PauseState()
{
	context().soundPlayer.play(SoundPlayer::SoundEffect::Pause);
}

bool PauseState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
	{
		context().musicPlayer.setVolume(100.f);
		requestStackPop();	
	}
	return false;
}

bool PauseState::handleRealtimeInput(sf::Time elapsedTime)
{
	return false;
}

bool PauseState::update(sf::Time elapsedTime)
{
	mTextBlinkingTimeElapsed += elapsedTime;
	while (mTextBlinkingTimeElapsed >= TimePerTextBlink)
	{
		sf::Color color = mText.getFillColor();
		color.a = color.a == 255 ? 0 : 255;
		mText.setFillColor(color);
		mTextBlinkingTimeElapsed -= TimePerTextBlink;
	}

	return false;
}

void PauseState::draw()
{
	context().window.draw(mOverlay);
	context().window.draw(mText);
}

