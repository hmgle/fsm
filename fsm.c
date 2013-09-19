#include "fsm.h"

struct fsm_t *fsm_init(struct fsm_t *fsm)
{
	assert(fsm != NULL);
	memset(fsm, 0, sizeof(*fsm));
	return fsm;
}

int state_add_branch(struct fsm_state *state, struct fsm_branch *branch)
{
	struct fsm_branch *tmp;

	assert(state != NULL && branch != NULL);
	if (state->branch == NULL) {
		state->branch = malloc(sizeof(*state->branch));
		state->branch->event = branch->event;
		state->branch->new_state = branch->new_state;
		state->branch->func = branch->func;
		state->branch->callback = branch->callback;
		INIT_LIST_HEAD(&state->branch->list);
	} else {
		list_for_each_entry(tmp, &state->branch->list, list) {
			if (tmp->event == branch->event)
				return -1;
		}
		tmp = malloc(sizeof(*tmp));
		memcpy(tmp, branch, sizeof(*tmp));
		list_add(&tmp->list, &state->branch->list);
	}
	return 0;
}

int fsm_add_state(struct fsm_t *fsm, struct fsm_state *state)
{
	struct fsm_state *tmp;

	assert(fsm != NULL && state != NULL);
	if (fsm->state_list == NULL) {
		fsm->state_list = malloc(sizeof(struct fsm_state));
		fsm->state_list->state = state->state;
		fsm->state_list->event_num = state->event_num;
		fsm->state_list->branch = state->branch;
		INIT_LIST_HEAD(&fsm->state_list->list);
	} else {
		list_for_each_entry(tmp, &fsm->state_list->list, list) {
			if (tmp->state == state->state)
				return -1;
		}
		tmp = malloc(sizeof(*tmp));
		memcpy(tmp, state, sizeof(*tmp));
		list_add(&tmp->list, &fsm->state_list->list);
	}
	return 0;
}

int fsm_renew_state(struct fsm_t *fsm, struct fsm_state *state)
{
	struct fsm_state *tmp;
	struct list_head *pos, *q;

	assert(state != NULL);
	if (fsm->state_list == NULL) {
		fsm->state_list = malloc(sizeof(struct fsm_state));
		fsm->state_list->state = state->state;
		fsm->state_list->event_num = state->event_num;
		fsm->state_list->branch = state->branch;
		INIT_LIST_HEAD(&fsm->state_list->list);
	} else {
		list_for_each_safe(pos, q, &fsm->state_list->list){
		 	tmp = list_entry(pos, struct fsm_state, list);
			if (tmp->state == state->state) {
		 		list_del(pos);
		 		free(tmp);

				tmp = malloc(sizeof(*tmp));
				memcpy(tmp, state, sizeof(*tmp));
				list_add(&tmp->list, &fsm->state_list->list);
				return 1;
			}
		}
		tmp = malloc(sizeof(*tmp));
		memcpy(tmp, state, sizeof(*tmp));
		list_add(&tmp->list, &fsm->state_list->list);
	}
	return 0;
}

#if 0
struct fsm_t *fsm_init_with_state(struct fsm_t *fsm, struct fsm_state *state, int state_num, 
				  int event_num, int init_state)
{
	int i;
	int j;

	assert(state != NULL);
	fsm->state_num = state_num;
	fsm->init_state = init_state;
	fsm->curr_state = init_state;
	fsm->state_list = malloc((state_num) * sizeof(struct fsm_state));
	fsm->data = NULL;
	fsm->stop = 0;
	fsm->ret = 0;
	memset(fsm->state_list, 0, (state_num) * sizeof(struct fsm_state));
	for (i = 0; i < state_num; i++) {
		assert(&state[i] != NULL);
		memcpy(&(fsm->state_list)[i], &state[i], sizeof(struct fsm_state));
	}
	for (i = 0; i < state_num; i++) {
		(fsm->state_list)[i].branch = malloc(sizeof(struct fsm_branch) * event_num);
		memset((fsm->state_list)[i].branch, 0, sizeof(struct fsm_branch) * event_num);
		for (j = 0; j < event_num; j++) {
			assert(&((&state[i])->branch)[j]);
			memcpy(&((fsm->state_list)[i].branch)[j], &((&state[i])->branch)[j],
			       sizeof(struct fsm_branch));
		}
	}
	return fsm;
}
#endif

void fsm_release(struct fsm_t *fsm)
{
	int i;

	for (i = fsm->state_num - 1; i >= 0; i--) {
		free((fsm->state_list)[i].branch);
		(fsm->state_list)[i].branch = NULL;
	}
	free(fsm->state_list);
	fsm->state_list = NULL;
	memset(fsm, 0, sizeof(*fsm));
}

int fsm_run(struct fsm_t *fsm, int (*get_event)(void *), void *para,
	    void *func_para, void *cb_para)
{
	int event;

	while (!fsm->stop) {
		event = get_event(para);
		int current_state = fsm->curr_state;
		if (fsm->state_list[current_state].branch[event].func)
			fsm->state_list[current_state].branch[event].func(func_para);
		fsm->curr_state = fsm->state_list[current_state].branch[event].new_state;
		if (fsm->state_list[current_state].branch[event].callback)
			fsm->state_list[current_state].branch[event].callback(cb_para);
	}
	return fsm->ret;
}

