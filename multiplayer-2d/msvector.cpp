#include "msvector.h"

float MSVector::length() {
	return sqrtf(powf(x, 2) + powf(y, 2));
}

float MSVector::distance(MSVector other) {
	return sqrtf(powf(other.x - x, 2) + powf(other.y - y, 2));
}

MSVector MSVector::plus(MSVector other) {
	return MSVector(x + other.x, y + other.y);
}

MSVector MSVector::minus(MSVector other) {
	return MSVector(x - other.x, y - other.y);
}

MSVector MSVector::scale(float amount) {
	return MSVector(x * amount, y * amount);
}