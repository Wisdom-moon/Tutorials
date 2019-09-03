/* ==========================精确计时和模拟延迟============================= */

# define M_PCM_CPUFREQ 1100 /* GHz */

#define NS2CYCLE(__ns) ((__ns) * M_PCM_CPUFREQ / 1000)
#define CYCLE2NS(__cycles) ((__cycles) * 1000 / M_PCM_CPUFREQ)

static inline unsigned long long asm_rdtsc (void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long)lo) | (((unsigned long long)hi) << 32);
}

typedef uint64_t pcm_hrtime_t;



……


pcm_hrtime_t start;
pcm_hrtime_t stop;

start = asm_rdtsc ();

……

stop = asm_rdtsc ();
g_total_t += CYCLE2NS (stop - start);
