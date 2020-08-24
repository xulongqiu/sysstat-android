#define LOG_NDEBUG 0
#define LOG_TAG

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


void* stress_thread(void* arg) {
    struct sched_param param;
    float tmp = 25.369f;
    int policy;

    pthread_detach(pthread_self());
    prctl(PR_SET_NAME, arg, NULL, NULL, NULL);

    pthread_getschedparam(pthread_self(), &policy, &param);
    printf("%s policy=%s, priority=%d\n", arg, \
        (policy == SCHED_FIFO)  ? "SCHED_FIFO" : \
        (policy == SCHED_RR)    ? "SCHED_RR" : \
        (policy == SCHED_OTHER) ? "SCHED_OTHER" : \
        "unkown", param.sched_priority);

    while(1) {
       tmp*=255.05f;
    }

    return NULL;
}

int run_task(const char* name, int policy, int priority) {
    pthread_t t_id;
    pthread_attr_t attr;
    struct sched_param param;
    int min, max, ret = -1;

    pthread_attr_init(&attr);
    if( (SCHED_RR == policy) || (SCHED_FIFO == policy) ){
        //pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
        ret = pthread_attr_setschedpolicy(&attr, policy);
        if( 0 == ret ){
            min = sched_get_priority_min(policy);
            max = sched_get_priority_max(policy);
            if( (min <= priority) && (priority <= max) ){
                pthread_attr_getschedparam(&attr, &param);
                param.sched_priority = priority;
                ret = pthread_attr_setschedparam(&attr, &param);
                if( 0 != ret ){
                    printf("%s sched param set failed, %d, %s\n", name, ret, strerror(ret));
                }
            }
        }
        else{
            printf("%s sched policy set failed, %d, %s\n", name, ret, strerror(ret));
        }
    }

    pthread_create(&t_id, &attr, stress_thread, name);

    return t_id;
}

int main(int argc, char *argv[]) {

    run_task("stress_0", SCHED_OTHER, 1);
    run_task("stress_1", SCHED_FIFO, 20);

    while(1) {
        sleep(100);
    }

    return 0;
}
