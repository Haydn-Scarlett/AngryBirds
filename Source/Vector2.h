#pragma once
class vector2
{
public:
	// construction
	vector2();
	vector2(float xval, float yval);
	vector2(const vector2& rhs);
	/**
	*  Destructor.
	*/
	~vector2();

	// operations
	vector2 operator*(float scalar);
	vector2 normalise();
	float getX();
	float getY();
	void setX(float newX);
	void setY(float newY);
	vector2 add(vector2 a);
	vector2 subtract(vector2 a);
	float getDistance(vector2 a);
	float getMagnitude();
	vector2 multiply(float scalar);
	float getScalar(vector2 a);

private:

	// data
	float x = 0;
	float y = 0;
};