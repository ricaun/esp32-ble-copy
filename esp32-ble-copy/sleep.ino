/*
   sleep.ino
*/

#define TIMESLEEP     60
#define WAKESLEEP     300

uint64_t timesleep = TIMESLEEP;

//! Long time delay, it is recommended to use shallow sleep, which can effectively reduce the current consumption
void espDelay(int ms)
{
  esp_sleep_enable_timer_wakeup(ms * 1000);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
  esp_light_sleep_start();
}

void sleep()
{
  lcd_off();

  espDelay(50);

  esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_TIMER);
  //uint64_t ms = WAKESLEEP * 1000;
  //esp_sleep_enable_timer_wakeup((uint64_t) ms * 1000);

  sleepButton();
}

void sleepButton()
{
  const int ext_wakeup_pin_1 = BUTTON_1;
  const uint64_t ext_wakeup_pin_1_mask = 1ULL << ext_wakeup_pin_1;
  const int ext_wakeup_pin_2 = BUTTON_2;
  const uint64_t ext_wakeup_pin_2_mask = 1ULL << ext_wakeup_pin_2;

  esp_sleep_enable_ext1_wakeup((uint64_t) 1ULL << BUTTON_1, ESP_EXT1_WAKEUP_ALL_LOW);
  //esp_sleep_enable_ext1_wakeup(ext_wakeup_pin_1_mask | ext_wakeup_pin_2_mask, ESP_EXT1_WAKEUP_ALL_LOW);
  esp_deep_sleep_start();
}


static uint64_t timeSleep = 0;

void sleep_time()
{
  timeSleep = millis();
}

void sleep_loop()
{
  if (millis() - timeSleep > timesleep * 1000)
  {
    sleep_time();
    sleep();
  }
}

void sleep_set(uint64_t s)
{
  timesleep = s;
}

uint32_t sleep_get()
{
  return timesleep;
}

uint32_t sleep_millis()
{
  return (timesleep * 1000) - (millis() - timeSleep);
}
