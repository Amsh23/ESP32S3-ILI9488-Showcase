#pragma once
#include <string>
struct WeatherSnapshot { std::string label; std::string temperature; };
class WeatherService { public: void begin(); void update(); WeatherSnapshot snapshot() const; };
