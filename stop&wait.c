#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct frame {
    int info;
    int seq;
} p;

int ack, expected = 1;
int error_frame = 1, error_ack = 1;
char turn = 's';
int done = 0;

void sender() {
    static int retries = 0;
    if (turn != 's') return;

    if (retries > 0) printf("SENDER: Retrying...\n");

    p.info++;
    p.seq++;

    printf("SENDER: Sending packet %d\n", p.seq);
    error_frame = rand() % 4;
    if (error_frame == 0) printf("Packet lost!\n");

    turn = 'r';
    retries++;
}

void receiver() {
    if (turn != 'r') return;

    if (error_frame == 0) return;

    if (p.seq == expected) {
        printf("RECEIVER: Packet %d received\n", p.seq);
        ack = p.seq;
        expected++;
    } else {
        printf("RECEIVER: Duplicate packet %d\n", expected - 1);
        ack = expected - 1;
    }

    error_ack = rand() % 4;
    if (error_ack == 0) {
        printf("ACK lost!\n");
    } else {
        printf("RECEIVER: ACK %d sent\n", ack);
        turn = 's';
    }

    if (p.seq == 5) done = 1;
}

int main() {
    srand(time(NULL));
    p.info = 0; p.seq = 0;

    while (!done) {
        sender();
        usleep(500000);
        receiver();
    }

    return 0;
}