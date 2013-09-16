#include "fsm.h"

struct fsm_t *fsm_init(struct fsm_t *fsm, int state_num, int event_num, 
		int init_state, int curr_state)
{
	int i;

	assert(state_num > 0 && event_num > 0);
	fsm->state_num = state_num;
	fsm->init_state = init_state;
	fsm->curr_state = curr_state;
	fsm->state_list = malloc((state_num) * sizeof(struct fsm_state));
	memset(fsm->state_list, 0, (state_num) * sizeof(struct fsm_state));
	for (i = 0; i < state_num; i++) {
		(fsm->state_list)[i].branck = malloc(sizeof(struct fsm_state) * event_num);
		memset((fsm->state_list)[i].branck, 0, sizeof(struct fsm_state) * event_num);
	}
	return fsm;
}

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
	memset(fsm->state_list, 0, (state_num) * sizeof(struct fsm_state));
	for (i = 0; i < state_num; i++) {
		assert(&state[i] != NULL);
		memcpy(&(fsm->state_list)[i], &state[i], sizeof(struct fsm_state));
	}
	for (i = 0; i < state_num; i++) {
		(fsm->state_list)[i].branck = malloc(sizeof(struct fsm_state) * event_num);
		memset((fsm->state_list)[i].branck, 0, sizeof(struct fsm_state) * event_num);
		for (j = 0; j < event_num; j++) {
			assert(&((&state[i])->branck)[j]);
			memcpy(&((fsm->state_list)[i].branck)[j], &((&state[i])->branck)[j], sizeof(struct fsm_branch));
		}
	}
	return fsm;
}

void fsm_release(struct fsm_t *fsm)
{
	int i;

	for (i = fsm->state_num - 1; i >= 0; i--) {
		free((fsm->state_list)[i].branck);
		(fsm->state_list)[i].branck = NULL;
	}
	free(fsm->state_list);
	fsm->state_list = NULL;
	memset(fsm, 0, sizeof(*fsm));
}
