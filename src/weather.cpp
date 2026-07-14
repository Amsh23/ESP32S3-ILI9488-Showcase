#include "weather.h"
// Initializes the placeholder weather provider; future API setup belongs here.
void WeatherService::begin() {}
// Placeholder update hook for a future non-blocking weather API refresh.
void WeatherService::update() {}
// Returns UI-safe placeholder text until the weather API is implemented.
WeatherSnapshot WeatherService::snapshot() const { return {"Weather API", "Soon"}; }
