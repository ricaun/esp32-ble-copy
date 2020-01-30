/*
 * ble.ino
 */

#define SERVICE_UUID        "0000fef3-0000-1000-8000-00805f9b34fb"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEBeacon.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>


// ---------------------------- scan ----------------------------- //

int scanTime = 1;
BLEScan* pBLEScan;

boolean _ble_state = false;
int _ble_count = 0;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {

      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());

      if (!advertisedDevice.haveServiceUUID())
        return;

      String s = "";
      s += advertisedDevice.getServiceUUID().toString().c_str();

      if (s != SERVICE_UUID) return;

      std::string strServiceData = advertisedDevice.getServiceData();
      uint8_t cServiceData[100];
      strServiceData.copy((char *)cServiceData, strServiceData.length(), 0);

      //Serial.println(advertisedDevice.getRSSI());

      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
      if (strServiceData.length() == 8)
        mac_add(cServiceData, advertisedDevice.getRSSI());
    }
};

void ble_setup_scan()
{
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

void ble_loop_scan()
{
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  _ble_count = foundDevices.getCount();
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
}

int ble_count()
{
  return _ble_count;
}

// ---------------------------- beacon ----------------------------- //

static BLEUUID serviceUUID((uint16_t)0xfef3);

BLEAdvertising *pAdvertising;

byte data[] = {0xDA, 0x4E, 0xF5, 0x1C, 0x16, 0x52, 0x90, 0xA1};
int size = 8;

void ble_loop_send()
{
  ble_change();
  pAdvertising->start();
  Serial.println("Advertizing started...");
  delay(100);
  pAdvertising->stop();
  delay(100);
}

void ble_change()
{
  pAdvertising = BLEDevice::getAdvertising();

  pAdvertising->addServiceUUID(serviceUUID);
  
  BLEAdvertisementData advertisementData = BLEAdvertisementData();

  std::string strServiceData = "";

  for (int i = 0; i < size; i++) {
    strServiceData += data[i];
  }

  strServiceData = mac_get_next();

  advertisementData.setServiceData(serviceUUID, strServiceData);

  pAdvertising->setAdvertisementData(advertisementData);
}

// ---------------------------- setup ----------------------------- //

int ble_state()
{
  return _ble_state;
}

String ble_state_string()
{
  String str = "";
  if (!_ble_state)
    str += "Scan";
  else
    str += "Send";
  return str;
}

void ble_state_change()
{
  _ble_state = !_ble_state;
}

void ble_setup()
{
  ble_setup_scan();
}

void ble_loop()
{
  if (!_ble_state)
  {
    ble_loop_scan();
  }
  else
  {
    ble_loop_send();
  }
}
