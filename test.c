#include "fsm.h"

static void *foo(void *p);
static void *bar(void *p);

static void *foo(void *p)
{
	printf("foo\n");
	return NULL;
}

static void *bar(void *p)
{
	printf("bar\n");
	return NULL;
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
			{EVENT0, EVENT1, foo, NULL},
			/* EVENT1 */
			{EVENT1, EVENT0, foo, bar},
		},
		/* STATE1 */
		{
			/* EVENT0 */
			{EVENT0, EVENT0, foo, bar},
			/* EVENT1 */
			{EVENT1, EVENT1, foo, NULL},
		},
	};
	struct fsm_state test_fsm_state[] = {
		{STATE0, 2, test_branch[0]},
		{STATE1, 2, test_branch[1]},
	};

	// fsm_init(&fsm, 2, 2, 0, 0);
	// fsm_init_with_state(&fsm, test_fsm_state, 2, 2, EVENT0);
	// fsm_release(&fsm);
	return 0;
}
