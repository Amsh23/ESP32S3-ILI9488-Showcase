#pragma once

#include <cstdint>

namespace config {
constexpr const char* kWifiSsid = "YOUR_WIFI_SSID";
constexpr const char* kWifiPassword = "YOUR_WIFI_PASSWORD";
constexpr const char* kNtpServer1 = "pool.ntp.org";
constexpr const char* kNtpServer2 = "time.nist.gov";
constexpr long kGmtOffsetSeconds = 0;
constexpr int kDaylightOffsetSeconds = 0;
constexpr int kScreenWidth = 480;
constexpr int kScreenHeight = 320;
constexpr int kFrameMs = 33;
constexpr int kBacklightPin = -1;
constexpr uint8_t kDefaultBrightness = 220;
}
