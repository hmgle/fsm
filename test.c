#include "fsm.h"

int main(int argc, char **argv)
{
	struct fsm_t fsm;
	fsm_init(&fsm, 2, 2, 0, 0);
	fsm_release(&fsm);
	return 0;
}
