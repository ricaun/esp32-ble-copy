/*
   button.ino
*/

#define BUTTON_1        35
#define BUTTON_2        0

#include <Button2.h>

Button2 btn1(BUTTON_1);
Button2 btn2(BUTTON_2);

void handler1(Button2& btn);
void handler2(Button2& btn);

void button_setup()
{
  btn1.setClickHandler(handler1);
  btn1.setLongClickHandler(handler1);
  btn1.setDoubleClickHandler(handler1);
  btn1.setTripleClickHandler(handler1);

  btn2.setClickHandler(handler2);
  btn2.setLongClickHandler(handler2);
  btn2.setDoubleClickHandler(handler2);
  btn2.setTripleClickHandler(handler2);

}

void button_loop()
{
  btn1.loop();
  btn2.loop();
}


void handler1(Button2& btn) {
  sleep_time();
  switch (btn.getClickType()) {
    case SINGLE_CLICK:
      sleep_set(60);
      break;
    case DOUBLE_CLICK:
      sleep_set(300);
      break;
    case TRIPLE_CLICK:
      sleep_set(600);
      break;
    case LONG_CLICK:
      sleep();
      break;
  }
}

void handler2(Button2& btn) {
  sleep_time();
  switch (btn.getClickType()) {
    case SINGLE_CLICK:
      ble_state_change();
      break;
    case DOUBLE_CLICK:
      mac_add_random();
      break;
    case TRIPLE_CLICK:
      for (int i = 0; i < 10; i++)
        mac_add_random();
      break;
    case LONG_CLICK:
      sleep();
      break;
  }
}
