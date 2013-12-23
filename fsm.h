#ifndef _FSM_H
#define _FSM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "list.h"
#include "debug_log.h"

struct fsm_branch {
	int event;
	int new_state;
	void *(*func) (void *);
	void *(*callback) (void *);
	struct list_head list;
};

struct fsm_state {
	int state;
	int event_num;
	struct fsm_branch *branch;
	struct list_head list;
};

struct fsm_t {
	int state_num;
	struct fsm_state *state_list;
	int init_state;
	int curr_state;
	void *data;
	int stop;
	int ret;
};

struct fsm_t *fsm_create(void);
struct fsm_t *fsm_create_with_state(const struct fsm_state *state,
				    int state_num, int event_num,
				    int init_state);
int state_add_branch(struct fsm_state *state, const struct fsm_branch *branch);
int state_renew_branch(struct fsm_state *state,
		       const struct fsm_branch *branch);
int state_del_branch(struct fsm_state *state, int event);
int state_del_all_branch(struct fsm_state *state);
int fsm_add_state(struct fsm_t *fsm, const struct fsm_state *state);
int fsm_renew_state(struct fsm_t *fsm, const struct fsm_state *state);
int fsm_del_state(struct fsm_t *fsm, int state);
int fsm_del_all_state(struct fsm_t *fsm);
void fsm_release(struct fsm_t *fsm);
int fsm_run(struct fsm_t *fsm, int (*get_event)(void *), void *get_event_para,
	    void *func_para, void *cb_para);
void fsm_print(struct fsm_t *fsm);

static inline void fsm_stop(struct fsm_t *fsm)
{
	fsm->stop = 1;
}

static inline int fsm_get_current_state(const struct fsm_t *fsm)
{
	return fsm->curr_state;
}

#endif
