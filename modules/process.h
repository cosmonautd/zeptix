struct process {
    int  pid;
    int  stack_size;
    int  stack_pointer;
    void *task_pointer;
    int running;
    int finished;
    int priority;
    double lottery_cards;
};
