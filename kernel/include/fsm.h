/*
 * It's a part of smartfsm, by Shanjin Yang.
 * Email: sjyangv0@qq.com
 *
 */

#define ERROR_PRINTF  printf("error: %d\n", __LINE__)

#define STATE_OBJ_MALLOC_AND_INSERT(token, state, func, state_name)  \
	STATE_DIS *state_obj_##token = (STATE_DIS *)malloc(sizeof(STATE_DIS)); \
state_obj_##token->state = state; \
state_obj_##token->name  = state_name; \
state_obj_##token->func  = func;\
list_insert( &(state_obj->node), &(state_obj_##token->node)) 

#define SET_STATE_DEFAULT_FUNC(function)  fsm_default.func = function
#define MSG_LEN 1024

typedef  void (*FUNC_PTR)(void *p_arg);

typedef struct fsm_s {
	int state;  
	FUNC_PTR func;
	U8 *name;
	U8 message[MSG_LEN];
} FSM;

typedef struct state_dis_s {
	LIST node;
	int state;
	FUNC_PTR func;
	U8 *name;
} STATE_DIS;

extern FSM *fsm_obj;
extern FSM fsm_default;

int state_init(int state, FUNC_PTR func, U8 *name);
int state_add(int state, FUNC_PTR func, U8 *name);
int state_remove(int state);
int state_tran(int state, void *message);
int fsm_init();
void fsm_while(FSM *obj);
int state_default(int state, FUNC_PTR func, U8 *name);
