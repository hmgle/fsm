#ifndef _FSM_H
#define _FSM_H

typedef void (*event_callback_t)(void *);

typedef void (*event_t)(int src_state, int dst_state, event_callback_t cb);

struct fsm_t {
	int *all_state;
	int current_state;
	int initial_state;
	event_t *event;
};

struct fsm_t *fsm_init(struct fsm_t *fsm, const int *all_state_list, int init_state, const event_t *event_list);

#endif
