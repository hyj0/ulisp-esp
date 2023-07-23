/*
 User Extensions
*/

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

//extern object *gEvalEnv = NULL; //eval env

int test_cfun(int n, char c) {
    printf("call %s %d %d\n", __FUNCTION__ , n, c);
    return 100;
}
void test_cfun1( char c, float f) {
    printf("call %s %d %f\n", __FUNCTION__ , c, f);
}

class TestClass {
public:
    TestClass(){
        printf("%s\n", __FUNCTION__ );
    }
    int foo(int n, char c) {
        printf("%s:%d %d %d\n", __FUNCTION__ , n, c, n+c);
        return n;
    }
    void testString(char *str) {
        printf("%s:%s\n", __FUNCTION__ , str);
        str[0]='h';
        str[1]='e';
    }
};

TestClass testClassInstance;

void *__get_value(object *o){
    if (keywordp(o)) {
        return reinterpret_cast<void *>(checkkeyword(o));
    } else if (integerp(o)) {
        return reinterpret_cast<void*>(checkinteger(o));
    } else if (characterp(o)) {
        return reinterpret_cast<void *>(checkchar(o));
    } else {
        /*
         * floatp(o)
         */
        printf("%s err type=%d", __FUNCTION__ , o->type);
        error("type not support type", o);
        return NULL;
    }
    return NULL;
}

int call_c_fun_debug = 1;

void *get_value(object *o){
    void *ret = __get_value(o);
    if (call_c_fun_debug){
        printf("%s value=%d\n", __FUNCTION__, ret);
    }
    return ret;
}

object *my_nth(object *o, int idx){
    object *ret = o;
    for (int i = 0; i < idx; ++i) {
        ret = cdr(ret);
    }
    return car(ret);
}

uint32_t (*pFun0)();
uint32_t (*pFun1)(void *);
uint32_t (*pFun2)(void *, void *);
uint32_t (*pFun3)(void *, void *, void *);
uint32_t (*pFun4)(void *, void *, void *, void *);
uint32_t (*pFun5)(void *, void *, void *, void *, void *);

#define cf_arg1 get_value(first(fun_args))
#define cf_arg2 cf_arg1,get_value(second(fun_args))
#define cf_arg3 cf_arg2,get_value(third(fun_args))
#define cf_arg4 cf_arg3,get_value(my_nth(fun_args, 3))
#define cf_arg5 cf_arg4,get_value(my_nth(fun_args, 4))
/*
 * call c function
 * (call-c-fun address [arg0 arg1 arg2])
 */
object *fn_call_c_fun (object *args, object *env) {
    (void) env; void *pAddr;

    object *arg = first(args);
    if (checkinteger(arg)) {
        pAddr = reinterpret_cast<void *>(checkinteger(arg));
    } else {
        return nil;
    }
    uint32_t ret;
    object *fun_args = cdr(args);
    int arg_len = listlength(cdr(args));
    if (call_c_fun_debug){
        printf("addr=%d arg_len=%d\n", pAddr, arg_len);
    }
    switch (arg_len) {
        case 0: {
            pFun0 = reinterpret_cast<uint32_t (*)()>(pAddr);
            ret = pFun0();
            return number(ret);
        }
        case 1: {
            pFun1 = reinterpret_cast<uint32_t (*)(void *)>(pAddr);
            ret = pFun1(cf_arg1);
            return number(ret);
            break;
        }
        case 2: {
            pFun2 = reinterpret_cast<uint32_t (*)(void *, void *)>(pAddr);
            ret = pFun2(cf_arg2);
            return number(ret);
            break;
        }
        case 3: {
            pFun3 = reinterpret_cast<uint32_t (*)(void *, void *, void *)>(pAddr);
            ret = pFun3(cf_arg3);
            return number(ret);
            break;
        }
        case 4: {
            pFun4 = reinterpret_cast<uint32_t (*)(void *, void *, void *, void *)>(pAddr);
            ret = pFun4(cf_arg4);
            return number(ret);
        }
        case 5: {
            pFun5 = reinterpret_cast<uint32_t (*)(void *, void *, void *, void *, void *)>(pAddr);
            ret = pFun5(cf_arg5);
            return number(ret);
        }
        default:
            error(toomanyargs, args);
            return nil;
    }
    return nil;
}

#define cf2_arg1 (first(signArgs)==nil? get_value(first(fun_args)):first(fun_args))
#define cf2_arg2 cf2_arg1, (second(signArgs)==nil? get_value(second(fun_args)):second(fun_args))
#define cf2_arg3 cf2_arg2,(third(signArgs)==nil? get_value(third(fun_args)):third(fun_args))
#define cf2_arg4 cf2_arg3,(my_nth(signArgs, 3)==nil? get_value(my_nth(fun_args, 3)):my_nth(fun_args, 3))
#define cf2_arg5 cf2_arg4,(my_nth(signArgs, 4)==nil? get_value(my_nth(fun_args, 4)):my_nth(fun_args, 4))
/*
 * call c function
 * (call-c-fun2 '(t (t nil nil...)) address arg0 arg1 ...)
 */
object *fn_call_c_fun2 (object *args, object *env) {
    (void) env; void *pAddr;

    //sign
    object *sign = first(args);
    if (!listp(sign) or listlength(sign) != 2) {
        error("arg not list 2 item", sign);
        return nil;
    }
    bool retTypeIsObjectFlag = (first(sign) != nil);
    object *signArgs = second(sign);
    int signArgsLen = listlength(second(sign));

    //addr
    object *arg = second(args);
    if (checkinteger(arg)) {
        pAddr = reinterpret_cast<void *>(checkinteger(arg));
    } else {
        return nil;
    }

    //args
    uint32_t ret;
    object *fun_args = cdr(cdr(args));
    int arg_len = listlength(fun_args);
    if (arg_len != signArgsLen) {
        error("args not match", cons(signArgs, cons(fun_args, nil)));
        return nil;
    }
    if (call_c_fun_debug){
        printf("addr=%d arg_len=%d\n", pAddr, arg_len);
    }
    switch (arg_len) {
        case 0: {
            pFun0 = reinterpret_cast<uint32_t (*)()>(pAddr);
            ret = pFun0();
            return retTypeIsObjectFlag? (object *)ret : number(ret);
        }
        case 1: {
            pFun1 = reinterpret_cast<uint32_t (*)(void *)>(pAddr);
            ret = pFun1(cf2_arg1);
            return retTypeIsObjectFlag? (object *)ret : number(ret);
            break;
        }
        case 2: {
            pFun2 = reinterpret_cast<uint32_t (*)(void *, void *)>(pAddr);
            ret = pFun2(cf2_arg2);
            return retTypeIsObjectFlag? (object *)ret : number(ret);
            break;
        }
        case 3: {
            pFun3 = reinterpret_cast<uint32_t (*)(void *, void *, void *)>(pAddr);
            ret = pFun3(cf2_arg3);
            return retTypeIsObjectFlag? (object *)ret : number(ret);
            break;
        }
        case 4: {
            pFun4 = reinterpret_cast<uint32_t (*)(void *, void *, void *, void *)>(pAddr);
            ret = pFun4(cf2_arg4);
            return retTypeIsObjectFlag? (object *)ret : number(ret);
        }
        case 5: {
            pFun5 = reinterpret_cast<uint32_t (*)(void *, void *, void *, void *, void *)>(pAddr);
            ret = pFun5(cf2_arg5);
            return retTypeIsObjectFlag? (object *)ret : number(ret);
        }
        default:
            error(toomanyargs, args);
            return nil;
    }
    return nil;
}

int testVaFun(int count, ...){
    va_list args;
    va_start(args, count);

    int result = 0;
    for (int i = 0; i < count; i++) {
        int n = va_arg(args, int);
        result += n;
        printf("%s %d\n", __FUNCTION__ , n);
    }
    va_end(args);
    return result;
}

#define va_arg1 object* argx0 = first(signArgs)==nil? number((long)arg0):(object*)arg0
#define va_arg2 va_arg1;object* argx1 = second(signArgs)==nil? number((long)va_arg(args, void *)):(object*)va_arg(args, void *)
#define va_arg3 va_arg2;object* argx2 = third(signArgs)==nil? number((long)va_arg(args, void *)):(object*)va_arg(args, void *)
#define va_arg4 va_arg3;object* argx3 = my_nth(signArgs,3)==nil? number((long)va_arg(args, void *)):(object*)va_arg(args, void *)
#define va_arg5 va_arg4;object* argx4 = my_nth(signArgs,4)==nil? number((long)va_arg(args, void *)):(object*)va_arg(args, void *)


#define cf3_arg1(tail) cons(argx0, (tail))
#define cf3_arg2(tail) cf3_arg1(cons(argx1, (tail)))
#define cf3_arg3(tail) cf3_arg2(cons(argx2, (tail)))
#define cf3_arg4(tail) cf3_arg3(cons(argx3, (tail)))
#define cf3_arg5(tail) cf3_arg4(cons(argx4, (tail)))

#define PRINT_STR_OBJECT(str, obj)  if (call_c_fun_debug){ pstring((char*)str, pserial);\
                                    printobject(obj, pserial); \
                                    pstring((char*)"\n", pserial);}
object *getHookFunData0(const char *__cfunName){
    char *cfunName = (char *)__cfunName;
    object *s = internlong((char *)"g_hook_fun");
    object *v = findvalue(s, GlobalEnv);
    object *ret = assoc(internlong(cfunName), cdr(v));
    if (ret == nil){
        error("not found item", cons(internlong(cfunName), cdr(v)));
        return NULL;
    }
    PRINT_STR_OBJECT("getHookFunData:", ret);
    return ret;
}

void *__hookFun00(){
    object *ret = getHookFunData0(__FUNCTION__);

    //ret=addr, sign, fun
    object *sign = second(ret);
    bool retTypeIsObjectFlag = (first(sign) != nil);
    object *lret = apply(third(ret), nil, gEvalEnv);
    return retTypeIsObjectFlag?lret:get_value(lret);
}
void *__hookFun01(){
    object *ret = getHookFunData0(__FUNCTION__);

    //ret=addr, sign, fun
    object *sign = second(ret);
    bool retTypeIsObjectFlag = (first(sign) != nil);
    object *lret = apply(third(ret), nil, gEvalEnv);
    return retTypeIsObjectFlag?lret:get_value(lret);
}
void *__hookFun02(){
    object *ret = getHookFunData0(__FUNCTION__);

    //ret=addr, sign, fun
    object *sign = second(ret);
    bool retTypeIsObjectFlag = (first(sign) != nil);
    object *lret = apply(third(ret), nil, gEvalEnv);
    return retTypeIsObjectFlag?lret:get_value(lret);
}

void *__hookFunN0(void *arg0, ...){
    object *ret = getHookFunData0(__FUNCTION__);

    //ret=addr, sign, fun
    object *sign = second(ret);
    bool retTypeIsObjectFlag = (first(sign) != nil);
    object *signArgs = second(sign);
    int signArgsLen = listlength(second(sign));
    va_list args;
    va_start(args, arg0);
    switch (signArgsLen) {
        case 1:{
            va_arg1;
            object *lispArgs = cf3_arg1(nil);
            PRINT_STR_OBJECT("lispArgs:", lispArgs);

            object *lret = apply(third(ret), lispArgs, gEvalEnv);
            return retTypeIsObjectFlag?lret:get_value(lret);
            break;
        }
        case 2:{
            va_arg2;
            object *lispArgs = cf3_arg2(nil);
            PRINT_STR_OBJECT("lispArgs:", lispArgs);

            object *lret = apply(third(ret), lispArgs, gEvalEnv);
            return retTypeIsObjectFlag?lret:get_value(lret);
            break;
        }
        case 3:{
            va_arg3;
            object *lispArgs = cf3_arg3(nil);
            PRINT_STR_OBJECT("lispArgs:", lispArgs);

            object *lret = apply(third(ret), lispArgs, gEvalEnv);
            return retTypeIsObjectFlag?lret:get_value(lret);
            break;
        }
        case 4: {
            va_arg4;
            object *lispArgs = cf3_arg4(nil);
            PRINT_STR_OBJECT("lispArgs:", lispArgs);

            object *lret = apply(third(ret), lispArgs, gEvalEnv);
            return retTypeIsObjectFlag?lret:get_value(lret);
            break;
        }
        case 5: {
            va_arg5;
            object *lispArgs = cf3_arg5(nil);
            PRINT_STR_OBJECT("lispArgs:", lispArgs);

            object *lret = apply(third(ret), lispArgs, gEvalEnv);
            return retTypeIsObjectFlag?lret:get_value(lret);
            break;
        }
        default:
            error("err argsLen", number(signArgsLen));
            return number(0);
    }
    return number(0);
}
void *__hookFunN1(void *arg0, ...){
    object *ret = getHookFunData0(__FUNCTION__);

    //ret=addr, sign, fun
    object *sign = second(ret);
    bool retTypeIsObjectFlag = (first(sign) != nil);
    object *signArgs = second(sign);
    int signArgsLen = listlength(second(sign));
    va_list args;
    va_start(args, arg0);
    switch (signArgsLen) {
        case 1:{
            va_arg1;
            object *lispArgs = cf3_arg1(nil);
            PRINT_STR_OBJECT("lispArgs:", lispArgs);

            object *lret = apply(third(ret), lispArgs, gEvalEnv);
            return retTypeIsObjectFlag?lret:get_value(lret);
            break;
        }
        case 2:{
            va_arg2;
            object *lispArgs = cf3_arg2(nil);
            PRINT_STR_OBJECT("lispArgs:", lispArgs);

            object *lret = apply(third(ret), lispArgs, gEvalEnv);
            return retTypeIsObjectFlag?lret:get_value(lret);
            break;
        }
        case 3:{
            va_arg3;
            object *lispArgs = cf3_arg3(nil);
            PRINT_STR_OBJECT("lispArgs:", lispArgs);

            object *lret = apply(third(ret), lispArgs, gEvalEnv);
            return retTypeIsObjectFlag?lret:get_value(lret);
            break;
        }
        case 4: {
            va_arg4;
            object *lispArgs = cf3_arg4(nil);
            PRINT_STR_OBJECT("lispArgs:", lispArgs);

            object *lret = apply(third(ret), lispArgs, gEvalEnv);
            return retTypeIsObjectFlag?lret:get_value(lret);
            break;
        }
        case 5: {
            va_arg5;
            object *lispArgs = cf3_arg5(nil);
            PRINT_STR_OBJECT("lispArgs:", lispArgs);

            object *lret = apply(third(ret), lispArgs, gEvalEnv);
            return retTypeIsObjectFlag?lret:get_value(lret);
            break;
        }
        default:
            error("err argsLen", number(signArgsLen));
            return number(0);
    }
    return number(0);
}
void *__hookFunN2(void *arg0, ...){
    object *ret = getHookFunData0(__FUNCTION__);

    //ret=addr, sign, fun
    object *sign = second(ret);
    bool retTypeIsObjectFlag = (first(sign) != nil);
    object *signArgs = second(sign);
    int signArgsLen = listlength(second(sign));
    va_list args;
    va_start(args, arg0);
    switch (signArgsLen) {
        case 1:{
            va_arg1;
            object *lispArgs = cf3_arg1(nil);
            PRINT_STR_OBJECT("lispArgs:", lispArgs);

            object *lret = apply(third(ret), lispArgs, gEvalEnv);
            return retTypeIsObjectFlag?lret:get_value(lret);
            break;
        }
        case 2:{
            va_arg2;
            object *lispArgs = cf3_arg2(nil);
            PRINT_STR_OBJECT("lispArgs:", lispArgs);

            object *lret = apply(third(ret), lispArgs, gEvalEnv);
            return retTypeIsObjectFlag?lret:get_value(lret);
            break;
        }
        case 3:{
            va_arg3;
            object *lispArgs = cf3_arg3(nil);
            PRINT_STR_OBJECT("lispArgs:", lispArgs);

            object *lret = apply(third(ret), lispArgs, gEvalEnv);
            return retTypeIsObjectFlag?lret:get_value(lret);
            break;
        }
        case 4: {
            va_arg4;
            object *lispArgs = cf3_arg4(nil);
            PRINT_STR_OBJECT("lispArgs:", lispArgs);

            object *lret = apply(third(ret), lispArgs, gEvalEnv);
            return retTypeIsObjectFlag?lret:get_value(lret);
            break;
        }
        case 5: {
            va_arg5;
            object *lispArgs = cf3_arg5(nil);
            PRINT_STR_OBJECT("lispArgs:", lispArgs);

            object *lret = apply(third(ret), lispArgs, gEvalEnv);
            return retTypeIsObjectFlag?lret:get_value(lret);
            break;
        }
        default:
            error("err argsLen", number(signArgsLen));
            return number(0);
    }
    return number(0);
}

typedef struct __HookFunData {
    int argsLen; //0, n
    void *addr;
    char funName[32];
}HookFunData;
HookFunData  g_hookFunData[]={
        {0, (void *)&__hookFun00, "__hookFun00"},
        {0, (void *)&__hookFun01, "__hookFun01"},
        {0, (void *)&__hookFun02, "__hookFun02"},
        {1, (void *)&__hookFunN0, "__hookFunN0"},
        {1, (void *)&__hookFunN1, "__hookFunN1"},
        {1, (void *)&__hookFunN2, "__hookFunN2"},
};

object *getAllHookFunList(){
    object *retList = nil;
    for (int i = 0; i < sizeof(g_hookFunData)/sizeof(HookFunData); ++i) {
        retList = cons(
                cons(internlong(g_hookFunData[i].funName),
                     cons(number((int)g_hookFunData[i].addr), cons(number(g_hookFunData[i].argsLen), nil))), retList);
    }
    return retList;
}
const char doc_call_c_fun[] PROGMEM = "(call-c-fun address arg0 arg1 ...)\n"
                              "call a c function at address with args";
const char doc_call_c_fun2[] PROGMEM = "(call-c-fun2 (t (t nil nil...)) address arg0 arg1 ...)\n"
                               "call a c function at address with args";
// Symbol lookup table
const tbl_entry_t lookup_table2[] PROGMEM = {
  { stringnow, fn_now, 0203, docnow },
  { "call-c-fun", fn_call_c_fun, 0x8F, doc_call_c_fun }, /* fn 0 7 */
  { "call-c-fun2", fn_call_c_fun2, 0x8F, doc_call_c_fun2 }, /* fn 0 7 */
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

//only to mark symbol be used, do not call
int markUse() {
    printf("(defvar getAllHookFunList %d)\n", &getAllHookFunList);
    printf("(defvar testClassInstance %d)\n", &testClassInstance);
    printf("(defvar test_cfun %d)\n", &test_cfun);
    if (1) {
        testClassInstance.foo(11, 0);
        testClassInstance.testString(0);
    }
}