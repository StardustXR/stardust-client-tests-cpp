#pragma once

inline float clamp(float d, float min, float max) {
	const float t = d < min ? min : d;
	return t > max ? max : t;
}

inline float map(float d, float inMin, float inMax, float outMin, float outMax) {
	return (d - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

inline float mapClamped(float d, float inMin, float inMax, float outMin, float outMax) {
	return clamp(map(d, inMin, inMax, outMin, outMax), outMin, outMax);
}
