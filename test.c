#include "fsm.h"

/*
 *           a / 0                 a / 1           
 *           ____                  ____           
 *          /    \                /    \
 *         |     |               |     |           
 *          \    V                \    V           
 *        +--------+   b / 1    +--------+   q    /------\
 * -----> | state0 |----------->| state1 |------->| STOP |
 *        +--------+            +--------+        \------/
 *             |                  /    ^             ^
 *            q|                 |     |             |
 *             |                  \    /             |
 *             |                   ---               |
 *             |                   b / 0             |
 *             |                                     |
 *             \_____________________________________/
 */

static void *foo(void *p);
static void *bar(void *p);
static int get_num(void *p);

static void *foo(void *p)
{
	fprintf(stderr, "0 ");
	return NULL;
}

static void *bar(void *p)
{
	fprintf(stderr, "1 ");
	return NULL;
}

static void *stop(void *p)
{
	fprintf(stderr, "stop \n");
	fsm_stop((struct fsm_t *)p);
	return NULL;
}

static int get_num(void *p)
{
	int c;

re:
	c = getchar();
	getchar();
	if (c == 'a')
		return 0;
	else if (c == 'b')
		return 1;
	else if (c == 'q')
		return 2; /* QUIT */
	else
		goto re;
}

int main(int argc, char **argv)
{
	struct fsm_t *fsm;
	enum test_state {
		STATE0 = 0,
		STATE1 = 1,
	};
	enum test_event {
		EVENT0 = 0,
		EVENT1 = 1,
		QUIT = 2,
	};
	struct fsm_branch test_branch[][3] = {
		/* STATE0 */
		{
			/* EVENT0 */
			{EVENT0, STATE0, foo,},
			/* EVENT1 */
			{EVENT1, STATE1, bar,},
			/* QUIT */
			{QUIT, 0, stop,},
		},
		/* STATE1 */
		{
			/* EVENT0 */
			{EVENT0, STATE1, bar,},
			/* EVENT1 */
			{EVENT1, STATE1, foo,},
			/* QUIT */
			{QUIT, 0, stop,},
		},
	};
	struct fsm_state test_fsm_state[] = {
		{STATE0, 3, test_branch[0],},
		{STATE1, 3, test_branch[1],},
	};

	fsm = fsm_create_with_state(test_fsm_state, 2, 3, EVENT0);
	fsm_run(fsm, get_num, NULL, fsm, NULL);
	fsm_release(fsm);
	return 0;
}
