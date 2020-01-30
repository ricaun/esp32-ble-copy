/*
   mac.ino
*/

#define MACMAX 50

uint64_t macs[MACMAX];
int16_t rssis[MACMAX];
uint8_t counts[MACMAX];
uint16_t macs_max = 0;

void mac_setup()
{
  macs_max = 0;
  for (int i = 0; i < MACMAX; i++)
  {
    macs[i] = 0;
    rssis[i] = 0;
    counts[i] = 0;
  }
}

bool mac_exists(uint64_t m)
{
  for (int i = 0; i < macs_max; i++)
  {
    if (macs[i] == m)
    {
      if (counts[i] != 255) counts[i]++;
      return true;
    }
  }
  return false;
}

void mac_add_random()
{
  uint64_t m = 0;
  m += (uint64_t) random(255) << 56;
  m += (uint64_t) random(255) << 48;
  m += (uint64_t) random(255) << 40;
  m += (uint64_t) random(255) << 32;
  m += (uint64_t) random(255) << 24;
  m += (uint64_t) random(255) << 16;
  m += (uint64_t) random(255) << 8;
  m += (uint64_t) random(255);
  if (mac_exists(m) == false)
  {
    rssis[macs_max] = 0;
    macs[macs_max++] = m;
  }
}

void mac_add(uint8_t *b, int16_t rssi)
{
  uint64_t m = 0;
  m += (uint64_t) b[0] << 56;
  m += (uint64_t) b[1] << 48;
  m += (uint64_t) b[2] << 40;
  m += (uint64_t) b[3] << 32;
  m += (uint64_t) b[4] << 24;
  m += (uint64_t) b[5] << 16;
  m += (uint64_t) b[6] << 8;
  m += (uint64_t) b[7];
  if (mac_exists(m) == false)
  {
    rssis[macs_max] = rssi;
    macs[macs_max++] = m;
  }
}

std::string mac_get_string(uint16_t i)
{
  std::string str = "";
  char b[8];
  b[0] = (uint64_t) macs[i] >> 56;
  b[1] = (uint64_t) macs[i] >> 48;
  b[2] = (uint64_t) macs[i] >> 40;
  b[3] = (uint64_t) macs[i] >> 32;
  b[4] = (uint64_t) macs[i] >> 24;
  b[5] = (uint64_t) macs[i] >> 16;
  b[6] = (uint64_t) macs[i] >> 8;
  b[7] = (uint64_t) macs[i];
  for (int i = 0; i < 8; i++) {
    str += b[i];
  }
  return str;
}

uint16_t _mac_next = 0;

std::string mac_get_next()
{
  int i = 0;

  i = _mac_next;
  _mac_next++;
  if (_mac_next >= macs_max) _mac_next = 0;
  
  std::string str = "";
  char b[8];
  b[0] = (uint64_t) macs[i] >> 56;
  b[1] = (uint64_t) macs[i] >> 48;
  b[2] = (uint64_t) macs[i] >> 40;
  b[3] = (uint64_t) macs[i] >> 32;
  b[4] = (uint64_t) macs[i] >> 24;
  b[5] = (uint64_t) macs[i] >> 16;
  b[6] = (uint64_t) macs[i] >> 8;
  b[7] = (uint64_t) macs[i];
  for (int i = 0; i < 8; i++) {
    str += b[i];
  }
  return str;
}

uint16_t mac_size()
{
  return macs_max;
}

String mac_get(int i)
{
  char b[8];
  b[0] = (uint64_t) macs[i] >> 56;
  b[1] = (uint64_t) macs[i] >> 48;
  b[2] = (uint64_t) macs[i] >> 40;
  b[3] = (uint64_t) macs[i] >> 32;
  b[4] = (uint64_t) macs[i] >> 24;
  b[5] = (uint64_t) macs[i] >> 16;
  b[6] = (uint64_t) macs[i] >> 8;
  b[7] = (uint64_t) macs[i];

  String mac = "";

  for (int j = 0; j < 8; j++)
  {
    char charVal[4];
    sprintf(charVal, "%02X", b[j]);
    mac += charVal;
  }
  return mac;
}

String mac_string()
{
  String str = "";
  for (int i = 0; i < macs_max; i++)
  {
    String mac = mac_get(i);
    str += mac;
    str += " ";
    str += counts[i];
    str += "\n";
    if (i > 20) break;
  }
  return str;
}

String mac_json()
{
  String json = "";

   float vcc = readVcc();

  json += "[";
  for (int i = 0; i < macs_max; i++)
  {

    String mac = mac_get(i);

    if (mac == "F0D6AA2361E6") lcd_str(mac + " " + rssis[i] + " " + counts[i]);

    json += "{";
    json += "\"";
    json += "mac";
    json += "\"";
    json += ":";
    json += "\"";

    uint32_t m2 = macs[i] >> 32;
    uint32_t m = macs[i];

    json += mac;

    /*
      char charVal[32];
      sprintf(charVal, "%02X", m2);
      json += charVal;

      sprintf(charVal, "%04X", m);
      json += charVal;
    */
    json += "\"";

    json += ",";
    json += "\"";
    json += "rssi";
    json += "\"";
    json += ":";
    json += rssis[i];

    json += ",";
    json += "\"";
    json += "count";
    json += "\"";
    json += ":";
    json += counts[i];

    json += ",";
    json += "\"";
    json += "vcc";
    json += "\"";
    json += ":";
    json += vcc;

    json += "}";
    //json += "\n";

    if (i < macs_max - 1)
      json += ",";
  }
  json += "]";
  //json += "\n";
  return json;
}
