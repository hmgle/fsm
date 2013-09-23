#include "fsm.h"

/*
 *           a / 0                 a / 1           
 *           ____                  ____           
 *          /    \                /    \
 *         |     |               |     |           
 *          \    V                \    V           
 *        +--------+   b / 1    +--------+
 * -----> | state0 |----------->| state1 |
 *        +--------+            +--------+
 *                                /    ^
 *                               |     |
 *                                \    /
 *                                 ---
 *                                 b / 0
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
	else
		goto re;
}

int main(int argc, char **argv)
{
	struct fsm_t fsm;
	enum test_state {
		STATE0 = 0,
		STATE1 = 1,
	};
	enum test_event {
		EVENT0 = 0,
		EVENT1 = 1,
	};
	struct fsm_branch test_branch[][2] = {
		/* STATE0 */
		{
			/* EVENT0 */
			{EVENT0, STATE0, foo, NULL},
			/* EVENT1 */
			{EVENT1, STATE1, bar, NULL},
		},
		/* STATE1 */
		{
			/* EVENT0 */
			{EVENT0, STATE1, bar, NULL},
			/* EVENT1 */
			{EVENT1, STATE1, foo, NULL},
		},
	};
	struct fsm_state test_fsm_state[] = {
		{STATE0, 2, test_branch[0]},
		{STATE1, 2, test_branch[1]},
	};

	// fsm_init(&fsm, 2, 2, 0, 0);
	fsm_init_with_state(&fsm, test_fsm_state, 2, 2, EVENT0);
	fsm_run(&fsm, get_num, NULL, NULL, NULL);
	// fsm_release(&fsm);
	return 0;
}
