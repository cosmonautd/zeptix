struct process {
    int  pid;
    int  stack_size;
    int  stack_pointer;
    void *task_pointer;
    int running;
    int finished;
    int priority;
    int lottery_cards;
    int deadline;
};
