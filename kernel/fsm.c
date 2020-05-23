/*
 * It's a part of smartfsm, by Shanjin Yang.
 * Email: sjyangv0@qq.com
 *
 */

#include <var_define.h>

FSM *fsm_obj;
STATE_DIS *state_obj;
STATE_DIS *state_obj_copy;

FSM fsm_default = 
{
	0, NULL, "state_default",
};

int fsm_init()
{
	state_obj = (STATE_DIS *)malloc(sizeof(STATE_DIS));
	if (state_obj == NULL) {
		os_printf("malloc error\n");
		return 0;
	}

	fsm_obj = (FSM *)malloc(sizeof(FSM));
	if (fsm_obj == NULL) {
		os_printf("malloc error\n");
		return 0;
	}

	return 0;
}

int state_init(int state, FUNC_PTR func, U8 *name)
{
	fsm_init();

	if ((state_obj == NULL) || ( fsm_obj == NULL)) 
		ERROR_PRINTF;

	fsm_obj->state = state;
	fsm_obj->func = func;
	fsm_obj->name = name;

	state_obj->state = state;
	state_obj->func  = func;
	state_obj->name  = name;

	state_obj_copy = state_obj;
	list_init(&(state_obj->node));
	return 0;
}

int state_add(int state, FUNC_PTR func, U8 *name)
{
	/*Just make a symbol for state name*/
	int token = (int)*name;

	STATE_OBJ_MALLOC_AND_INSERT(token, state, func, name);

	state_obj = (STATE_DIS *)(&(state_obj->node))->next;
	return 0;
}

int state_remove(int state)
{
	STATE_DIS *tmp = state_obj_copy;
	while (!is_list_empty(&(tmp->node))) {
		if (tmp->state == state) {
			list_delete(&(tmp->node));
			tmp->state = 10000;
			tmp->func = NULL;
			tmp->name = "";
			return 0;
		}

		tmp = (STATE_DIS *)(&(tmp->node))->next;
	}

	if (tmp->state == state) {
		list_delete(&(tmp->node)); 

		tmp->state = 10000;
		tmp->func = NULL;
		tmp->name = "";

		return 0;
	}
	os_printf("State no fund\n");
	return 0;
}

int state_tran(int state, void *message)
{
	STATE_DIS *tmp = state_obj_copy;

	memset(fsm_obj->message, 0, MSG_LEN);
	while (!is_list_empty(&(tmp->node))) {
		if (tmp->state == state) {
			fsm_obj->func = tmp->func;
			fsm_obj->name = tmp->name; 
			fsm_obj->state = state;
			if (message != NULL)
				memcpy(fsm_obj->message, message, strlen((char *)message));

			return 0; 
		}

		tmp = (STATE_DIS *)(&(tmp->node))->next;
	}

	if (tmp->state == state) {
		fsm_obj->name = tmp->name; 
		fsm_obj->state = state;

		fsm_obj->func = tmp->func; 
		if (message != NULL)
			memcpy(fsm_obj->message, message, strlen((char *)message));
		return 0; 
	}
	state_default(fsm_default.state, fsm_default.func, fsm_default.name);

	os_printf("state is no found, and fsm will transfer to default,"
		"It is name: %s\n", fsm_obj->name);
	return 0;
}

/*state dispatch*/
void fsm_while(FSM *obj)
{
	while (1) {
		os_printf("state name = %s\n", obj->name);
		obj->func(obj->message);
	}
}

int state_default(int state, FUNC_PTR func, U8 *name)
{
	fsm_obj->state = state;
	fsm_obj->func  = func;
	fsm_obj->name  = name;

	return 0;
}

/*test FSM*/
#if 0
#define wakeup_state 1
#define asr_state 2
#define tts_state 3
#define comm_state 4
#define sock_state 5


void wakeup_state_func(void *arg)
{
	char *message = (char *)arg;
	if (message != NULL)
		os_printf("%s: %s\n", __func__, message);
	state_tran(asr_state, "wakeup end message");
}

void asr_state_func(void *arg)
{
	char *message = (char *)arg;
	if (message != NULL)
		os_printf("%s: %s\n", __func__, message);
	state_tran(tts_state, "asr end message");
}

void tts_state_func(void *arg)
{
	char *message = (char *)arg;
	if (message != NULL)
		os_printf("%s: %s\n", __func__, message);
	state_tran(wakeup_state, NULL);
}

void comm_state_func(void *arg)
{
	char *message = (char *)arg;
	if (message != NULL)
		os_printf("%s: %s\n", __func__, message);
	state_tran(10, "ten message");
}

void sock_state_func(void *arg)
{
	char *message = (char *)arg;
	if (message != NULL)
		os_printf("%s: %s\n", __func__, message);
	state_tran(10, "ten message");
}

void func_default(void *arg)
{
	char *message = (char *)arg;
	if (message != NULL)
		os_printf("default: %s\n", message);
	state_tran(wakeup_state, "default message");
}

FSM user_app[5] = {
	{wakeup_state, wakeup_state_func, "name_one"},
	{asr_state, asr_state_func, "name_two"},
	{tts_state, tts_state_func, "name_three"},
	{comm_state, comm_state_func, "name_three"},
	{sock_state, sock_state_func, "name_three"},
};

int main()
{
	SET_STATE_DEFAULT_FUNC(func_default);

	state_init(user_app[0].state, user_app[0].func, user_app[0].name);
	state_add(user_app[1].state, user_app[1].func, user_app[1].name);
	state_add(user_app[2].state, user_app[2].func, user_app[2].name);
	fsm_while(fsm_obj);
	return 0;
}
#endif
