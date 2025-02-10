#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Frame {
    int seq;
    char data;
};

#define TIMEOUT 3
#define MAX_SEQ 4
#define NUM_FRAMES 5

int sender_turn = 1;
int frame_to_send = 0;
int expected_ack = 0;
int timeout_counter = 0;

int simulate_error();
void sender(struct Frame frame);
void receiver(struct Frame frame);

int main() {
    srand(time(NULL));

    struct Frame current_frame;
    current_frame.data = 'A';

    while (frame_to_send < NUM_FRAMES) {
        if (sender_turn) {
            sender(current_frame);
        } else {
            receiver(current_frame);
        }
    }

    printf("Communication complete.\n");
    return 0;
}

int simulate_error() {
    return (rand() % 5 == 0);
}

void sender(struct Frame frame) {
    if (timeout_counter >= TIMEOUT) {
        printf("SENDER: Timeout occurred! Resending frame %d\n", expected_ack);
        timeout_counter = 0;
    } else {
        if(frame_to_send == expected_ack) {
            frame.seq = frame_to_send;
            printf("SENDER: Sending frame %d with data '%c'\n", frame.seq, frame.data + frame.seq);

            if (simulate_error()) {
                printf("SENDER: Error sending frame %d!\n", frame.seq);
            } else {
                sender_turn = 0;
                timeout_counter++;
            }
        } else {
            printf("SENDER: Waiting for ACK %d, current ACK = %d.\n", expected_ack, frame_to_send);
            timeout_counter++;
        }
    }
}

void receiver(struct Frame frame) {
    if (simulate_error()) {
        printf("RECEIVER: Error receiving frame! ACK %d not sent.\n", frame.seq);
        sender_turn = 1;
        timeout_counter++;
    } else {
        if (frame.seq == expected_ack) {
            printf("RECEIVER: Received frame %d with data '%c' successfully.\n", frame.seq, frame.data + frame.seq);
            expected_ack++;
            frame_to_send = expected_ack;
            sender_turn = 1;
            timeout_counter = 0;
        } else {
            printf("RECEIVER: Received duplicate or out-of-order frame %d (expecting %d). Sending ACK for %d again.\n", frame.seq, expected_ack, expected_ack-1);
            sender_turn = 1;
            timeout_counter = 0;
        }
    }
}

