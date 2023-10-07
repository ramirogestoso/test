#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

void handler(int sig) {
    printf("Temporizador expirado.\n");
}

int main() {
    timer_t timerid;
    struct sigevent sev;
    struct itimerspec its;

    // Establecer el temporizador para expirar en 5 segundos
    its.it_value.tv_sec = 5;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;

    // Crear el temporizador
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGALRM;
    sev.sigev_value.sival_ptr = &timerid;
    if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1) {
        perror("timer_create");
        return 1;
    }

    // Establecer el manejador de señales
    struct sigaction sa;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = handler;
    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    // Establecer el temporizador
    if (timer_settime(timerid, 0, &its, NULL) == -1) {
        perror("timer_settime");
        return 1;
    }

    // Esperar a que el temporizador expire
    while(1) {
        sleep(1);
    }

    return 0;
}