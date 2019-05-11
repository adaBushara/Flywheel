#pragma once
#include <limits.h>
#include <math.h>
#include <iostream>
#include <Box2D\Box2D.h>
#include "Position.h" 

#if defined(max) && defined(min)
#pragma push_macro("max")
#undef max
#pragma push_macro("min")
#undef min
#endif


namespace Math
{
	

	template <class T>
	typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type 
		approximately( T a, T b, unsigned int ulp = 2)
	{
		return (std::abs(a - b) <= std::numeric_limits<T>::epsilon() * std::abs(a + b) * ulp) || ( std::abs(a - b) < std::numeric_limits<T>::min() );
	}

	bool approximately(const Position &p1, const Position &p2, unsigned int precision = 2)
	{
		return approximately(p1.x, p2.x, precision) & approximately(p1.y, p2.y, precision);
	}
	

	bool approximately(const Position &p,const  b2Vec2 v, unsigned int precision =2)
	{
		return approximately(p.x, v.x, precision) & approximately(p.y, v.y, precision);
	}

	const float Rad2Deg = 180.0f / M_PI;

	const float Deg2Rad = M_PI / 180.0f;
	

}

#if  !defined(max) && !defined(min)
#pragma pop_macro("min")
#pragma pop_macro("max")
#endif // ! max
