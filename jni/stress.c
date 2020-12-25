#define LOG_NDEBUG 0
#define LOG_TAG
#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TEST_FILE_SRC "/sdcard/vpm/test.wav"
#define TEST_FILE_DES "/sdcard/vpm/test.wav.bak"

static inline uint64_t  get_sys_millis()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000L) + (tv.tv_usec / 1000L);
}

static void* cpu_io_task(const char* name)
{
    FILE* fpw;
    FILE* fpr;
    int tries = 10;
    float tmp = 25.369f, result = 0.0f;
    uint64_t timestamp;

    fpr = fopen(TEST_FILE_SRC, "r");
    fpw = fopen(TEST_FILE_DES, "w");

    if (fpr == NULL || fpw == NULL) {
        printf("can not open file\n");
        return NULL;
    }

    timestamp = get_sys_millis();

    while (tries > 0) {
        int  test = 0;
        size_t ret = 0;

        ret = fread((void*)&test, sizeof(test), 1, fpr);

        if (ret == 1) {
            result = tmp * test;
            test = (int)result;
            fwrite((void*)&test, sizeof(test), 1, fpw);
            fflush(fpw);
        } else {
            fseek(fpr, 0, SEEK_SET);
            fseek(fpw, 0, SEEK_SET);
            printf("thread %s test %d, use millis=%llu\n", name, tries, get_sys_millis() - timestamp);
            timestamp = get_sys_millis();
            tries--;
        }
    }

    fclose(fpr);
    fclose(fpw);

    return NULL;
}

static inline void* simple_task(const char* name)
{
    while (1) {
        int a = 25 + 36;
    }

    return NULL;
}

static void inline cpu_affinity(const char* name)
{
    int cpu_num = 1;//sysconf(_SC_NPROCESSORS_ONLN);
    cpu_set_t set;
    cpu_set_t get;

    CPU_ZERO(&set);
    CPU_SET(cpu_num, &set);

    if (sched_setaffinity(0, sizeof(set), &set) == -1) {
        printf("%s cpu_affinity.sched_setaffinity.error\n", name);
    } else {
        printf("%s cpu_affinity.sched_setaffinity.success\n", name);
    }

    CPU_ZERO(&get);

    if (sched_getaffinity(0, sizeof(cpu_set_t), &get) == -1) {
        printf("%s cpu_affinity.sched_getaffinity.error\n", name);
    } else {
        printf("%s cpu_affinity.sched_getaffinity.success\n", name);
    }

    if (CPU_ISSET(cpu_num, &get)) {
        printf("%s cpu_affinity.cpu=%d\n", name, cpu_num);
    }
}


static void* stress_thread(void* arg)
{
    struct sched_param param;
    float tmp = 25.369f, result = 0.02498f;
    int policy;

    pthread_detach(pthread_self());
    prctl(PR_SET_NAME, arg, NULL, NULL, NULL);

    pthread_getschedparam(pthread_self(), &policy, &param);
    printf("%s policy=%s, priority=%d\n", arg, \
           (policy == SCHED_FIFO)  ? "SCHED_FIFO" : \
           (policy == SCHED_RR)    ? "SCHED_RR" : \
           (policy == SCHED_OTHER) ? "SCHED_OTHER" : \
           "unkown", param.sched_priority);

    //cpu_affinity((const char*)arg);

    while(1) {
        tmp *= result;
    }

    return NULL;
}

static pthread_t run_task(const char* name, int policy, int priority)
{
    pthread_t t_id;
    pthread_attr_t attr;
    struct sched_param param;
    int min, max, ret = -1;

    printf("run_task.name=%s, policy=%d, priority=%d\n", name, policy, priority);

    pthread_attr_init(&attr);

    if ((SCHED_RR == policy) || (SCHED_FIFO == policy)) {
        //pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
        ret = pthread_attr_setschedpolicy(&attr, policy);

        if (0 == ret) {
            min = sched_get_priority_min(policy);
            max = sched_get_priority_max(policy);

            if ((min <= priority) && (priority <= max)) {
                pthread_attr_getschedparam(&attr, &param);
                param.sched_priority = priority;
                ret = pthread_attr_setschedparam(&attr, &param);

                if (0 != ret) {
                    printf("%s sched param set failed, %d, %s\n", name, ret, strerror(ret));
                }
            }
        } else {
            printf("%s sched policy set failed, %d, %s\n", name, ret, strerror(ret));
        }
    }

    ret = pthread_create(&t_id, &attr, stress_thread, name);

    if (ret != 0) {
        printf("pthread_create.name=%s, ret=%d, err=%s\n", name, ret, strerror(ret));
    }

    return t_id;
}

static void inline usage(const char* name)
{
    printf("\nUsage: %s -c <thread_count> [-s <scheduler_name> -p <priority>]\n", name);
    printf("\n");
    printf("  -c <thread_count>           threads count, default 1\n");
    printf("  -s <scheduler_name>         scheduler, default SCHED_OTHER (SCHED_FIFO %d, SCHED_RR %d)\n", \
           SCHED_FIFO, SCHED_RR);
    printf("  -p <priority>               priority, default 20 (SCHED_OTHER [%d, %d], SCHED_FIFO [%d, %d], SCHED_RR [%d, %d])\n", \
           sched_get_priority_min(SCHED_OTHER), sched_get_priority_max(SCHED_OTHER), \
           sched_get_priority_min(SCHED_FIFO), sched_get_priority_max(SCHED_FIFO), \
           sched_get_priority_min(SCHED_RR), sched_get_priority_max(SCHED_RR));
    printf("  -h                          print usage info\n");
    printf("\n\n");
    exit(0);
}

#define THREAD_COUNT_MAX (20)
typedef struct {
    pthread_t tid;
    char name[64];
} task_t;

int main(int argc, char* argv[])
{
    int tid_1 = 0;
    int tid_2 = 0;
    uint64_t timestamp;
    int opt;
    int thread_count = 1;
    int policy = SCHED_OTHER;
    int priority = 0;
    task_t tasks[THREAD_COUNT_MAX];

    memset(tasks, 0, sizeof(tasks));

    while ((opt = getopt(argc, argv, "c:s:p:h")) != -1) {
        switch (opt) {
        case 'c':
            thread_count = atoi(optarg);
            break;

        case 's':
            policy = atoi(optarg);

        case 'p':
            priority = atoi(optarg);
            break;

        case 'h':
        case '?':
        default:
            usage(argv[0]);
        }
    }

    if(policy != SCHED_OTHER && policy != SCHED_FIFO && policy != SCHED_RR) {
        printf("unknown scheduler\n");
        usage(argv[0]);
    }

    if(priority > sched_get_priority_max(policy) || priority < sched_get_priority_min(policy)) {
        printf("priority is not right\n");
        usage(argv[0]);
    }

    printf("thread_count=%d, priority=%d, policy=%d\n", thread_count, priority, policy);

    timestamp = get_sys_millis();

    for (int i = 0; i < thread_count; i++) {
        int tid = 0;
        snprintf(tasks[i].name, sizeof(tasks[i].name), "stress_%02d", i);
        tid = run_task(tasks[i].name, policy, priority);
        if (tid != 0) {
            tasks[i].tid = tid;
        } else {
            break;
        }
    }

    for (int j = 0; j < THREAD_COUNT_MAX; j++) {
        if (tasks[j].tid != 0) {
            pthread_join(tasks[j].tid, NULL);
        } else {
            break;
        }
    }

    printf("time millis: %llu\n", get_sys_millis() - timestamp);

    return 0;
}
