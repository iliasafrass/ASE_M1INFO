#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <assert.h>
#include "test.h"
struct ctx_s * current_ctx =(struct ctx_s *) 0;

struct ctx_s {
		void* ctx_esp;
		void* ctx_ebp;
		unsigned ctx;
		func_t * f;
		void * arg;
		enum state_e status;
		unsigned char* stack;
  	struct ctx_s * next;
};

int init_ctx(struct ctx_s * pctx, int stack_size, func_t f, void* args){
		pctx ->stack = (char*)malloc(stack_size);
		if(! pctx -> stack)
		return 1;
		pctx->ctx_ebp= &(pctx->stack[stack_size-4]);
		pctx->ctx_esp= pctx->ctx_ebp;
		pctx->f = f;
		pctx->arg = args;
		pctx->status = CTX_READY;
		pctx->ctx = CTX_MAGIC;
		pctx->next = pctx;
		return 0;
	}

struct ctx_s * create_ctx(int stack_size, func_t f, void* args){
  	struct ctx_s * ctx =(struct ctx_s*) malloc(sizeof(struct ctx_s));
  	if( ctx == NULL){
      	return NULL;
    }
    if(!init_ctx(ctx,stack_size,f, args))
  		return NULL;
    struct ctx_s * last_ctx=current_ctx;
    if(last_ctx == NULL){
	      ctx->next=ctx;
	      current_ctx=ctx;
	      return ctx;
	  }
    while(last_ctx->next!=current_ctx){
      	last_ctx=last_ctx->next;
    }
    last_ctx->next=ctx;
    ctx->next=current_ctx;
    return ctx;
  }

void start_ctx(){
	   struct ctx_s * ctx=current_ctx;
	   ctx->status=CTX_ACTIVATED;
	   ctx->f(ctx->arg);
	   ctx->status=CTX_TERMINATED;
	 }

 void switch_to_ctx(struct ctx_s * newctx){
	  static struct ctx_s * ctx_current =NULL;
	  assert((newctx->ctx == CTX_MAGIC) || (newctx->status!=CTX_TERMINATED));
	  if(ctx_current){
	  		asm("movl %% esp, %0":"=r"(ctx_current->ctx_esp)::);
	  		asm("movl %% ebp, %0":"=r"(ctx_current->ctx_ebp)::);
		}
	  ctx_current= newctx;
	  asm("movl %0 , %%esp"::"r"(ctx_current->ctx_esp):);
	  asm("movl %0 , %%ebp"::"r"(ctx_current->ctx_ebp):);
	  if(ctx_current-> status==CTX_READY){
	  		start_ctx();
	  }
	  return;
	 }


void yield() {
	   switch_to_ctx(current_ctx->next);
	 }

struct ctx_s ctx_ping;
struct ctx_s ctx_pong;

void f_ping(void *args){
	 	while(1) {
		 	printf("A") ;
		 	switch_to_ctx(&ctx_pong);
		 	printf("B") ;
		 	switch_to_ctx(&ctx_pong);
		 	printf("C") ;
		 	switch_to_ctx(&ctx_pong);
	 	}
}

void f_pong(void *args){
	 	while(1) {
	 		printf("1") ;
	 		switch_to_ctx(&ctx_ping);
	 		printf("2") ;
	 		switch_to_ctx(&ctx_ping);
	 	}
}

int main(int argc, char*argv[]){
	create_ctx(16384, f_ping, NULL);
	create_ctx(16384, f_pong, NULL);
	yield();
	printf("done\n");
	exit(EXIT_SUCCESS);
}
