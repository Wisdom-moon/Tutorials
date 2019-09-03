
void foo(int loop, long long *tmp, long long *attStart, long long *attEnd, long long *timerData, unsigned long *chip, unsigned long *core, int pId) {
    *attStart = rdtsc();
    for(int i = 0; i < loop; i++) {
        long long start = read_perf_instructions(pId);//__builtin_ia32_rdpmc(INSTR_COUNT);
        *tmp += rdtsc();
        timerData[i] = read_perf_instructions(pId) - start;
    }
    *attEnd = rdtscp(chip, core);
}

int perf_fds;

void init_instructions() {
    struct perf_event_attr attr;
    memset(&attr, 0, sizeof(struct perf_event_attr));
    attr.type = PERF_TYPE_HARDWARE;
    attr.size = sizeof(struct perf_event_attr);
    attr.config = PERF_COUNT_HW_INSTRUCTIONS;
    attr.inherit = 1;
    attr.pinned = 1;
    attr.exclude_idle = 1;
    attr.exclude_kernel = 1;
    perf_fds = perf_event_open(&attr, 0, -1, -1, 0);
    ioctl(perf_fds, PERF_EVENT_IOC_RESET, 0); // Resetting counter to zero
    ioctl(perf_fds, PERF_EVENT_IOC_ENABLE, 0); // Start counters
}

int main () {
    init_instructions();
    for(int attempts = 0; attempts <= 20; ++attempts) {
        rdtscp(&chipOrig, &coreOrig);
        foo(loop, &tmp, &attStart, &attEnd, timerData.data(), &chip, &core, perf_fds);
    }
    close_instructions();
    return 0;
}

