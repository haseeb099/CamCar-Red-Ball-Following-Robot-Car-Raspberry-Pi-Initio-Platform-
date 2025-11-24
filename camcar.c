#include <stdlib.h>
#include <initio.h>
#include <curses.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "detect_blob.h"

#define DIST_MIN 60
#define DIST_MAX 100
#define OBSTACLE_THRESHOLD 30
#define BLOB_SIZE_THRESHOLD 50
#define SPIN_TIMEOUT 2.0

struct thread_dat {
    TBlobSearch blob;
    int blobnr;
    int bExit;
    pthread_mutex_t mutex;
};

clock_t last_spin_time = 0;

void camcar(int argc, char *argv[], struct thread_dat *ptdat);
void *worker(void *p_thread_dat);

void initio_DriveBackward(int speed) {
    initio_DriveForward(-speed);
}

int main(int argc, char *argv[]) {
    WINDOW *mainwin = initscr();
    noecho();
    cbreak();
    nodelay(mainwin, TRUE);
    keypad(mainwin, TRUE);

    initio_Init();

    pthread_t cam_thread;
    pthread_attr_t pt_attr;
    struct thread_dat tdat = { 
        .blob = { 
            .blob = {0}, 
            .halign = 0.0, 
            .valign = 0.0, 
            .size = 0, 
            .pimg = NULL 
        }, 
        .blobnr = 0, 
        .bExit = 0 
    };
    pthread_mutex_init(&tdat.mutex, NULL);

    pthread_attr_init(&pt_attr);
    pthread_create(&cam_thread, &pt_attr, worker, &tdat);

    camcar(argc, argv, &tdat);

    tdat.bExit = 1;
    pthread_join(cam_thread, NULL);
    pthread_attr_destroy(&pt_attr);
    pthread_mutex_destroy(&tdat.mutex);

    initio_Cleanup();
    endwin();
    return EXIT_SUCCESS;
}

void camcar(int argc, char *argv[], struct thread_dat *ptdat) {
    int ch = 0;
    TBlobSearch blob = { .blob = {0}, .halign = 0.0, .valign = 0.0, .size = 0, .pimg = NULL };

    while (ch != 'q') {
        pthread_mutex_lock(&ptdat->mutex);
        blob = ptdat->blob;
        pthread_mutex_unlock(&ptdat->mutex);

        int obstacle_L = initio_IrLeft();
        int obstacle_R = initio_IrRight();
        int distance = initio_UsGetDistance();
        int obstacle = (obstacle_L || obstacle_R) && (distance < OBSTACLE_THRESHOLD);

        mvprintw(6, 1, "Blob Info: Size=%d, Halign=%.2f, Valign=%.2f",
                 blob.size, blob.halign, blob.valign);
        mvprintw(7, 1, "Distance: %d cm, Obstacle Left: %d, Obstacle Right: %d",
                 distance, obstacle_L, obstacle_R);

        if (obstacle) {
            mvprintw(3, 1, "State: Obstacle Detected, stopping.");
            initio_DriveForward(0);
        } else if (blob.size > BLOB_SIZE_THRESHOLD) {
            double halign = blob.halign;

            if (halign < -0.15) {
                mvprintw(3, 1, "State: Aligning to Blob (turning right)");
                initio_SpinRight(30);
                last_spin_time = clock();
            } else if (halign > 0.15) {
                mvprintw(3, 1, "State: Aligning to Blob (turning left)");
                initio_SpinLeft(30);
                last_spin_time = clock();
            } else {
                if (distance < DIST_MIN) {
                    mvprintw(3, 1, "State: Too Close, moving backward.");
                    initio_DriveBackward(30);
                } else if (distance > DIST_MAX) {
                    mvprintw(3, 1, "State: Too Far, moving forward.");
                    initio_DriveForward(30);
                } else {
                    mvprintw(3, 1, "State: Maintaining Distance.");
                    initio_DriveForward(0);
                }
            }
        } else {
            mvprintw(3, 1, "State: Searching for Blob.");
            if ((double)(clock() - last_spin_time) / CLOCKS_PER_SEC > SPIN_TIMEOUT) {
                mvprintw(4, 1, "Spin Timeout, stopping spin.");
                initio_DriveForward(0);
            }
        }

        refresh();
        ch = getch();
    }
}

void *worker(void *p_thread_dat) {
    struct thread_dat *ptdat = (struct thread_dat *)p_thread_dat;
    const char blobColor[3] = { 255, 0, 0 };

    while (!ptdat->bExit) {
        TBlobSearch blob = cameraSearchBlob(blobColor);

        pthread_mutex_lock(&ptdat->mutex);
        ptdat->blob = blob;
        ptdat->blobnr++;
        pthread_mutex_unlock(&ptdat->mutex);

        usleep(100000);
    }
    return NULL;
}
