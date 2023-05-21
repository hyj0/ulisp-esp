/*
 User Extensions
*/
#include <BleKeyboard.h>
BleKeyboard bleKeyboard;

void BleKeyboard_begin(BleKeyboard *b) {
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


void markUse(){
    printf("(defvar %s %d)\n", "BleKeyboard_begin", &BleKeyboard_begin);
    printf("(defvar %s %d)\n", "BleKeyboard_end", &BleKeyboard_end);
    printf("(defvar %s %d)\n", "BleKeyboard_isConnected", &BleKeyboard_isConnected);
    printf("(defvar %s %d)\n", "BleKeyboard_press", &BleKeyboard_press);
    printf("(defvar %s %d)\n", "BleKeyboard_release", &BleKeyboard_release);
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
