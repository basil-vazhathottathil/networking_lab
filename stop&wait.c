#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct frame {
    int seq;
};

int disconnect = 0;
int ack = 0;
int expected_seq = 1;
char turn = 's'; // 's' for sender, 'r' for receiver
struct frame packet;

void sender();
void receiver();

int main() {
    packet.seq = 0;

    while (!disconnect) {
        if (turn == 's') sender();
        else receiver();
        usleep(500000); // Simulate delay
    }

    return 0;
}

void sender() {
    static int retries = 5;

    if (retries == 0) { // Timeout occurred
        printf("SENDER: Timeout! Resending packet %d\n", packet.seq);
        retries = 5;
        turn = 'r';
        return;
    }

    if (packet.seq == 5) { // End communication after 5 packets
        disconnect = 1;
        printf("SENDER: Communication complete.\n");
        return;
    }

    packet.seq++;
    printf("SENDER: Sending packet %d\n", packet.seq);

    if (rand() % 4 == 0) { // Simulate packet loss
        printf("SENDER: Packet %d lost!\n", packet.seq);
        retries--;
        return;
    }

    turn = 'r'; // Hand over to receiver
}

void receiver() {
    if (rand() % 4 == 0) { // Simulate ACK loss
        printf("RECEIVER: ACK lost for packet %d\n", ack);
        turn = 's'; // Sender will retry
        return;
    }

    if (packet.seq == expected_seq) { // Correct sequence
        printf("RECEIVER: Received packet %d\n", packet.seq);
        ack = expected_seq++;
    } else { // Duplicate or out-of-order packet
        printf("RECEIVER: Duplicate/out-of-order packet %d\n", packet.seq);
    }

    printf("RECEIVER: Sending ACK %d\n", ack);
    turn = 's'; // Hand over to sender
}
