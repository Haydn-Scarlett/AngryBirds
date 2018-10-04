#include <math.h>
#include "Vector2.h"

/**
*   @brief   Constructor.
*   @details Requires values for it's x and y magnitudes.
*   @return  void
*/
vector2::vector2()
{

};


/**
*   @brief   Destructor.
*   @details Requires values for it's x and y magnitudes.
*   @return  void
*/
vector2::~vector2()
{

}
/**
*   @brief   overloaded Constructor
*   @brief   sets the x and y values of the vector.
*/
vector2::vector2(float xval, float yval)
{
	x = xval;
	y = yval;
}

/**
*   @brief   Copies a vector.
*   @details Constructs a new vector2 from an existing.
*   @return  void
*/
vector2::vector2(const vector2 & rhs)
{
	this->x = rhs.x;
	this->y = rhs.y;
}

/**
*   @brief   Normalises vector.
*   @details Turns the vector into a unit vector.
*   @return  void
*/
vector2 vector2::normalise()
{
	vector2 result;
	float len = getMagnitude();
	if (len != 0.0f)
	{
		result.setX(getX() / len);
		result.setY(getY() / len);
	}
	else
	{
		result.setX(0.0f);
		result.setY(0.0f);
	}

	return result;
}

/**
*   @brief   Scales the vector.
*   @details Uses a single scalar value to adjust the vector.
*   @return  void
*/
vector2 vector2::operator*(float scalar)
{
	vector2 vec(*this);
	vec.x *= scalar;
	vec.y *= scalar;
	return vec;
}


float vector2::getX()
{
	return x;
}

float vector2::getY()
{
	return y;
}

void vector2::setX(float newX)
{
	x = newX;
}

void vector2::setY(float newY)
{
	y = newY;
}



/**
*   @brief   add
*   @details adds two vectors
*   @return  Vector
*/
vector2 vector2::add(vector2 a)
{
	vector2 result(getX() + a.getX(), getY() + a.getY());
	return result;
}



/**
*   @brief   subtract
*   @details subtracts one vector from another
*   @return  Vector
*/
vector2 vector2::subtract(vector2 a)
{
	vector2 result(getX() - a.getX(), getY() + -a.getY());
	return result;
}

/**
*   @brief   getDistance
*   @details gets the distance between two vectors
*   @return  float
*/
float vector2::getDistance(vector2 a)
{
	return (float)sqrt((a.getX() - getX()) *
		(a.getX() - getX()) + (a.getY() - getY()) *
		(a.getY() - getY()));
}

/**
*   @brief   Get magnitude
*   @details Calculates the magnitude of the vector
*   @return  float
*/
float vector2::getMagnitude()
{
	return (sqrt(x * x) + (y * y));
}



/**
*   @brief   multiply
*   @details multiplys a vector by the scalar
*   @return  Vector
*/
vector2 vector2::multiply(float scalar)
{
	vector2 result(getX() * scalar, getY() * scalar);
	return result;
}

/**
*   @brief   Get scalar
*   @details Calculates the dot product of two vectors
*   @return  float
*/
float vector2::getScalar(vector2 a)
{
	return((getX() * a.getX()) - (getY() * a.getY()));
}