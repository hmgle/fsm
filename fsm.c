#include "fsm.h"

struct fms_t *fms_init(struct fms_t *fms, int state_num, int event_num, 
		int init_state, int curr_state)
{
	int i;

	fms->state_num = state_num;
	fms->init_state = init_state;
	fms->curr_state = curr_state;
	fms->state_list = malloc((state_num) * sizeof(struct fms_state));
	memset(fms->state_list, 0, (state_num) * sizeof(struct fms_state));
	for (i = 0; i < state_num; i++) {
		(fms->state_list)[i].branck = malloc(sizeof(struct fms_state));
		memset((fms->state_list)[i].branck, 0, sizeof(struct fms_state));
	}
	return fms;
}

void fms_release(struct fms_t *fms)
{
}
