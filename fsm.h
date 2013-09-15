#ifndef _FSM_H
#define _FSM_H

#include <stdlib.h>
#include <string.h>

struct fms_branch {
	int event;
	int new_state;
	void *(*func)(void *);
	void *(*callback)(void *);
};

struct fms_state {
	int state;
	struct fms_branch *branck;
};

struct fms_t {
	int state_num;
	struct fms_state *state_list;
	int init_state;
	int curr_state;
};

struct fms_t *fms_init(struct fms_t *fms, int state_num, int event_num, int init_state, int curr_state);
void fms_release(struct fms_t *fms);

#endif
