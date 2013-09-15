#ifndef _FSM_H
#define _FSM_H

struct fms_branch {
	int new_state;
	void *(*func)(void *);
	void *(*callback)(void *);
};

struct fms_state {
	int state;
	struct fms_branch *branck;
};

struct fms {
	int state_num;
	struct fms_state *state_list;
	int init_state;
	int curr_state;
};

#endif
