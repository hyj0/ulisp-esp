
#include <cstdint>
#include <zconf.h>
#include <time.h>
#include <cstring>
#include <cstdio>
#include <poll.h>

typedef long int		intptr_t;

#define ARDUINO_ESP32C3_DEV

#define PROGMEM
#define PGM_P const char *
//typedef unsigned int		uint32_t;
//
//typedef unsigned short int	uint16_t;
//typedef unsigned char		uint8_t;
//typedef  char		int8_t;

//#define NULL 0L
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

typedef uint64_t symbol_t;

typedef struct sobject {
  union {
    struct {
      sobject *car;
      sobject *cdr;
    };
    struct {
      uint64_t type;
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


typedef long int64_t ;

//extern void pserial (char c);
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
extern bool stringcompare (object *args, bool lt, bool gt, bool eq);
extern object *eval (object *form, object *env);
extern void pint (int i, pfun_t pfun);
extern void pstr (char c);
typedef bool boolean ;
extern void printobject (object *form, pfun_t pfun);
extern object *findpair (object *var, object *env);
extern char *lookupdoc (builtin_t name);

extern object *fn_abs (object *args, object *env) ;
extern object *fn_acos (object *args, object *env) ;
extern object *fn_asin (object *args, object *env) ;
extern object *sp_withclient (object *args, object *env) ;
extern object *fn_cos (object *args, object *env) ;
extern object *fn_acos (object *args, object *env) ;
extern object *fn_cosh (object *args, object *env) ;
extern char *cstring (object *form, char *buffer, int buflen) ;
extern object *fn_eval (object *args, object *env) ;
extern object *eval (object *form, object *env) ;
extern object *fn_exp (object *args, object *env) ;
extern object *findpair (object *var, object *env) ;
extern bool findsubstring (char *part, builtin_t name) ;
extern object *fn_floor (object *args, object *env) ;
extern object *fn_princtostring (object *args, object *env) ;
extern int glibrary () ;
extern int gserial () ;
extern object *fn_log (object *args, object *env) ;
extern char *lookupdoc (builtin_t name) ;
extern object *sp_formillis (object *args, object *env) ;
extern object *fn_millis (object *args, object *env) ;
extern builtin_t lookupbuiltin (char* c) ;
extern void pbuiltin (builtin_t name, pfun_t pfun) ;
extern void pint (int i, pfun_t pfun) ;
extern void pintbase (uint32_t i, uint8_t base, pfun_t pfun) ;
extern void plispstr (symbol_t name, pfun_t pfun) ;
extern void prin1object (object *form, pfun_t pfun) ;
extern void printsymbol (object *form, pfun_t pfun) ;
extern void pstr (char c) ;
extern void plispstr (symbol_t name, pfun_t pfun) ;
extern object *fn_random (object *args, object *env) ;
extern int I2Cread () ;
extern inline int spiread () ;
extern inline int serial1read () ;
extern inline int SDread () ;
extern inline int WiFiread () ;
extern void checkanalogread (int pin) ;
extern object *fn_read (object *args, object *env) ;
extern object *fn_digitalread (object *args, object *env) ;
extern object *fn_analogread (object *args, object *env) ;
extern object *read (gfun_t gfun) ;
extern void repl (object *env) ;
extern int myround (float number) ;
extern object *fn_round (object *args, object *env) ;
extern object *fn_wifiserver (object *args, object *env) ;
extern object *fn_sin (object *args, object *env) ;
extern object *fn_asin (object *args, object *env) ;
extern object *fn_sinh (object *args, object *env) ;
extern object *fn_sqrt (object *args, object *env) ;
extern bool stringcompare (object *args, bool lt, bool gt, bool eq) ;
extern int subwidthlist (object *form, int w) ;
extern void supersub (object *form, int lm, int super, pfun_t pfun) ;
extern unsigned int tablesize (int n) ;
extern object *fn_tan (object *args, object *env) ;
extern object *fn_atan (object *args, object *env) ;
extern object *fn_tanh (object *args, object *env) ;
extern void testescape () ;
extern object *tf_progn (object *args, object *env) ;
extern void ulispreset () ;
extern object *fn_eval (object *args, object *env) ;
extern object *eval (object *form, object *env) ;
extern void pstring (char *s, pfun_t pfun);
extern char* strstr(char *a, char *b) {
    int len_a = strlen(a);
    int len_b = strlen(b);
    int i;

    for (i = 0; i <= len_a - len_b; i++) {
        if (strncmp(a + i, b, len_b) == 0) {
            return a + i;
        }
    }

    return NULL;
}

extern unsigned long millis() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long ms = ts.tv_nsec / 1000000;
    return ms;
}

extern unsigned long micros() {
    return millis();
}

extern void yield(){

}
//extern int abs(int n);
//extern float fmod(float fdividend,float fdivisor);
extern int random(int integer){

}
extern char *itoa(int n,char *strx,int i) {
    sprintf(strx, "%d", n);
    return strx;
}

#define pgm_read_ptr(addr) *(addr)
#define pgm_read_byte(addr) *((char *)addr)
#define strlen_P strlen
#define strcasecmp_P strcasecmp
#define HIGH 1
#define LOW 0

extern void delay(int ms) {
    usleep(1000*ms);
}

class SerialCL {
public:
    SerialCL(){}

    void begin(int i) {

    }

    void write(int i) {
        putchar(i);
        fsync(1);
    }

    void println(char *string) {
        printf(string);
    }

    int read() {
        if (available() == 0){
            return 0;
        }
        int c = getchar();
//        printf("%s %d %c\n", __FUNCTION__ , c, c);
        return c;
    }

    int available() {
        pollfd fds[1];
        fds[0].fd = 0;
        fds[0].events = POLLIN;
        fds[0].revents = 0;
        int ret = poll(fds, 1, 10);
        if (ret == 0) {
            return 0;
        } else if (ret == 1) {
            return 1;
        }
        return 0;
    }
};
