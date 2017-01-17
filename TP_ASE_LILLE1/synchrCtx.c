//3)changement de contexte:


#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#define CTX_MAGIC 0xBABE

typedef void(func_t)(void *);


//enumeration sur le status du contexte.
enum state_e
{
  CTX_READY,CTX_TERMINATED,CTX_ACTIVABLE
};

struct ctx_s
{
  int * esp,ebp;
  unsigned int magic;
  func_t *f;
  void * arg;
  enum state_e status;
  unsigned char* stack;
  struct ctx_s *next;
};

struct sem_s
{
  unsigned int val;
  struct ctx_s *locked;

}

struct ctx_s ctx_ping;
struct ctx_s ctx_pong;
struct ctx_s *ctx_current;
void f_ping();
void f_pong();

//initialisation du contexte.
int init_ctx(struct ctx_s *pctx,int stack_size,func_t f,void *arg)
{
  pctx->stack = malloc(stack_size);
  if(!pctx->stack)
    return 0;
  pctx -> ebp = (void*)&pctx->stack[stack_size-4];
  pctx -> esp = (int*)pctx->ebp;
  pctx -> f = f;
  pctx -> arg = arg;
  pctx -> status = CTX_READY;
  pctx -> magic = CTX_MAGIC;
  return 1;
}

void switch_to_ctx(struct ctx_s * newctx)
{
  *ctx_current = NULL;
  assert(newctx-> magic == CTX_MAGIC || newctx -> status != CTX_TERMINATED);

  if(ctx_current)
  {
    asm("movl %%esp,%0"
        :"=r"(ctx_current->esp)::);

    asm("movl %%ebp,%0"
        :"=r"(ctx_current->ebp)::);
  }
  ctx_current = newctx;
  asm(
    "movl %0,%%esp"
    ::"r"(ctx_current->esp):);//la variable local
    asm(
      "movl %0,%%ebp"
      ::"r"(ctx_current->ebp):
    );
  if(ctx_current->status == CTX_READY)
  {
    ctx_current->status == CTX_ACTIVABLE;
    ctx_current->f(ctx_current->arg);
    ctx_current->status = CTX_TERMINATED;
    exit(0);
  }
  return;
}

void Yield()
{
  switch_to_ctx(current_ctx->Next);
}

struct * ctx_s create_ctx(int stack_size, func_t f, void * args)
{
  struct *ctx_s ctx = malloc(sizeof(struct ctx_s));
  if(ctx == NULL || !(init_ctx(ctx,f,args)))
  {
    return NULL;
  }
  struct * ctx_s last_ctx = current_ctx;
  if(last_ctx == NULL)
  {
    ctx->next = ctx;
    current_ctx -> next = ctx;
    return ctx;
  }
  while(last_ctx->next != current_ctx)
  {
    last_ctx = last_ctx->next;
  }
  last_ctx->next = current_ctx;
  return ctx;
}

void start_ctx()
{
  struct ctx_s *ctx = current_ctx;
  ctx->status = ACTIVABLE;
  ctx->f(ctx->args);
  ctx->status = TERMINATED;
}

//fonction pour initialiser la sémaphore
void sem_init(struct sem_s *sem, unsigned int val)
{
  sem->val = val;
  sem->locked = NULL;
}

void sem_down(struct sem_s *sem)
{
  mask(15);
  sem->val--;
  if(sem->val < 0){
    struct ctx_s *ctx = current_ctx;
    while (ctx->next != current_ctx) {
      /* code */
      ctx = ctx->next;
    }
    ctx-> next = current_ctx -> next;
    current_ctx -> next = sem->locked;
    sem->locked = current_ctx;

  if(ctx == current_ctx){
    printf("Kernel panic : lock");
    exit(0);
  }
  switch_to(ctx->next);
    }
}

void sem_up(struct sem_s * sem)
{
    mask(15);
    sem->val++;
    if(sem->val <= 0){
      struct ctx_s * ctx = sem->locked;
          if(ctx != NULL){

            sem->locked = sem->next;
            ctx->next = current_ctx->next;
            current_ctx->next = ctx;
          }
      }
      mask(0);
}

void f_ping()
{
  while(1) {
    printf("A") ;
    switch_to_ctx(&ctx_pong);
    printf("B") ;
    switch_to_ctx(&ctx_pong);
    printf("C") ;
    switch_to_ctx(&ctx_pong);
  }
}

void f_pong()
{
  while(1) {
    printf("1") ;
    switch_to_ctx(&ctx_ping);
    printf("2") ;
    switch_to_ctx(&ctx_ping);
  }
}


int main()
{
  init_ctx(&ctx_ping, 16384, f_ping, NULL);
  init_ctx(&ctx_pong, 16384, f_pong, NULL);
  switch_to_ctx(&ctx_ping);
  exit(EXIT_SUCCESS);
}
