/*
 User Extensions
*/
#include <BleKeyboard.h>
#include <BLESerial.h>

BleKeyboard bleKeyboard;
BLESerial SerialBT;

void SerialBT_begin() {
    SerialBT.begin("ESP32 Keyboard");
}
void BleKeyboard_begin(BleKeyboard *b) {
//    SerialBT_begin();
    b->begin();
}
void BleKeyboard_end(BleKeyboard *b) {
    b->end();
}
bool BleKeyboard_isConnected(BleKeyboard *b){
    return b->isConnected();
}
size_t BleKeyboard_press(BleKeyboard *b, int key){
    return b->press(key);
}

size_t BleKeyboard_release(BleKeyboard *b, int key) {
    return b->release(key);
}
void BLEDeviceClose(){
    BLEDevice::deinit();
}
int getMemChar(char *addr){
   return *addr;
}
std::string SerialBT_pserial_buff;
void SerialBT_pserial (char c) {
    if (c == ' ' || c == ':' || c == '(') {
        SerialBT_pserial_send();
    }
    SerialBT_pserial_buff.append(1, c);
//    LastPrint = c;
//    if (c == '\n') SerialBT.write('\r');
//    SerialBT.write(c);
}
void SerialBT_pserial_send(){
    if (SerialBT_pserial_buff.length() > 0){
        SerialBT.write((const uint8_t *)SerialBT_pserial_buff.c_str(), SerialBT_pserial_buff.length());
        SerialBT_pserial_buff.clear();
    }
}
void markUse0(){
    //markUse
    SerialBT.connected();
    SerialBT.available();
    SerialBT.read();
    SerialBT.write(1);
    SerialBT.write(0, 1);
    SerialBT_pserial(0);
}
void markUse(){
    printf("(defvar %s %d)\n", "BleKeyboard_begin", &BleKeyboard_begin);
    printf("(defvar %s %d)\n", "BleKeyboard_end", &BleKeyboard_end);
    printf("(defvar %s %d)\n", "BleKeyboard_isConnected", &BleKeyboard_isConnected);
    printf("(defvar %s %d)\n", "BleKeyboard_press", &BleKeyboard_press);
    printf("(defvar %s %d)\n", "BleKeyboard_release", &BleKeyboard_release);
    printf("(defvar %s %d)\n", "SerialBT_begin", &SerialBT_begin);
    printf("(defvar %s %d)\n", "BLEDeviceClose", &BLEDeviceClose);
    printf("(defvar %s %d)\n", "getMemChar", &getMemChar);
    printf("(defvar %s %d)\n", "SerialBT", &SerialBT);
    printf("(defvar %s %d)\n", "bleKeyboard", &bleKeyboard);
    printf("(defvar %s %d)\n", "memset", &memset);
    printf("(defvar %s %d)\n", "malloc", &malloc);
    printf("(defvar %s %d)\n", "free", &free);
    printf("(defvar %s %d)\n", "SerialBT_pserial", &SerialBT_pserial);
    printf("(defvar %s %d)\n", "SerialBT_pserial_send", &SerialBT_pserial_send);

    printf("%d\n", &markUse0);
//         printf("%p \n", &SerialBT.end);
//         printf("%p \n", &SerialBT.available);
//         printf("%p \n", &SerialBT.read);
//         printf("%p\n", &SerialBT.write);
}

// Definitions
object *fn_now (object *args, object *env) {
  (void) env;
  static unsigned long Offset;
  unsigned long now = millis()/1000;
  int nargs = listlength(args);

  // Set time
  if (nargs == 3) {
    Offset = (unsigned long)((checkinteger(first(args))*60 + checkinteger(second(args)))*60
      + checkinteger(third(args)) - now);
  } else if (nargs > 0) error2(PSTR("wrong number of arguments"));

  // Return time
  unsigned long secs = Offset + now;
  object *seconds = number(secs%60);
  object *minutes = number((secs/60)%60);
  object *hours = number((secs/3600)%24);
  return cons(hours, cons(minutes, cons(seconds, NULL)));
}

// Symbol names
const char stringnow[] PROGMEM = "now";

// Documentation strings
const char docnow[] PROGMEM = "(now [hh mm ss])\n"
"Sets the current time, or with no arguments returns the current time\n"
"as a list of three integers (hh mm ss).";

// Symbol lookup table
const tbl_entry_t lookup_table2[] PROGMEM = {
  { stringnow, fn_now, 0203, docnow },
};

// Table cross-reference functions

tbl_entry_t *tables[] = {lookup_table, lookup_table2};
const unsigned int tablesizes[] = { arraysize(lookup_table), arraysize(lookup_table2) };

const tbl_entry_t *table (int n) {
  return tables[n];
}

unsigned int tablesize (int n) {
  return tablesizes[n];
}
