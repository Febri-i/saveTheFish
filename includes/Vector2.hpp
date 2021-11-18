#pragma once
#include <cmath>

template<typename typeName>
class Vector2
{
	public:
	Vector2(typeName ix, typeName iy): x(ix), y(iy){}

	Vector2(typeName ia): x(ia), y(ia){}

	Vector2(): x(0), y(0){}

	Vector2<typeName> operator+(const Vector2<typeName> &right) const{
		Vector2<typeName> temp(*this);
		temp += right;
		return temp;
	}

	Vector2<typeName> operator-(const Vector2<typeName> &right) const{
		Vector2<typeName> temp(*this);
		temp -= right;
		return temp;
	}

	Vector2<typeName> operator*(const Vector2<typeName> &right) const{
		Vector2<typeName> temp(*this);
		temp *= right;
		return temp;
	}

	Vector2<typeName> operator/(const Vector2<typeName> &right) const{
		Vector2<typeName> temp(*this);
		temp /= right;
		return temp;
	}

	Vector2<typeName>& operator+=(const Vector2<typeName> &right){
		x += right.x;
		y += right.y;
		return *this;
	}
	
	Vector2<typeName>& operator-=(const Vector2<typeName> &right){
		x -= right.x;
		y -= right.y;
		return *this;
	}
	
	Vector2<typeName>& operator*=(const Vector2<typeName> &right){
		x *= right.x;
		y *= right.y;
		return *this;
	}
	
	Vector2<typeName>& operator/=(const Vector2<typeName> &right){
		if(right.x != 0 && right.y != 0){
			x /= right.x;
			y /= right.y;
		}
		return *this;
	}
	
	Vector2<typeName> normalize(){
		if(x != 0 || y != 0){
			typeName lenght = sqrt(pow(x, 2) + pow(y, 2));
			return Vector2<typeName>(x / lenght, y / lenght);
		}
	}

	typeName x, y;
};
