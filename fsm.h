#ifndef _FSM_H
#define _FSM_H

struct fsm {
	char *current_state;
	char **state;
	void *event;
}

#endif
