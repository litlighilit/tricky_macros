// 2022/12/20T22:45 litlighilit

#ifndef C_PRINT_H
#define C_PRINT_H

#define EMPTY
#define DEFER(id) id EMPTY

#define PRIMITIVE_CAT(x, y) x ## y
#define CAT(x, y) PRIMITIVE_CAT(x, y)

#define EXISTS(...) EXISTS_ ## __VA_OPT__(1)
#define EXISTS_1 1
#define EXISTS_  0


#define FOR_EACH(op, x, ...) CAT(FOR_EACH_, EXISTS(__VA_ARGS__)) (op, x __VA_OPT__(,) __VA_ARGS__)
#define FOR_EACH_1(op, x, ...) op(x) DEFER(FOR_EACH_I)() (op, __VA_ARGS__)
#define FOR_EACH_0(op,x) op(x) END_OP
#define FOR_EACH_I() FOR_EACH


#define EVAL(...)  EVAL1(EVAL1(EVAL1(__VA_ARGS__)))
#define EVAL1(...) EVAL2(EVAL2(EVAL2(__VA_ARGS__)))
#define EVAL2(...) __VA_ARGS__
// use the way above, it's at most 14 arguments within a call, 
// However, in general, when define util EVAL`n`, it'll become `(3^(n+1)+1)/2`( a.k.a. `(pow(3,(n+1)+1))/2` )
// e.g. write EVAL and EVAL1-6(EVAL5 -> __VA_ARGS__), then it'll touch 1094 (for more details, see README.md)
//  Do you have a enough stack size if it's in one normal function call?
// All right, if it's now not enough, feel free to add more (e.g.EVAL3 EVAL4 ... and keep the last's value is `__VA_ARGS__`)
// emm...remember the power of power in math

#define OP(x) PRINT1(x)
#define END_OP fprintf(FILE_, END);

#ifndef __NO_STDIO_H__ //for custom-implemented `fprintf`
#include <stdio.h>
#endif

#define SEP " "
#define END "\n"
#define FLUSH 0
#define FILE_ stdout
// use `FILE_` for not confusing with <stdio.h>'s `FILE` type

#if FLUSH != 0
# define MAY_FLUSH fflush(FILE_);
#else
# define MAY_FLUSH
#endif

#define _FMT(x) "%" #x SEP
#define PRINT1(x) \
  fprintf(FILE_, _Generic(x \
    ,char*:_FMT(s) \
    ,char:_FMT(c)  /*note: 'x' literat is not recongized as char but int(due to C-compile), use char-variable or type-converting to show a char*/\
    ,short:_FMT(h) \
    ,int:_FMT(i)   \
    ,long:_FMT(l)  \
    ,long long:_FMT(ll)  \
    ,float:_FMT(f)  \
    ,double:_FMT(lf)\
    ,long double:_FMT(Lf) \
    ,unsigned short:_FMT(hu) \
    ,unsigned int:_FMT(iu)   \
    ,unsigned long:_FMT(lu)  \
    ,unsigned long long:_FMT(llu)  \
    ,default:_FMT(p) \
  ),x);// note we assume the else ones are pointer, which may be not suitable all time, in this case you can add the right one

#define print(...) do{\
  EVAL(FOR_EACH(OP, __VA_ARGS__));
  MAY_FLUSH \
} while(0)

#endif // C_PRINT_H
