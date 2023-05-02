// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <lwip/apps/sntp.h>
#include "esp_log.h"

#include "azure_c_shared_utility/agenttime.h"
#include "azure_c_shared_utility/xlogging.h"

static const char * TAG = "esp-azure-time";


void initialize_sntp(void)
{
  if (sntp_enabled())
    return;
  ESP_LOGD(TAG, "Initializing SNTP");
  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setservername(0, "pool.ntp.org");
  sntp_init();
}

time_t get_time(time_t * currentTime)
{
  bool have_notified = false;
  while (true)
  {
    time_t now;
    time(&now);
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);
    // Is time set? If not, tm_year will be (1970 - 1900).
    if (timeinfo.tm_year > (2020 - 1900))
      return now;
    assert(timeinfo.tm_year == 1970 - 1900);
    if (!have_notified)
    {
      ESP_LOGI(TAG, "Waiting for NTP to set system time...");
      have_notified = true;
    }
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

double get_difftime(time_t stopTime, time_t startTime)
{
    return (double)stopTime - (double)startTime;
}

struct tm* get_gmtime(time_t* currentTime)
{
    return NULL;
}

char* get_ctime(time_t* timeToGet)
{
    return NULL;
}
