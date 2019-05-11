#pragma once
#include <utility>

namespace Utils
{

	template<class T>
	bool isValid(T _t)
	{
		return _t != nullptr;
	}

}