#pragma once

#include <time.h>


void initClock()
{

  configTime(
    GMT_OFFSET_SEC,
    DAYLIGHT_OFFSET_SEC,
    "pool.ntp.org",
    "time.nist.gov"
  );

}



String getTimeNow()
{

  struct tm timeinfo;


  if(!getLocalTime(&timeinfo))
  {
    return "NO TIME";
  }


  char buffer[20];


  strftime(
    buffer,
    sizeof(buffer),
    "%I:%M:%S %p",
    &timeinfo
  );


  return String(buffer);

}




String getDateNow()
{

  struct tm timeinfo;


  if(!getLocalTime(&timeinfo))
  {
    return "NO DATE";
  }


  char buffer[40];


  strftime(
    buffer,
    sizeof(buffer),
    "%d/%m/%Y",
    &timeinfo
  );


  return String(buffer);

}