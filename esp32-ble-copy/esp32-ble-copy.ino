/*
 * ble_copy.ino
 * ricaun
 * 30-01-2020
 */

TaskHandle_t Task0;

void setup() {
  Serial.begin(115200);

  lcd_setup();
  button_setup();

  mac_setup();
  ble_setup();

  adc_setup();

  xTaskCreatePinnedToCore(
    TaskCode0,   /* Task function. */
    "IDLE0",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task0,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */

}

void loop() {
  if (runEvery(1000))
  {
    //Serial.println(mac_json());
    lcd_str(mac_string());
  }
  ble_loop();
}

void TaskCode0( void * pvParameters ) {
  while (1)
  {
    lcd_loop();
    sleep_loop();
    button_loop();
    vTaskDelay(10);
  }
}

boolean runEvery(unsigned long interval)
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}
