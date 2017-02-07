#pragma once 

class Vector2i {
public:
	int x;
	int y;

	Vector2i(const int x = 0, const int y = 0) : x(x), y(y) {}
	bool operator!=(const Vector2i& r) const {
		if(x != r.x || y != r.y) return true;
		return false;
	}
	Vector2i operator-(const Vector2i& r) const {
		return Vector2i(x - r.x, y - r.y);
	}
	void show() const { cout << "x: " << x << " y: " << y << endl; }
};