#pragma once
#include <memory>
#include <math.h>

template<class T>
class Vector2
{

public:

	//basic constructor
	Vector2()
	{
		//zero out our data
		memset(data, 0, sizeof(T) * 2);
	}

	//Constructor to initialize data
	Vector2(T _x, T _y)
	{

		x = _x;
		y = _y;
	}

	Vector2(T* _data)
	{
		//take in an array and copy the first 2 elements
		memcpy(data, _data, sizeof(T) * 2);
	}

	Vector2(Vector2<T> & obj)
	{
		memcpy(data, obj.data, sizeof(T) * 2);
	}


	//Operators
	Vector2<T>& operator=(const Vector2<T> &rhs)
	{
		//assign data
		memcpy(data, rhs.data, sizeof(T) * 2);
		return *this;
	}
	Vector2<T>& operator+=(Vector2<T> &rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}

	Vector2<T>& operator*=(const T& rhs)
	{
		this->x *= rhs;
		this->y *= rhs;
		return *this;
	}
	Vector2<T> operator*(const T& rhs)
	{
		return (*this *= rhs);
	}

	Vector2<T> operator-()
	{
		return (*this * (T)-1);
	}
	Vector2<T>& operator-=(Vector2<T> &rhs)
	{
		return *this += -rhs;
	}
	
	Vector2<T>& operator/=(T &rhs)
	{
		this->x /= rhs;
		this->y /= rhs;
		return *this
	}

	Vector2<T> operator/(const T& rhs)
	{
		return (*this /= rhs);
	}

	union 
	{	
		struct 
		{
			T x;
			T y;
		};
		
		T data[2];
	};
	


};

template <class  T>
inline Vector2<T> operator-(Vector2<T> lhs,  Vector2<T> &rhs)
{
	return (lhs -= rhs);
}
template <class  T>
inline Vector2<T> operator+(Vector2<T> lhs,Vector2<T> &rhs)
{
	return (lhs += rhs);
}

//typedefs, this lets use easier naming and not have to deal with templates
typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;