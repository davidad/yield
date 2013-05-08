#ifndef _YIELD_H
#define _YIELD_H
#include <ucontext.h>
#include <stdlib.h>
#define STACK_SIZE 0x10000

typedef struct {
  ucontext_t c_loop;
  ucontext_t c_main;
  void* data;
  char end_early;
} yield_t;

#define INVOKE(y,t,v,f,c,...) yield_t y;\
                              y.end_early=1;\
                              getcontext(&((y).c_loop));\
                              (y).c_loop.uc_stack.ss_sp=malloc(STACK_SIZE);\
                              (y).c_loop.uc_stack.ss_size=STACK_SIZE;\
                              (y).c_loop.uc_stack.ss_flags=0;\
                              (y).c_loop.uc_link = &((y).c_main);\
                              makecontext(&((y).c_loop), (void(*)())f, c, (int)(&(y)), ## __VA_ARGS__);\
                              swapcontext(&((y).c_main), &((y).c_loop));\
                              if((y).data && (y).end_early) { \
                                  t v = *((t*)((y).data));
                                  //Code goes here.
#define CLOSE(y)                  setcontext(&((y).c_loop));\
                              }
#define END_EARLY(y)  (y).end_early=0;\
                      setcontext(&((y).c_main));

//Coroutine uses this macro to swap back to the above.
#define YIELD(y,d) (y)->data=d;\
                   swapcontext(&((y)->c_loop), &((y)->c_main));
//Call this within the coroutine to leave it for good.
#define TERMINATE(y) YIELD(y,NULL)

#endif
