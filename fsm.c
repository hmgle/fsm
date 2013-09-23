#include "fsm.h"

struct fsm_t *fsm_init(struct fsm_t *fsm)
{
	assert(fsm != NULL);
	memset(fsm, 0, sizeof(*fsm));
	return fsm;
}

int state_add_branch(struct fsm_state *state, const struct fsm_branch *branch)
{
	struct fsm_branch *tmp;

	assert(state != NULL && branch != NULL);
	if (state->branch == NULL) {
		state->branch = malloc(sizeof(*state->branch));
		INIT_LIST_HEAD(&state->branch->list);
		tmp = malloc(sizeof(*tmp));
		memcpy(tmp, branch, sizeof(*tmp));
		list_add(&tmp->list, &state->branch->list);
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

int state_renew_branch(struct fsm_state *state, const struct fsm_branch *branch)
{
	struct fsm_branch *tmp;
	struct list_head *pos, *q;

	assert(state != NULL && branch != NULL);
	if (state->branch == NULL) {
		state->branch = malloc(sizeof(*state->branch));
		INIT_LIST_HEAD(&state->branch->list);
		tmp = malloc(sizeof(*tmp));
		memcpy(tmp, branch, sizeof(*tmp));
		list_add(&tmp->list, &state->branch->list);
	} else {
		list_for_each_safe(pos, q, &state->branch->list) {
		 	tmp = list_entry(pos, struct fsm_branch, list);
			if (tmp->event == branch->event) {
		 		list_del(pos);
		 		free(tmp);

				tmp = malloc(sizeof(*tmp));
				memcpy(tmp, branch, sizeof(*tmp));
				list_add(&tmp->list, &state->branch->list);
				return 1;
			}
		}
		tmp = malloc(sizeof(*tmp));
		memcpy(tmp, branch, sizeof(*tmp));
		list_add(&tmp->list, &state->branch->list);
	}
	return 0;
}

int fsm_add_state(struct fsm_t *fsm, const struct fsm_state *state)
{
	struct fsm_state *tmp;

	assert(fsm != NULL && state != NULL);
	if (fsm->state_list == NULL) {
		fsm->state_list = malloc(sizeof(struct fsm_state));
		INIT_LIST_HEAD(&fsm->state_list->list);
		tmp = malloc(sizeof(*tmp));
		memcpy(tmp, state, sizeof(*tmp));
		list_add(&tmp->list, &fsm->state_list->list);
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

int fsm_renew_state(struct fsm_t *fsm, const struct fsm_state *state)
{
	struct fsm_state *tmp;
	struct list_head *pos, *q;

	assert(state != NULL);
	if (fsm->state_list == NULL) {
		fsm->state_list = malloc(sizeof(struct fsm_state));
		INIT_LIST_HEAD(&fsm->state_list->list);
		tmp = malloc(sizeof(*tmp));
		memcpy(tmp, state, sizeof(*tmp));
		list_add(&tmp->list, &fsm->state_list->list);
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

struct fsm_t *fsm_init_with_state(struct fsm_t *fsm, const struct fsm_state *state, int state_num, 
				  int event_num, int init_state)
{
	int i;
	int j;
	struct fsm_state tmp_state;

	assert(state != NULL && fsm != NULL);
	fsm->state_num = state_num;
	fsm->init_state = init_state;
	fsm->curr_state = init_state;
	fsm->data = NULL;
	fsm->stop = 0;
	fsm->ret = 0;
	fsm->state_list = NULL;
	for (i = 0; i < state_num; i++) {
		memset(&tmp_state, 0, sizeof(tmp_state));
		tmp_state.state = state[i].state;
		tmp_state.event_num = state[i].event_num;
		for (j = 0; j < event_num; j++)
			state_add_branch(&tmp_state, &state[i].branch[j]);
		fsm_add_state(fsm, &tmp_state);
	}
	return fsm;
}

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
	struct fsm_state *tmp_state;
	struct fsm_branch *tmp_branch;

	while (!fsm->stop) {
		event = get_event(para);
		list_for_each_entry(tmp_state, &fsm->state_list->list, list) {
			if (tmp_state->state == fsm->curr_state) {
				list_for_each_entry(tmp_branch, &tmp_state->branch->list, list) {
					if (tmp_branch->event == event) {
						if (tmp_branch->func)
							tmp_branch->func(func_para);
						fsm->curr_state = tmp_branch->new_state;
						if (tmp_branch->callback)
							tmp_branch->callback(cb_para);
						goto next_time;
					}
				}
			}
		}
next_time:
		;
	}
	return fsm->ret;
}

void fsm_print(struct fsm_t *fsm)
{
	struct fsm_state *tmp_state;
	struct fsm_branch *tmp_branch;

	list_for_each_entry(tmp_state, &fsm->state_list->list, list) {
		list_for_each_entry(tmp_branch, &tmp_state->branch->list, list) {
			debug_print("state is %d", tmp_state->state);
			debug_print("event_num is %d", tmp_state->event_num);
			debug_print("branch -> event is %d", tmp_branch->event);
			debug_print("branch -> new_state is %d", tmp_branch->new_state);
		}
	}
	debug_print();
}

