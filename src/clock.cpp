#include "clock.h"
#include "config.h"
#include <Arduino.h>
#include <time.h>

// Starts SNTP using the configured WiFi timezone and NTP servers.
void ClockService::begin() { configTime(config::kGmtOffsetSeconds, config::kDaylightOffsetSeconds, config::kNtpServer1, config::kNtpServer2); }

// Refreshes the cached synchronization flag without blocking rendering.
void ClockService::update() { synced_ = hasTime(); }

// Returns true once NTP has produced a plausible modern calendar date.
bool ClockService::hasTime() const { std::tm t{}; return getLocalTime(&t, 5) && (t.tm_year + 1900) >= 2024; }

std::tm ClockService::now() const { std::tm t{}; getLocalTime(&t, 5); return t; }

static std::string fmt(const std::tm& t, const char* f) { char b[32]; strftime(b, sizeof(b), f, &t); return b; }
std::string ClockService::timeText() const { return hasTime() ? fmt(now(), "%H:%M:%S") : "--:--:--"; }
std::string ClockService::dateText() const { return hasTime() ? fmt(now(), "%Y-%m-%d") : "Syncing NTP"; }
std::string ClockService::dayText() const { return hasTime() ? fmt(now(), "%A") : "Please wait"; }
