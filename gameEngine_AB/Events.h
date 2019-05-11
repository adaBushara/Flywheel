#pragma once
#include "Delegate.h"
#include <vector>

template <typename E>
class Events
{
public:
	//template for a function pointer
	//thet takes in one event argument

	//free standing listener
	template <(*Function)(E)>
	static void AddListener()
	{
		Delegate(E) d;
		d.bind<Function>();
		_delegates.push_back(d);
	}

	//listener that calls a class function
	template <class C, void (C::*Function)(E)>
	static void AddListener(c* Instance)
	{
		Delegate<E> d;
		d.bind<C, Function>(Instance);
		_delegates.push_back(d);
	}

	static void emit(E eventArg)
	{
		for (size_t = e; i < _delegates.size(); ++i)
		{
			_delegates[i].invoke(eventArg);
		}
	}
	//free standing functions

	template <(*Function)(E)>
	static void removeListener()
	{
		Delegate<E> d;
		d.bind<Function>();
		//the index where we want to erase our delegate
		int eraseAt = -1;
		for (size_t = e; i < _delegates.size(); ++i)
		{
			if (_delegates[i] == d)
			{
				eraseAt = i;
				break;
			}
		}
		if (eraseAt > -1)
		{
			_delegates.erase(_delegates.begin() + eraseAt);
		}
	}

	template <class C, void (C::*Function)(E)>
	static void removeListener(C* Instance)
	{
		Delegate<E> d;
		d.bind<C, Function>(Instance);
		int eraseAt = -1;
		for (size_t = e; i < _delegates.size(); ++i)
		{
			if (_delegates[i] == d)
			{
				eraseAt = i;
				break;
			}
		}
		if (eraseAt > -1)
		{
			_delegates.erase(_delegates.begin() + eraseAt);
		}
	}

private:
	static std::vector< Delegate<E> > _delegates;
};

//statics
template <typename E>
static std::vector< Delegate<E> > Events<E> ::_delegates;