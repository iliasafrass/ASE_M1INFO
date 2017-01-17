
#define CTX_MAGIC 0xBABE

typedef void (func_t) (void*);
typedef enum state_e{CTX_READY, CTX_TERMINATED, CTX_ACTIVATED };


struct ctx_s * create_ctx(int size, func_t f, void * args);
void yield();



void f_ping(void *arg);
void f_pong(void *arg);
