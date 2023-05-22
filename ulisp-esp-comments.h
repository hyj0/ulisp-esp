
typedef long int		intptr_t;

#define ARDUINO_ESP32C3_DEV

#define PROGMEM
#define PGM_P const char *
typedef unsigned int		uint32_t;

typedef unsigned short int	uint16_t;
typedef unsigned char		uint8_t;
typedef  char		int8_t;

#define NULL 0L
typedef unsigned long int	uintptr_t;

#define PSTR



// C Macros

#define nil                NULL
#define car(x)             (((object *) (x))->car)
#define cdr(x)             (((object *) (x))->cdr)

#define first(x)           (((object *) (x))->car)
#define second(x)          (car(cdr(x)))
#define cddr(x)            (cdr(cdr(x)))
#define third(x)           (car(cdr(cdr(x))))

#define push(x, y)         ((y) = cons((x),(y)))
#define pop(y)             ((y) = cdr(y))

#define integerp(x)        ((x) != NULL && (x)->type == NUMBER)
#define floatp(x)          ((x) != NULL && (x)->type == FLOAT)
#define symbolp(x)         ((x) != NULL && (x)->type == SYMBOL)
#define stringp(x)         ((x) != NULL && (x)->type == STRING)
#define characterp(x)      ((x) != NULL && (x)->type == CHARACTER)
#define arrayp(x)          ((x) != NULL && (x)->type == ARRAY)
#define streamp(x)         ((x) != NULL && (x)->type == STREAM)

#define mark(x)            (car(x) = (object *)(((uintptr_t)(car(x))) | MARKBIT))
#define unmark(x)          (car(x) = (object *)(((uintptr_t)(car(x))) & ~MARKBIT))
#define marked(x)          ((((uintptr_t)(car(x))) & MARKBIT) != 0)
#define MARKBIT            1

#define setflag(x)         (Flags = Flags | 1<<(x))
#define clrflag(x)         (Flags = Flags & ~(1<<(x)))
#define tstflag(x)         (Flags & 1<<(x))

#define issp(x)            (x == ' ' || x == '\n' || x == '\r' || x == '\t')
#define isbr(x)            (x == ')' || x == '(' || x == '"' || x == '#')
#define longsymbolp(x)     (((x)->name & 0x03) == 0)
#define twist(x)           ((uint32_t)((x)<<2) | (((x) & 0xC0000000)>>30))
#define untwist(x)         (((x)>>2 & 0x3FFFFFFF) | ((x) & 0x03)<<30)
#define arraysize(x)       (sizeof(x) / sizeof(x[0]))
#define PACKEDS            0x43238000
#define BUILTINS           0xF4240000
#define ENDFUNCTIONS       1536

// Constants

const int TRACEMAX = 3; // Number of traced functions
enum type { ZZERO=0, SYMBOL=2, CODE=4, NUMBER=6, STREAM=8, CHARACTER=10, FLOAT=12, ARRAY=14, STRING=16, PAIR=18 };  // ARRAY STRING and PAIR must be last
enum token { UNUSED, BRA, KET, QUO, DOT };
enum stream { SERIALSTREAM, I2CSTREAM, SPISTREAM, SDSTREAM, WIFISTREAM, STRINGSTREAM, GFXSTREAM };
enum fntypes_t { OTHER_FORMS, TAIL_FORMS, FUNCTIONS, SPECIAL_FORMS };

// Stream names used by printobject
const char serialstream[] PROGMEM = "serial";
const char i2cstream[] PROGMEM = "i2c";
const char spistream[] PROGMEM = "spi";
const char sdstream[] PROGMEM = "sd";
const char wifistream[] PROGMEM = "wifi";
const char stringstream[] PROGMEM = "string";
const char gfxstream[] PROGMEM = "gfx";
PGM_P const streamname[] PROGMEM = {serialstream, i2cstream, spistream, sdstream, wifistream, stringstream, gfxstream};

// Typedefs

typedef uint32_t symbol_t;

typedef struct sobject {
  union {
    struct {
      sobject *car;
      sobject *cdr;
    };
    struct {
      unsigned int type;
      union {
        symbol_t name;
        int integer;
        int chars; // For strings
        float single_float;
      };
    };
  };
} object;


typedef void (*mapfun_t)(object *, object **);

    typedef object *(*fn_ptr_type)(object *, object *);
    typedef const struct {
        PGM_P string;
        fn_ptr_type fptr;
        uint8_t minmax;
        const char *doc;
    } tbl_entry_t;

typedef int (*gfun_t)();
typedef void (*pfun_t)(char);

typedef uint16_t builtin_t;

enum builtins: builtin_t { NIL, TEE, NOTHING, OPTIONAL, INITIALELEMENT, ELEMENTTYPE, BIT, AMPREST, LAMBDA, LET, LETSTAR,
CLOSURE, PSTAR, QUOTE, DEFUN, DEFVAR, CAR, FIRST, CDR, REST, NTH, AREF, STRINGFN, PINMODE, DIGITALWRITE,
ANALOGREAD, REGISTER, FORMAT,
 };


extern void pserial (char c);
void pfl (pfun_t pfun);

/*
 sym - converts a builtin to a symbol name
*/
extern symbol_t sym (builtin_t x);
extern void psymbol (symbol_t name, pfun_t pfun);
extern void printobject (object *form, pfun_t pfun);
extern inline void pln (pfun_t pfun);
extern void indent (uint8_t spaces, char ch, pfun_t pfun);
extern void printstring (object *form, pfun_t pfun);
extern object *intern (symbol_t name);
extern object *lispstring (char *s);
extern uint8_t nthchar (object *string, int n);
extern bool keywordp (object *obj);
extern uint8_t getminmax (builtin_t name);
extern intptr_t lookupfn (builtin_t name);
extern int listlength (object *list);
extern void checkminmax (builtin_t name, int nargs);
typedef bool boolean ;
#define HIGH 1
#define LOW 0

extern void delay(int ms);

class SerialCL {
public:
    SerialCL(){}

    void begin(int i) {

    }

    void write(int i) {

    }
};