/*
 * http://zh.wikipedia.org/wiki/自动机编程
 */

#include "fsm.h"

enum states {
	before = 0,
	inside = 1,
	after  = 2,
};

enum event {
	S = 0,	/* 空白字符 */
	N = 1,	/* 换行字符 */
	A = 2,	/* 其他字符 */
	Q = 3,	/* EOF */
};

static struct fsm_t *fsm;

static void *stop(void *p)
{
	fsm_stop(fsm);
	return NULL;
}

static int get_char(int *p)
{
	*p = getchar();
	if (*p == '\n')
		return N;
	else if (*p == ' ' || *p == '\t')
		return S;
	else if (*p == EOF)
		return Q;
	else
		return A;
}

static void *print(void *p)
{
	putchar(*(int *)p);
	return NULL;
}

int main(int argc, char **argv)
{
	int c;
	struct fsm_branch branch[][4] = {
		/* before */
		{
			/* S */
			{S, before, NULL,},
			/* N */
			{N, before, print,},
			/* A */
			{A, inside, print,},
			/* Q */
			{Q, 0, stop,},
		},
		/* inside */
		{
			/* S */
			{S, after, NULL,},
			/* N */
			{N, before, print,},
			/* A */
			{A, inside, print,},
			/* Q */
			{Q, 0, stop,},
		},
		/* after */
		{
			/* S */
			{S, after, NULL,},
			/* N */
			{N, before, print,},
			/* A */
			{A, after, NULL,},
			/* Q */
			{Q, 0, stop,},
		},
	};
	struct fsm_state test_fsm_state[] = {
		{before, 4, branch[0],},
		{inside, 4, branch[1],},
		{after,  4, branch[2],},
	};
	fsm = fsm_create_with_state(test_fsm_state, 3, 4, before);
	fsm_run(fsm, get_char, &c, &c, NULL);
	fsm_release(fsm);
	return 0;
}
