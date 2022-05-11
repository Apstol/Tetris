#include "MenuState.hpp"
#include "Utility.hpp"

MenuState::MenuState(StateStack& stack, Context context)
: State(stack, context)
, mSelectedOption(MenuOptions::NewGame)
{
	sf::Text newGameOption;
	newGameOption.setFont(context.font);
	newGameOption.setCharacterSize(40);
	newGameOption.setString("New Game");
	newGameOption.setFillColor(sf::Color::White);
	Utility::centerOrigin(newGameOption);
	newGameOption.setPosition(400, 260);

	sf::Text exitOption;
	exitOption.setFont(context.font);
	exitOption.setCharacterSize(40);
	exitOption.setString("Exit");
	exitOption.setFillColor(sf::Color(211, 211, 211, 133));
	Utility::centerOrigin(exitOption);
	exitOption.setPosition(400, 320);

	mOptions = { newGameOption, exitOption };

	mControls.setFont(context.font);
	mControls.setCharacterSize(14);
	mControls.setStyle(sf::Text::Italic);
	mControls.setString("Arrows - move\ns - rotate\nspace - hard drop");
	mControls.setFillColor(sf::Color(211, 211, 211, 133));
	Utility::centerOrigin(mControls);
	mControls.setPosition(400, 420);

	context.musicPlayer.play(MusicPlayer::MusicTheme::Menu);
}

MenuState::~MenuState()
{
	context().musicPlayer.stop();
}

bool MenuState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
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
			switch (static_cast<MenuOptions>(mSelectedOption))
			{
				case MenuOptions::NewGame:
					requestStackPop();
					requestStackPush(States::Game);
					break;
				case MenuOptions::Exit:
					requestStackPop();
					break;
			}
		}
	}

	return true;
}

bool MenuState::handleRealtimeInput(sf::Time elapsedTime)
{
	return true;
}

bool MenuState::update(sf::Time elapsedTime)
{
	for (sf::Text& option : mOptions)
	{
		option.setColor(sf::Color(211, 211, 211, 133));
	}
	mOptions[mSelectedOption].setColor(sf::Color::White);

	return true;
}

void MenuState::draw()
{
	for (sf::Text& option : mOptions)
	{
		context().window.draw(option);
	}
	context().window.draw(mControls);
}
