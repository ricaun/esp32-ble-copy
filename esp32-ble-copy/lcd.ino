/*
 * lcd.ino
 */

#define USER_SETUP_LOADED

#define ST7789_DRIVER

#define TFT_WIDTH  135
#define TFT_HEIGHT 240

#define CGRAM_OFFSET      // Library will add offsets required

#define TFT_MOSI            19
#define TFT_SCLK            18
#define TFT_CS              5
#define TFT_DC              16
#define TFT_RST             23

#define TFT_BL          4  // Display backlight control pin

#define TFT_BACKLIGHT_ON HIGH  // HIGH or LOW are options

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF

#define SMOOTH_FONT

#define SPI_FREQUENCY  40000000   // Maximum for ILI9341
#define SPI_READ_FREQUENCY  6000000 // 6 MHz is the maximum SPI read speed for the ST7789V

#include <TFT_eSPI.h>
#include <SPI.h>
#include "WiFi.h"
#include <Wire.h>

#ifndef TFT_DISPOFF
#define TFT_DISPOFF 0x28
#endif

#ifndef TFT_SLPIN
#define TFT_SLPIN   0x10
#endif

#define ADC_EN          14
#define ADC_PIN         34

TFT_eSPI tft = TFT_eSPI(135, 240); // Invoke custom library

String _lcd_str = "";

void lcd_setup()
{
  tft.init();
  tft.setRotation(2);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(1);

  if (TFT_BL > 0) { // TFT_BL has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
    pinMode(TFT_BL, OUTPUT); // Set backlight pin to output mode
    digitalWrite(TFT_BL, TFT_BACKLIGHT_ON); // Turn backlight on. TFT_BACKLIGHT_ON has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
  }

  //tft.drawString("...", tft.width() / 2, tft.height() / 2);
}

void lcd_loop()
{
  if (lcd_runEvery(500))
  {
    if (ble_state())
      tft.setTextColor(TFT_BLUE, TFT_BLACK);
    else
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
    
    tft.setCursor(0, 0);
    String str = "";
    str += timeOn(0);
    str += " - ";
    str += timeOn2(sleep_millis());
    str += "\n";
    
    str += ESP.getFreeHeap();
    str += " - ";
    str += readVcc();
    str += "\n";

    str += ble_state_string();
    str += " - ";
    str += mac_size();
    str += " - ";
    str += ble_count();
    str += "\n";
    
    str += "\n";
    str += _lcd_str;
    tft.println(str);
  }
}

void lcd_off()
{
  tft.fillScreen(TFT_BLACK);
  digitalWrite(TFT_BL, !TFT_BACKLIGHT_ON);
  tft.writecommand(TFT_DISPOFF);
  tft.writecommand(TFT_SLPIN);
}

void lcd_backlight(boolean on)
{
  if (on)
    digitalWrite(TFT_BL, TFT_BACKLIGHT_ON);
  else
    digitalWrite(TFT_BL, !TFT_BACKLIGHT_ON);
}

void lcd_backlight2()
{
  digitalWrite(TFT_BL, !digitalRead(TFT_BL));
}

void lcd_str(String str)
{
  _lcd_str = str;
}

void lcd_print(String str)
{
  _lcd_str += str;
}

void lcd_println(String str)
{
  _lcd_str += str + "\n";
}

boolean lcd_runEvery(unsigned long interval)
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

String timeOn(unsigned long diff)
{
    String str = "";
    unsigned long t = (millis() - diff) / 1000;
    int s = t % 60;
    int m = (t / 60) % 60;
    int h = (t / 3600);
    str += h;
    str += ":";
    if (m < 10)
        str += "0";
    str += m;
    str += ":";
    if (s < 10)
        str += "0";
    str += s;
    return str;
}

String timeOn2(unsigned long diff)
{
    String str = "";
    unsigned long t = (diff) / 1000;
    int s = t % 60;
    int m = (t / 60) % 60;
    int h = (t / 3600);
    str += h;
    str += ":";
    if (m < 10)
        str += "0";
    str += m;
    str += ":";
    if (s < 10)
        str += "0";
    str += s;
    return str;
}
