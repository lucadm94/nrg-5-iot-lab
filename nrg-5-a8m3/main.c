#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include "iota8m3_serial.h"
//#include "iotm3_ops.h"


#define IEEE802154_SHORT_ADDRESS_LEN 2

int c;
uint8_t msg[128];
uint8_t hw_addr[2];
char buf [MAX_SER_MSG_LEN];

char *fileline;
size_t linelen;
char *nodeidstr;
uint8_t skip;


int main(int argc, char *argv[]) {

    serial_t sfd;
    DEBUG("main: connecting sfd...");
    sfd = serial_connect();
    if (sfd < 0) {
        ERROR("main: cannot open port");
        return -1;
    }

    INFO("main: serial connected with 5 sec timeout non-blocking");


    int n = 0;
    msg_type_t msg_typ;

//    printf("main: waiting for m3\n");

//    while(!n) {   // TODO: change to !n
//        n = read(sfd, buf, sizeof buf);  // read up to 100 characters if ready to read
//        if (n>0) {
//
//            printf("m3: %s", buf);
//        } else {
//            debug("main: sfd timeout\n");
//        }
//    }
//    info("m3 detected!\n");


//    sleep(5);


    int attempts = 0;

    //printf ("argc: %d, argv: %s| %s %s", argc, argv[0], argv[1], argv[2]);


    FILE *fi;
    fi = fopen(HOSTNAME_FILE, "r");
    getline(&fileline, &linelen, fi);
    //printf("%s", fileline);

    char *tok;
    if (linelen) tok = strtok(fileline, "-");
    while (tok) {
        //printf("%s", tok);
        nodeidstr = tok;
        tok = strtok(NULL, "-");
    }
    INFO("detected node id: %s", nodeidstr);
    if (nodeidstr) {
        hw_addr[0] = (uint8_t) atoi(nodeidstr) % 255;
        hw_addr[1] = (uint8_t) atoi(nodeidstr) / 255;
    } else if (argc > 2) {
        hw_addr[0] = (uint8_t) atoi(argv[1]);
        hw_addr[1] = (uint8_t) atoi(argv[2]);
    } // uses the args as address



//    // build init msg
//    sprintf(msg, "messg%c%c%c", M3_INIT, hw_addr[1], hw_addr[0]);
//
//    while (msg_typ != M3_ACK && attempts++ < MAX_ATTEMPTS) {
//        memset(buf, 0, 100);
//
//        // SEND INIT COMMAND
//        if (write(sfd, msg, 8) < 0) {     // send 5 character greetings + init command + HW addr
//            error("main: sfd send error");
//            continue;
//        }
//        printf("---> A8: (1) %s\n", msg);
//        printf("  (");
//        print_bytes_str(msg, 8, " ");
//        printf(")\n");
//
//        n = __m3_reads(sfd, buf, MAX_LINE);
//        skip = 1;
//        // SKIP DEBUG COMMENTS
//        while (skip){
//
//            n = __m3_reads(sfd, buf, MAX_LINE);
//            if (!n) {
//                debug("sfd timeout");
//                continue;
//            }
//
//            msg_typ = buf[0];
//
//            switch (msg_typ) {
//                case M3_ACK:
//                    skip = 0;
//                    info("[M3 ACK]");
//                    break;
//                case M3_ERR:
//                    skip = 0;
//                    info("[M3 ERR]");
//                    break;
//                case M3_RECV:
//                    skip = 0;
//                    info("[M3 RECV]");
//                    break;
//                default:
//                    info("[M3 DEBUG]");
//                    break;
//            }
//            printf("<--- M3[%d]: %s", n, buf);
//#ifdef VERBOSE
//            printf("  (");
//            print_bytes_str(buf,n," ");
//            printf(")\n");
//#endif
//        }
//
//
//
//        sleep(3);
//    }
//
//
//    if (attempts>MAX_ATTEMPTS) exit(-1);

//
//
//    // build send command
//    sprintf(msg, "messg%c", M3_SEND);
//
//    while (msg_typ!=M3_ACK && attempts++<MAX_ATTEMPTS) {
//        memset(buf, 0, 100);
//
//        // SEND SEND COMMAND
//        if (write(sfd, msg, 6) < 0) {     // send 5 character greetings + init command + HW addr
//            error("main: sfd send error");
//        }
//        printf("---> A8: (1) %s\n", msg);
//        printf("  (");
//        print_bytes_str(msg, 8, " ");
//        printf(")\n");
//
//        n = __m3_reads(sfd, buf, MAX_LINE);
//        skip = 1;
//        // SKIP DEBUG COMMENTS
//        while (skip){
//
//            n = __m3_reads(sfd, buf, MAX_LINE);
//            if (!n) {
//                debug("sfd timeout");
//                continue;
//            }
//
//            msg_typ = buf[0];
//
//            switch (msg_typ) {
//                case M3_ACK:
//                    skip = 0;
//                    info("[M3 ACK]");
//                    break;
//                case M3_ERR:
//                    skip = 0;
//                    info("[M3 ERR]");
//                    break;
//                case M3_RECV:
//                    skip = 0;
//                    info("[M3 RECV]");
//                    break;
//                default:
//                    info("[M3 DEBUG]");
//                    break;
//            }
//            printf("<--- M3[%d]: %s", n, buf);
//#ifdef VERBOSE
//            printf("  (");
//            print_bytes_str(buf,n," ");
//            printf(")\n");
//#endif
//        }
//
//        sleep(3);
//
//    }


    INFO("main: STARTING M3 RADIO with hw_addr: %d %d", hw_addr[0], hw_addr[1]);
    if (serial_send(sfd, buf, MAX_SER_MSG_LEN, M3_INIT, hw_addr, IEEE802154_SHORT_ADDRESS_LEN) < 0) {
        ERROR("main: cannot send M3_INIT");
        exit(-1);
    }


    INFO("main: STARTING SENDER...");
    sprintf(msg, "");
    if (serial_send(sfd, buf, MAX_SER_MSG_LEN, M3_SEND, NULL, 0) < 0) {
        ERROR("main: cannot send M3_SEND");
        exit(-1);
    }


    INFO("main: LISTENING...");
    while (1) {
        // LISTENING
        n = serial_recv(sfd, buf, MAX_SER_MSG_LEN, &msg_typ);
    }




    return 0;
}
