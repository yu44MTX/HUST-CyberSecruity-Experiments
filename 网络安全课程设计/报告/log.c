#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <linux/netlink.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>


#define NETLINK_TEST	17
#define MSG_LEN	256000

struct sockaddr_nl local;
struct sockaddr_nl kpeer;
int skfd, ret, kpeerlen = sizeof(struct sockaddr_nl);

struct msg_to_kernel {
	struct nlmsghdr hdr;
	char data[MSG_LEN];
};

struct u_packet_info {
	struct nlmsghdr hdr;
	char msg[MSG_LEN];
};

int netlink_init(){
    skfd = socket(PF_NETLINK, SOCK_RAW, NETLINK_TEST);
	if (skfd < 0) {
		printf("can not create a netlink socket\n");
		return -1;
	}

	memset(&local, 0, sizeof(local));
	local.nl_family = AF_NETLINK;
	local.nl_pid = getpid();
	local.nl_groups = 0;
	if (bind(skfd, (struct sockaddr *) &local, sizeof(local)) != 0) {
		printf("bind() error\n");
		return -1;
	}
	memset(&kpeer, 0, sizeof(kpeer));
	kpeer.nl_family = AF_NETLINK;
	kpeer.nl_pid = 0;
	kpeer.nl_groups = 0;
    return 0;
}

int netlink_send(char *data, unsigned int len){
    struct nlmsghdr *message;
    int dlen;

    dlen = len + 1;
    message = (struct nlmsghdr *) malloc(sizeof(struct msg_to_kernel));
	if (message == NULL) {
		printf("malloc() error\n");
		return -1;
	}

	memset(message, '\0', sizeof(struct nlmsghdr));
	message->nlmsg_len = NLMSG_SPACE(dlen);
	message->nlmsg_flags = 0;
	message->nlmsg_type = 0;
	message->nlmsg_seq = 0;
	message->nlmsg_pid = local.nl_pid;

	memcpy(NLMSG_DATA(message), data, len);

	ret = sendto(skfd, message, message->nlmsg_len, 0, (struct sockaddr *) &kpeer, sizeof(kpeer));
	if (!ret) {
		perror("sendto:");
		exit(-1);
	}
    return ret;
}

int main(){

    netlink_init();
    char data[200];
    ((unsigned int *)data)[0] = 0x03030303;
    ((unsigned int *)data)[1] = 0;
    netlink_send(data, 4);

    FILE *file = fopen("./log.txt", "a");
    if(file == NULL){
        printf("read file error!\n");
        return 0;
    }

    while(1){
        char log_line[1000];
        struct u_packet_info info;
        ret = recvfrom(skfd, &info, sizeof(struct u_packet_info), 0, (struct sockaddr *) &kpeer, &kpeerlen);
        if (!ret) {
            perror("recvfrom:");
            exit(-1);
        }

        time_t current_time;
        struct tm *local_time;
    
        time(&current_time);
        local_time = localtime(&current_time);
    
        int year = local_time->tm_year + 1900;
        int month = local_time->tm_mon + 1;
        int day = local_time->tm_mday;
        int hour = local_time->tm_hour;
        int minute = local_time->tm_min;
        int second = local_time->tm_sec;

        sprintf(log_line, "[%d-%02d-%02d %02d:%02d:%02d] %s\n",
            year,
            month,
            day,
            hour,
            minute,
            second,
            info.msg
        );
        
        printf("%s",log_line);

        fprintf(file,"%s",log_line);
    }
    fclose(file);
}