#pragma once
#include <ctime>
#include <string>

class ClockService {
public:
  void begin();
  void update();
  bool hasTime() const;
  std::tm now() const;
  std::string timeText() const;
  std::string dateText() const;
  std::string dayText() const;
private:
  bool synced_{false};
};
