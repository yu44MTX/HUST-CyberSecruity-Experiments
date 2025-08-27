#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <linux/netlink.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h> 
#include <sys/stat.h>
#include <fcntl.h> 
#include <errno.h>

#define NETLINK_TEST	17
#define MSG_LEN	256000

typedef struct rule_list
{
	unsigned int src_ip;
	unsigned int src_port;
	unsigned int des_ip;
	unsigned int des_port;
	unsigned int type;
    unsigned int rule;
	unsigned int log;
}Rule_list;


char *default_data = "Netlink Test Default Data";

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

unsigned int ipTint(char *ipstr){
    if (ipstr == NULL) return 0;
    if (strcmp(ipstr,"*") == 0 || strcmp(ipstr,"ALL") == 0 || strcmp(ipstr,"all") == 0) return 0xffffffff;
    char *token;
    unsigned int i = 0, total = 0, cur;
    token = strtok(ipstr, ".");
    while (token != NULL) {
        cur = atoi(token);
        if (cur >= 0 && cur <= 255) {
            total += cur << (8 * i);
        }
        i++;
        token = strtok(NULL, ".");
    }
    return total;
}

void ipTchar(unsigned int ipInt, char *ipChar)
{
    sprintf(ipChar,"%d.%d.%d.%d",
        ipInt&0xff,
        (ipInt>>8)&0xff,
        (ipInt>>16)&0xff,
        (ipInt>>24)&0xff
    );
}

unsigned int charTint(char *str){
    if (str == NULL) return 0;
    if (strcmp(str,"*") == 0 || strcmp(str,"ALL") == 0 || strcmp(str,"all") == 0) return 0xffffffff;
    char *p = str;
    unsigned int result = 0;
    while(*p <= '9' && *p >= '0'){
        result = result*10 + (*p) - '0';
        p++;
    }
    return result;
}


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

	//printf("message sendto kernel, content: '%s', len: %d\n", (char *) NLMSG_DATA(message), message->nlmsg_len);
	ret = sendto(skfd, message, message->nlmsg_len, 0, (struct sockaddr *) &kpeer, sizeof(kpeer));
	if (!ret) {
		perror("sendto:");
		exit(-1);
	}
    return ret;
}

/*获取文件行数*/
unsigned int file_wc(const char *filename)
{
    int fd;
    register unsigned int linect = 0;
    char buf[50000];
    int len;
    char *p = NULL;
    if(filename) {
        if((fd = open(filename, O_RDONLY)) < 0) {
            fprintf(stderr, "open %s\n",strerror(errno));
            return -1;
        }
 
        while(1) {
            if((len = read(fd,buf,50000)) == -1) {
                return -1;
            }
            if(len == 0){
                break;
            }
            for( p = buf; len--; ){
                if( *p == '\n' ){
                   ++linect;
                }
                ++p;
            }
        }
    }
    return linect;
}

int main(int argc, char *argv[])
{
	char *cmd;
    char *data;

    if(argc!=3) return 0;

    netlink_init();

    cmd = argv[1];
    data = argv[2];

    if(strcmp(cmd,"ls") == 0){
        if(strcmp(data,"rule") == 0){
            FILE *file = fopen("./rule", "r");
            if(file == NULL){
                printf("read file error!\n");
                return 0;
            }

            printf("+----------------+------+----------------+------+----------+------+-----+\n");
            printf("|     Src IP     | Port |     Dst IP     | Port | Protocol | Rule | Log |\n");
            printf("+----------------+------+----------------+------+----------+------+-----+\n");

            unsigned int list_sum = file_wc("./rule");
            for(int i=1; i<=list_sum ;i++){
                char src_ip[30];
                char src_port[30];
                char dest_ip[30];
                char dest_port[30];
                char protocol[30];
                char rule[30];
                unsigned int log;
                fscanf(file, "%s\t%s\t%s\t%s\t%s\t%s\t%u",
                    src_ip,
                    src_port,
                    dest_ip,
                    dest_port,
                    protocol,
                    rule,
                    &log
                );

                printf("|%16s|%6s|%16s|%6s|%10s|%6s|%5u|\n",
                    src_ip,
                    src_port,
                    dest_ip,
                    dest_port,
                    protocol,
                    rule,
                    log
                );
                printf("+----------------+------+----------------+------+----------+------+-----+\n");
            }
            fclose(file);
        }
        else if(strcmp(data,"connect") == 0){
            char data[200];
            ((unsigned int *)data)[0] = 0x02020202;
            ((unsigned int *)data)[1] = 0;
            netlink_send(data, 4);

            struct u_packet_info info;
            ret = recvfrom(skfd, &info, sizeof(struct u_packet_info), 0, (struct sockaddr *) &kpeer, &kpeerlen);
            if (!ret) {
                perror("recvfrom:");
                exit(-1);
            }
            unsigned int list_sum = *(unsigned int *)info.msg;
            unsigned int *p = ((unsigned int *)info.msg) + 1;

            printf("+----------------+------+----------------+------+----------+-----+\n");
            printf("|     Src IP     | Port |     Dst IP     | Port | Protocol | Log |\n");
            printf("+----------------+------+----------------+------+----------+-----+\n");

            for(int i=1; i<=list_sum ;i++){
                char src_ip[30];
                unsigned int src_port;
                char dest_ip[30];
                unsigned int dest_port;
                char protocol[30];
                unsigned int log;
                
                ipTchar(p[0], src_ip);
                src_port = p[1];
                ipTchar(p[2], dest_ip);
                dest_port = p[3];

                if(p[4] == 1) sprintf(protocol, "ICMP");
                else if(p[4] == 6) sprintf(protocol, "TCP");
                else if(p[4] == 17) sprintf(protocol, "UDP");
                else sprintf(protocol, "UNKNOWN");

                log = p[5];

                printf("|%16s|%6u|%16s|%6u|%10s|%5u|\n",
                    src_ip,
                    src_port,
                    dest_ip,
                    dest_port,
                    protocol,
                    log
                );
                printf("+----------------+------+----------------+------+----------+-----+\n");

                p += 6;
            }
        }
        else if(strcmp(data,"nat") == 0){
            FILE *file = fopen("./NAT", "r");
            if(file == NULL){
                printf("read file error!\n");
                return 0;
            }

            printf("+----------------+----------+----------------+----------+\n");
            printf("|     LAN IP     | LAN Port |     WAN IP     | WAN Port |\n");
            printf("+----------------+----------+----------------+----------+\n");

            unsigned int list_sum = file_wc("./NAT");
            for(int i=1; i<=list_sum ;i++){
                char src_ip[30];
                char src_port[30];
                char dest_ip[30];
                char dest_port[30];
                fscanf(file, "%s\t%s\t%s\t%s",
                    src_ip,
                    src_port,
                    dest_ip,
                    dest_port
                );

                printf("|%16s|%10s|%16s|%10s|\n",
                    src_ip,
                    src_port,
                    dest_ip,
                    dest_port
                );
                printf("+----------------+----------+----------------+----------+\n");
            }
            fclose(file);
        }
        else if(strcmp(data,"natc") == 0){
            char data[200];
            ((unsigned int *)data)[0] = 0x05050505;
            ((unsigned int *)data)[1] = 0;
            netlink_send(data, 4);

            struct u_packet_info info;
            ret = recvfrom(skfd, &info, sizeof(struct u_packet_info), 0, (struct sockaddr *) &kpeer, &kpeerlen);
            if (!ret) {
                perror("recvfrom:");
                exit(-1);
            }
            unsigned int list_sum = *(unsigned int *)info.msg;
            unsigned int *p = ((unsigned int *)info.msg) + 1;

            printf("+----------------+------+----------------+------+----------+\n");
            printf("|     LAN IP     | Port |     WAN IP     | Port | Protocol |\n");
            printf("+----------------+------+----------------+------+----------+\n");

            for(int i=1; i<=list_sum ;i++){
                char src_ip[30];
                unsigned int src_port;
                char dest_ip[30];
                unsigned int dest_port;
                char protocol[30];
                
                ipTchar(p[0], src_ip);
                src_port = p[1];
                ipTchar(p[2], dest_ip);
                dest_port = p[3];

                if(p[4] == 1) sprintf(protocol, "ICMP");
                else if(p[4] == 6) sprintf(protocol, "TCP");
                else if(p[4] == 17) sprintf(protocol, "UDP");
                else sprintf(protocol, "UNKNOWN");

                printf("|%16s|%6u|%16s|%6u|%10s|\n",
                    src_ip,
                    src_port,
                    dest_ip,
                    dest_port,
                    protocol
                );
                printf("+----------------+------+----------------+------+----------+\n");

                p += 5;
            }
        }
    }
    else if(strcmp(cmd,"ch") == 0){
        if(strcmp(data,"rule") == 0){
            FILE *file = fopen("./rule", "r");
            if(file == NULL){
                printf("read file error!\n");
                return 0;
            }

            printf("+----------------+------+----------------+------+----------+------+-----+\n");
            printf("|     Src IP     | Port |     Dst IP     | Port | Protocol | Rule | Log |\n");
            printf("+----------------+------+----------------+------+----------+------+-----+\n");

            unsigned int list_sum = file_wc("./rule");
            char data[70000];
            ((unsigned int *)data)[0] = 0x01010101; 
            ((unsigned int *)data)[1] = list_sum; 

            for(int i=1; i<=list_sum ;i++){
                char src_ip[30];
                char src_port[30];
                char dest_ip[30];
                char dest_port[30];
                char protocol[30];
                char rule[30];
                unsigned int log;
                fscanf(file, "%s\t%s\t%s\t%s\t%s\t%s\t%u",
                    src_ip,
                    src_port,
                    dest_ip,
                    dest_port,
                    protocol,
                    rule,
                    &log
                );

                printf("|%16s|%6s|%16s|%6s|%10s|%6s|%5u|\n",
                    src_ip,
                    src_port,
                    dest_ip,
                    dest_port,
                    protocol,
                    rule,
                    log
                );
                printf("+----------------+------+----------------+------+----------+------+-----+\n");

                ((unsigned int *)data)[(i-1)*7 + 2] = ipTint(src_ip);
                ((unsigned int *)data)[(i-1)*7 + 3] = charTint(src_port);
                ((unsigned int *)data)[(i-1)*7 + 4] = ipTint(dest_ip);
                ((unsigned int *)data)[(i-1)*7 + 5] = charTint(dest_port);

                if(strcmp(protocol,"TCP") == 0 || strcmp(protocol,"tcp") == 0) ((unsigned int *)data)[(i-1)*7 + 6] = 6;
                else if(strcmp(protocol,"UDP") == 0 || strcmp(protocol,"udp") == 0) ((unsigned int *)data)[(i-1)*7 + 6] = 17;
                else if(strcmp(protocol,"ICMP") == 0 || strcmp(protocol,"icmp") == 0) ((unsigned int *)data)[(i-1)*7 + 6] = 1;
                else ((unsigned int *)data)[(i-1)*7 + 6] = 0xffffffff;

                if(strcmp(rule,"ALLOW") == 0 || strcmp(rule,"allow") == 0 || strcmp(rule,"all") == 0) ((unsigned int *)data)[(i-1)*7 + 7] = 1;
                else ((unsigned int *)data)[(i-1)*7 + 7] = 0;

                ((unsigned int *)data)[(i-1)*7 + 8] = log;
            }
            fclose(file);
            ((unsigned int *)data)[(list_sum-1)*7 + 9] = 0;
            netlink_send(data, ((list_sum-1)*7 + 9)<<2);
        }
        else if(strcmp(data,"nat") == 0){
            FILE *file = fopen("./NAT", "r");
            if(file == NULL){
                printf("read file error!\n");
                return 0;
            }

            printf("+----------------+----------------+\n");
            printf("|     LAN IP     |     WAN IP     |\n");
            printf("+----------------+----------------+\n");

            unsigned int list_sum = file_wc("./NAT");
            char data[70000];
            ((unsigned int *)data)[0] = 0x04040404; 
            ((unsigned int *)data)[1] = list_sum;
            for(int i=1; i<=list_sum ;i++){
                char src_ip[30];
                char dest_ip[30];
                fscanf(file, "%s\t%s",
                    src_ip,
                    dest_ip
                );

                printf("|%16s|%16s|\n",
                    src_ip,
                    dest_ip
                );
                printf("+----------------+----------------+\n");

                ((unsigned int *)data)[(i-1)*2 + 2] = ipTint(src_ip);
                ((unsigned int *)data)[(i-1)*2 + 3] = ipTint(dest_ip);
            }
            fclose(file);
            ((unsigned int *)data)[(list_sum-1)*2 + 4] = 0;
            netlink_send(data, ((list_sum-1)*2 + 4)<<2);
        }
    }
    else if(strcmp(cmd,"add") == 0){
        if(strcmp(data,"rule") == 0){
            FILE *file = fopen("./rule", "a");
            if(file == NULL){
                printf("read file error!\n");
                return 0;
            }
            char src_ip[30];
            char src_port[30];
            char dest_ip[30];
            char dest_port[30];
            char protocol[30];
            char rule[30];
            unsigned int log;
            scanf("%s\t%s\t%s\t%s\t%s\t%s\t%u",
                src_ip,
                src_port,
                dest_ip,
                dest_port,
                protocol,
                rule,
                &log
            );

            fprintf(file, "%s\t%s\t%s\t%s\t%s\t%s\t%u\n",
                src_ip,
                src_port,
                dest_ip,
                dest_port,
                protocol,
                rule,
                log
            );

            fclose(file);
        }
        else if(strcmp(data,"nat") == 0){
            FILE *file = fopen("./NAT", "a");
            if(file == NULL){
                printf("read file error!\n");
                return 0;
            }
            char src_ip[30];
            char src_port[30];
            char dest_ip[30];
            char dest_port[30];
            scanf("%s\t%s",
                src_ip,
                dest_ip
            );

            fprintf(file, "%s\t%s\n",
                src_ip,
                dest_ip
            );

            fclose(file);
        }
    }
    else if(strcmp(cmd,"start") == 0){
        system("sudo insmod ../myfw.ko");
        system("./UI ch rule");
        system("./UI ch nat");
    }
    else if(strcmp(cmd,"exit") == 0){
        close(skfd);
        system("sudo rmmod myfw.ko");
        return 0;
    }
    close(skfd);
    return 0;    
}