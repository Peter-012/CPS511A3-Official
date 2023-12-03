

#include "header/Util.h"
#include <chrono>

const float PI = 3.14159265;

long long currentTimeMillis() {
	return std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::system_clock::now().time_since_epoch()).count();
}

double toRadians(float angDeg) {
	return (angDeg * PI) / 180;
}


double toDegrees(float angRads) {
	return (180 * angRads) / PI;
}




