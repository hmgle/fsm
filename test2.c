/*
 * http://zh.wikipedia.org/wiki/自动机编程
 */

#include "fsm.h"
#include <ctype.h>

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

struct para {
	struct fsm_t *fsm;
	int c;
};

static void *stop(void *p)
{
	fsm_stop(((struct para *)p)->fsm);
	return NULL;
}

static int get_char(void *p)
{
	((struct para *)p)->c = getchar();
	if (((struct para *)p)->c == '\n')
		return N;
	else if (isblank(((struct para *)p)->c))
		return S;
	else if (((struct para *)p)->c == EOF)
		return Q;
	else
		return A;
}

static void *print(void *p)
{
	putchar(((struct para *)p)->c);
	return NULL;
}

int main(int argc, char **argv)
{
	struct para func_para;
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
	struct fsm_state fsm_state[] = {
		{before, sizeof(branch[0]) / sizeof(branch[0][0]), branch[0],},
		{inside, sizeof(branch[1]) / sizeof(branch[1][0]), branch[1],},
		{after,  sizeof(branch[2]) / sizeof(branch[2][0]), branch[2],},
	};
	func_para.fsm = fsm_create_with_state(fsm_state, 
				sizeof(fsm_state) / sizeof(fsm_state[0]), 
				sizeof(branch[0]) / sizeof(branch[0][0]),
				before);
	fsm_run(func_para.fsm, get_char, &func_para, &func_para, NULL);
	fsm_release(func_para.fsm);
	return 0;
}
