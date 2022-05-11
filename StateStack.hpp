#pragma once

#include <SFML/System.hpp>

#include <vector>
#include <utility>
#include <functional>
#include <map>

#include "State.hpp"
#include "StateIdentifiers.hpp"

class StateStack : private sf::NonCopyable
{
	public:
		enum Action
		{
			Push,
			Pop,
			Clear
		};

		explicit StateStack(State::Context context);
		
		template <typename T>
		void registerState(States::ID stateID);

		void handleEvent(const sf::Event& event);
		void handleRealtimeInput(sf::Time elapsedTime);
		void update(sf::Time elapsedTime);
		void draw();

		void pushState(States::ID stateID);
		void popState();
		void clearStates();
		bool isEmpty() const;

	private:
		State::Ptr createState(States::ID stateID);
		void applyPendingChanges();

		struct PendingChange
		{
			explicit PendingChange(Action action, States::ID stateID = States::None);

			Action action;
			States::ID stateID;
		};

		std::vector<State::Ptr> mStack;
		std::vector<PendingChange> mPendingList;

		State::Context mContext;
		std::map<States::ID, std::function<State::Ptr()>> mFactories;
};

template <typename T>
void StateStack::registerState(States::ID stateID)
{
	mFactories[stateID] = [this] ()
	{
		return State::Ptr(std::make_unique<T>(*this, mContext));
	};
}
