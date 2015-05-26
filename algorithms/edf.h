#include "../modules/process.h"

void init_scheduler(struct process* list, int size, int ramend);
int next_task(struct process* list, int size);