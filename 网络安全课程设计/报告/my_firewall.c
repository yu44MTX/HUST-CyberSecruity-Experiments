#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/skbuff.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/icmp.h>
#include <linux/netlink.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <net/sock.h>
#include <net/inet_hashtables.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/ktime.h>
#include <linux/random.h>
#include <net/udp.h>
#include <net/tcp.h>
#include <linux/inet.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("wmz");

#define NETLINK_TEST	17
#define CONNECT_STATUE_TIME	10
#define NAT_CONNECT_STATUE_TIME 100

static struct sock *nlsk = NULL;
unsigned int LOG_pid = 0;


static struct nf_hook_ops fw_in;
static struct nf_hook_ops fw_out;
static struct nf_hook_ops nat_in;
static struct nf_hook_ops nat_out;

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
Rule_list rule_list[10000];
unsigned int rule_list_sum=0;

typedef struct connect_list
{
	unsigned int src_ip;
	unsigned int src_port;
	unsigned int des_ip;
	unsigned int des_port;
	unsigned int type;
	unsigned int log;
	unsigned long long time;
	struct connect_list *next;
	struct connect_list *last;
} Connect_list;
//根据源目ip的后两组异或后建立hash表
Connect_list *Connect_index[66000];

typedef struct nat_list
{
	unsigned int inner_ip;
	unsigned int outside_ip;
}NAT_list;
NAT_list nat_list[10000];
unsigned int nat_list_sum=0;

typedef struct nat_connect_list
{
	unsigned int inner_ip;
	unsigned int inner_port;
	unsigned int outside_ip;
	unsigned int outside_port;
	unsigned int protocol;
	unsigned int id;
	unsigned long long time;
	struct socket *sock;
	struct nat_connect_list *next;
	struct nat_connect_list *last;
}NAT_connect_list;
//根据源目ip的后两组异或后建立hash表
NAT_connect_list *NAT_connect_index[66000];


int nltest_ksend(char *reply, int pid, unsigned int rlen);

/*
	打印日志
*/
void print_log(int src_ip, int src_port, int des_ip, int des_port, int protocol, int rule){
	char log_data[300];
	sprintf(log_data,"[%s] %d.%d.%d.%d:%d -> %d.%d.%d.%d:%d [%s]",
		protocol==IPPROTO_TCP?"TCP":protocol==IPPROTO_UDP?"UDP":protocol==IPPROTO_ICMP?"ICMP":"UNKONWN",
		src_ip&0xff,
		(src_ip>>8)&0xff,
		(src_ip>>16)&0xff,
		(src_ip>>24)&0xff,
		src_port,
		des_ip&0xff,
		(des_ip>>8)&0xff,
		(des_ip>>16)&0xff,
		(des_ip>>24)&0xff,
		des_port,
		rule?"ACCEPT":"DROP"
	);
	nltest_ksend(log_data, LOG_pid, strlen(log_data));
}

/*
	获取秒级时间戳
*/
unsigned long long get_second(void) {
    time64_t seconds;
    // 获取秒级别的实时时间戳
    seconds = ktime_get_real_seconds();
    return (unsigned long long)seconds;
}

/*
	删除连接状态
*/
void delete_connect(unsigned int connect_index, Connect_list *index){
	if(index->next != NULL){
		index->next->last = index->last;
	}
	if(index->last != NULL){
		index->last->next = index->next;
	}
	else{
		Connect_index[connect_index] = NULL;
	}
	kfree(index);
}

/*
	连接匹配程序
*/
bool connect_match(struct sk_buff *skb){
	struct iphdr *iph;
	iph = ip_hdr(skb);
	unsigned int type = iph->protocol;
	unsigned int index1 = iph->daddr;
	unsigned int index2 = iph->saddr;
	unsigned int saddr = iph->saddr;
	unsigned int daddr = iph->daddr;

	index1 >>= 16;
    (index1) &= 0xffff;
	index2 >>= 16;
    (index2) &= 0xffff;

	Connect_list *p = Connect_index[index1^index2];


	while(p != NULL){
		if(p->time < get_second()){
			Connect_list *q = p->next;
			delete_connect(index1^index2, p);
			if(q == NULL) break;
			p = q;
		} 
		if(type == p->type){
			if(type == IPPROTO_TCP || type == IPPROTO_UDP){
				unsigned int sport = 0;
				unsigned int dport = 0;
				
				if(type == IPPROTO_TCP){
					struct tcphdr *tcph;
					tcph = (void *)iph + iph->ihl*4;
					sport = ntohs(tcph->source);
					dport = ntohs(tcph->dest);
				}
				else{
					struct udphdr *udph;
					udph = (void *)iph + iph->ihl*4;
					sport = ntohs(udph->source);
					dport = ntohs(udph->dest);
				}

                printk("connect_match Get a package from %d.%d.%d.%d:%d to %d.%d.%d.%d:%d type:%d\n",
                    ((unsigned char *)&iph->saddr)[0],
                    ((unsigned char *)&iph->saddr)[1],
                    ((unsigned char *)&iph->saddr)[2],
                    ((unsigned char *)&iph->saddr)[3],
                    sport,
                    ((unsigned char *)&iph->daddr)[0],
                    ((unsigned char *)&iph->daddr)[1],
                    ((unsigned char *)&iph->daddr)[2],
                    ((unsigned char *)&iph->daddr)[3],
                    dport,
                    iph->protocol
                );
                
                printk("rule from %d.%d.%d.%d:%d to %d.%d.%d.%d:%d type:%d\n",
                    ((unsigned char *)&p->src_ip)[0],
                    ((unsigned char *)&p->src_ip)[1],
                    ((unsigned char *)&p->src_ip)[2],
                    ((unsigned char *)&p->src_ip)[3],
                    p->src_port,
                    ((unsigned char *)&p->des_ip)[0],
                    ((unsigned char *)&p->des_ip)[1],
                    ((unsigned char *)&p->des_ip)[2],
                    ((unsigned char *)&p->des_ip)[3],
                    p->des_port,
                    p->type
                );


				if(saddr == p->src_ip && daddr == p->des_ip && sport == p->src_port && dport == p->des_port){
					p->time = get_second() + CONNECT_STATUE_TIME;
					if(p->log){
						print_log(saddr, sport, daddr, dport, type, 1);
					}
					return 1;
				}	
				else if(daddr == p->src_ip && saddr == p->des_ip && dport == p->src_port && sport == p->des_port){
					p->time = get_second() + CONNECT_STATUE_TIME;
					if(p->log){
						print_log(saddr, sport, daddr, dport, type, 1);
					}
					return 1;
				}
			}
			else if(type == IPPROTO_ICMP){
				struct icmphdr *icmph;
				icmph = (void *)iph + iph->ihl*4;

				if(daddr == p->des_ip && saddr == p->src_ip){
					delete_connect(index1^index2, p);
					if(p->log){
						print_log(saddr, 0, daddr, 0, type, 1);	
					}
					return 1;
				}
			}
		}
		p = p->next;
	}
	return 0;
}

/*
	清空连接状态
*/
void mepty_connect(void){
	int i = 0;
	for(;i<65536;i++)
		Connect_index[i] = NULL;
}

/*
	规则匹配程序
*/
bool rule_match(struct sk_buff *skb){
	struct iphdr *iph;
	iph = ip_hdr(skb);
	unsigned int type = iph->protocol;
	unsigned int saddr = iph->saddr;
	unsigned int daddr = iph->daddr;

	unsigned int i = 1;
	while(i <= rule_list_sum){
		if((rule_list[i].type == type || rule_list[i].type == 0xffffffff) && (rule_list[i].des_ip == daddr || rule_list[i].des_ip == 0xffffffff) && (rule_list[i].src_ip == saddr || rule_list[i].src_ip == 0xffffffff)){
			if(type == IPPROTO_TCP || type == IPPROTO_UDP){
				unsigned int sport = 0;
				unsigned int dport = 0;
				
				if(type == IPPROTO_TCP){
					struct tcphdr *tcph;
					tcph = (void *)iph + iph->ihl*4;
					sport = ntohs(tcph->source);
					dport = ntohs(tcph->dest);
				}
				else{
					struct udphdr *udph;
					udph = (void *)iph + iph->ihl*4;
					sport = ntohs(udph->source);
					dport = ntohs(udph->dest);
				}
				
				if((rule_list[i].src_port == sport || rule_list[i].src_port == 0xffffffff) && (rule_list[i].des_port == dport || rule_list[i].des_port == 0xffffffff)){
					if(rule_list[i].rule){
						Connect_list * new_connect1 = (Connect_list*)kmalloc(sizeof(Connect_list),GFP_KERNEL);
						new_connect1->des_ip = daddr;
						new_connect1->src_ip = saddr;
						new_connect1->des_port = dport;
						new_connect1->src_port = sport;
						new_connect1->type = type;
						new_connect1->log = rule_list[i].log;
						new_connect1->time = get_second() + CONNECT_STATUE_TIME;
                        new_connect1->last = NULL;
                        new_connect1->next = NULL;
					
						unsigned int index1 = ((new_connect1->des_ip)>>16) & 0xffff;
						unsigned int index2 = ((new_connect1->src_ip)>>16) & 0xffff;

						Connect_list * p = Connect_index[index1^index2];
						if(p == NULL){
							Connect_index[index1^index2] = new_connect1;
						}
						else {
							while(p->next != NULL) p = p->next;
							p->next = new_connect1;
							new_connect1->next = NULL;
							new_connect1->last = p;
						}

						if(rule_list[i].log){
							print_log(saddr, sport, daddr, dport, type, 1);	
						}
						return 1;
					}
					else{
						if(rule_list[i].log){
							print_log(saddr, sport, daddr, dport, type, 0);	
						}
						return 0;
					}
				}
			}
			else if(type == IPPROTO_ICMP){
				struct icmphdr *icmph;
				icmph = (void *)iph + iph->ihl*4;

				if(rule_list[i].rule){
                    
					Connect_list * new_connect1 = (Connect_list*)kmalloc(sizeof(Connect_list),GFP_KERNEL);
					new_connect1->des_ip = saddr;
					new_connect1->src_ip = daddr;
					new_connect1->type = IPPROTO_ICMP;
					new_connect1->log = rule_list[i].log;
					new_connect1->time = get_second() + CONNECT_STATUE_TIME;
					new_connect1->last = NULL;
                    new_connect1->next = NULL;

					unsigned int index1 = ((new_connect1->des_ip)>>16) & 0xffff;
					unsigned int index2 = ((new_connect1->src_ip)>>16) & 0xffff;

					Connect_list * p = Connect_index[index1^index2];
					if(p == NULL){
						Connect_index[index1^index2] = new_connect1;
					}
					else {
						while(p->next != NULL) p = p->next;
						p->next = new_connect1;
						new_connect1->last = p;
					}
					if(rule_list[i].log){
						print_log(saddr, 0, daddr, 0, type, 1);
					}
					return 1;
				}
				else{
					if(rule_list[i].log){
						print_log(saddr, 0, daddr, 0, type, 0);
					}
				}	
			}
		}
		i++;
	}
	return 0;
}

/*
	防火墙实现程序
*/
unsigned int telnet_filter(void *priv,
			struct sk_buff *skb,
			const struct nf_hook_state *state)
{
	struct iphdr *iph;
	iph = ip_hdr(skb);
	
	printk("Get a package from %d.%d.%d.%d to %d.%d.%d.%d type:%d\n",
		((unsigned char *)&iph->saddr)[0],
		((unsigned char *)&iph->saddr)[1],
		((unsigned char *)&iph->saddr)[2],
		((unsigned char *)&iph->saddr)[3],
		((unsigned char *)&iph->daddr)[0],
		((unsigned char *)&iph->daddr)[1],
		((unsigned char *)&iph->daddr)[2],
		((unsigned char *)&iph->daddr)[3],
		iph->protocol
	);

	if(connect_match(skb)){
		printk("connect exist! permit!\n");
		return NF_ACCEPT;
	}
	
	if(rule_match(skb)){
		printk("rule exist! permit!\n");
		return NF_ACCEPT;
	}

	printk("forbid!\n");
	return NF_DROP;
}


// 计算伪头校验和
static __inline__ __wsum csum_pseudo_header(struct sk_buff *skb, int proto, int len) {
    struct iphdr *iph = ip_hdr(skb); // 获取 IP 头
    return csum_tcpudp_magic(iph->saddr, iph->daddr, len, proto, 0);
}

// 更新 IP 头校验和
void update_ip_checksum(struct sk_buff *skb) {
    struct iphdr *iph = ip_hdr(skb);  // 获取 IP 头

    // 校验和字段清零
    iph->check = 0;

    // 计算新的 IP 头校验和
    iph->check = ip_fast_csum((u8 *)iph, iph->ihl);  // `iph->ihl` 是 IP 头的长度，以 32 位字（4 字节）为单位
}

// 更新 UDP 校验和
void update_udp_checksum(struct sk_buff *skb) {
    struct udphdr *udph = udp_hdr(skb);  // 获取 UDP 头
    int udp_len = ntohs(udph->len);      // UDP 长度字段

    // 清除原有校验和
    udph->check = 0;

    // 计算伪头校验和
    __wsum udp_sum = csum_partial(udph, udp_len, 0);
    udp_sum = csum_pseudo_header(skb, IPPROTO_UDP, udp_len);

    // 填充 UDP 校验和
    udph->check = (udp_sum == 0) ? CSUM_MANGLED_0 : htons(~udp_sum);
	udph->check = 0;
}

// 更新 TCP 校验和
void update_tcp_checksum(struct sk_buff *skb) {
    struct tcphdr *tcph = tcp_hdr(skb);  // 获取 TCP 头
    int tcp_len = skb->len - ip_hdrlen(skb); // 计算 TCP 数据长度

    // 清除原有校验和
    tcph->check = 0;

    // 计算伪头校验和
    __wsum tcp_sum = csum_partial(tcph, tcp_len, 0);
    tcp_sum = csum_pseudo_header(skb, IPPROTO_TCP, tcp_len);

    // 填充 TCP 校验和
    tcph->check = htons(~tcp_sum);
}

// 更新 IP, TCP 和 UDP 校验和的通用函数
void update_checksum(struct sk_buff *skb) {
    struct iphdr *iph = ip_hdr(skb);  // 获取 IP 头

    // 更新 IP 头校验和
    update_ip_checksum(skb);

    switch (iph->protocol) {
        case IPPROTO_UDP:
            update_udp_checksum(skb);  // 更新 UDP 校验和
            break;
        case IPPROTO_TCP:
            update_tcp_checksum(skb);  // 更新 TCP 校验和
            break;
        default:
            // 其他协议不处理
            break;
    }
}

/*
	产生一个随机的端口号
*/
static unsigned int generate_random_port(unsigned int protocol, NAT_connect_list *p) {
    unsigned int rand;

	while(1){
		// 使用 get_random_u32 生成随机数
		rand = get_random_u32() % (40000 - 20000 + 1); // 取模得到范围内的数
		rand += 20000; // 加上最小值，得到最终的范围

		struct socket *sock;
		int port = rand;  // 你想监听的端口号
		int err;

		if(protocol == IPPROTO_UDP) err = sock_create_kern(&init_net, AF_INET, SOCK_DGRAM, protocol, &sock);
		else err = sock_create_kern(&init_net, AF_INET, SOCK_STREAM, protocol, &sock);
		if (err < 0) {
			printk(KERN_ERR "Error creating socket: %d\n", err);
			continue;
		}

		struct sockaddr_in server_addr;
		memset(&server_addr, 0, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = p->outside_ip;  // 绑定到所有可用接口
		server_addr.sin_port = htons(port);  // 绑定到指定的端口

		err = kernel_bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
		if (err < 0) {
			printk(KERN_ERR "Error binding socket: %d\n", err);
			sock_release(sock);
			continue;
		}
		printk(KERN_ERR "binding port: %d\n", rand);
		p->sock = sock;
		break;
	}
    return rand;
}

/*
	删除NAT状态
*/
void delete_nat_connect(unsigned int connect_index, NAT_connect_list *index){
	if(index->next != NULL){
		index->next->last = index->last;
	}
	if(index->last != NULL){
		index->last->next = index->next;
	}
	else{
		NAT_connect_index[connect_index] = NULL;
	}
	if(index->sock != NULL) sock_release(index->sock);
	kfree(index);
}

/*
	清空NAT连接状态
*/
void mepty_nat_connect(void){
	int i = 0;
	for(;i<65536;i++)
		NAT_connect_index[i] = NULL;
}

/*
	NAT发包转换实现程序
*/
unsigned int telnet_NAT_out(void *priv,
			struct sk_buff *skb,
			const struct nf_hook_state *state)
{
	struct iphdr *iph;
	iph = ip_hdr(skb);
	unsigned int type = iph->protocol;
	unsigned int saddr = iph->saddr;
	unsigned int daddr = iph->daddr;
				
	if(type == IPPROTO_TCP){
		unsigned int sport = 0;
		unsigned int dport = 0;
		struct tcphdr *tcph;
		tcph = (void *)iph + iph->ihl*4;
		sport = ntohs(tcph->source);
		dport = ntohs(tcph->dest);

		NAT_connect_list *p = NAT_connect_index[((daddr>>16)&0xffff)^dport];
		while(p != NULL){
			if(p->time < get_second()){
				NAT_connect_list * n = p->next;
				delete_nat_connect(((daddr>>16)&0xffff)^dport, p);
				if(n == NULL) break;
				p = n;
			}
			if(saddr == p->inner_ip && sport == p->inner_port && IPPROTO_TCP == p->protocol){
				iph->saddr = p->outside_ip;
				tcph->source = htons((unsigned short)p->outside_port);
				p->time = get_second() + NAT_CONNECT_STATUE_TIME;
				update_checksum(skb);
				printk("NAT Get a package from %d.%d.%d.%d:%d to %d.%d.%d.%d:%d type:%d\n",
					((unsigned char *)&iph->saddr)[0],
					((unsigned char *)&iph->saddr)[1],
					((unsigned char *)&iph->saddr)[2],
					((unsigned char *)&iph->saddr)[3],
					ntohs(tcph->source),
					((unsigned char *)&iph->daddr)[0],
					((unsigned char *)&iph->daddr)[1],
					((unsigned char *)&iph->daddr)[2],
					((unsigned char *)&iph->daddr)[3],
					ntohs(tcph->dest),
					iph->protocol
				);
				return NF_ACCEPT;
			}
			p = p->next;
		}

		int i=1;
		for(;i<=nat_list_sum;i++){
			if(saddr == nat_list[i].inner_ip){
				NAT_connect_list * new_connect1 = (NAT_connect_list*)kmalloc(sizeof(NAT_connect_list),GFP_KERNEL);
				new_connect1->inner_ip = saddr;
				new_connect1->inner_port = sport;
				new_connect1->outside_ip = nat_list[i].outside_ip;
				new_connect1->sock = NULL;
				new_connect1->outside_port = generate_random_port(IPPROTO_TCP, new_connect1);
				new_connect1->protocol = IPPROTO_TCP;
				new_connect1->time = get_second() + NAT_CONNECT_STATUE_TIME;
				new_connect1->id = 0;
                new_connect1->last = NULL;
                new_connect1->next = NULL;

				unsigned int index1 = dport;
				unsigned int index2 = ((daddr)>>16) & 0xffff;

				NAT_connect_list * p = NAT_connect_index[index1^index2];
				if(p == NULL){
					NAT_connect_index[index1^index2] = new_connect1;
				}
				else {
					while(p->next != NULL) p = p->next;
					p->next = new_connect1;
					new_connect1->last = p;
				}

				iph->saddr = new_connect1->outside_ip;
				tcph->source = htons((unsigned short)new_connect1->outside_port);
				update_checksum(skb);
				printk("NAT Get a package from %d.%d.%d.%d:%d to %d.%d.%d.%d:%d type:%d\n",
					((unsigned char *)&iph->saddr)[0],
					((unsigned char *)&iph->saddr)[1],
					((unsigned char *)&iph->saddr)[2],
					((unsigned char *)&iph->saddr)[3],
					ntohs(tcph->source),
					((unsigned char *)&iph->daddr)[0],
					((unsigned char *)&iph->daddr)[1],
					((unsigned char *)&iph->daddr)[2],
					((unsigned char *)&iph->daddr)[3],
					ntohs(tcph->dest),
					iph->protocol
				);
				break;
			}
		}
	}
	else if(type == IPPROTO_UDP){
		unsigned int sport = 0;
		unsigned int dport = 0;
		struct udphdr *udph;
		udph = (void *)iph + (iph->ihl)*4;
		sport = ntohs(udph->source);
		dport = ntohs(udph->dest);

		NAT_connect_list *p = NAT_connect_index[((daddr>>16)&0xffff)^dport];
		while(p != NULL){
			if(p->time < get_second()){
				NAT_connect_list * n = p->next;
				delete_nat_connect(((daddr>>16)&0xffff)^dport, p);
				if(n == NULL) break;
				p = n;
			}
			if(saddr == p->inner_ip && sport == p->inner_port && IPPROTO_UDP == p->protocol){
				iph->saddr = p->outside_ip;
				udph->source = htons((unsigned short)p->outside_port);
				update_checksum(skb);
				update_ip_checksum(skb);
				p->time = get_second() + NAT_CONNECT_STATUE_TIME;
				printk("NAT Get a package from %d.%d.%d.%d:%d to %d.%d.%d.%d:%d type:%d\n",
					((unsigned char *)&iph->saddr)[0],
					((unsigned char *)&iph->saddr)[1],
					((unsigned char *)&iph->saddr)[2],
					((unsigned char *)&iph->saddr)[3],
					ntohs(udph->source),
					((unsigned char *)&iph->daddr)[0],
					((unsigned char *)&iph->daddr)[1],
					((unsigned char *)&iph->daddr)[2],
					((unsigned char *)&iph->daddr)[3],
					ntohs(udph->dest),
					iph->protocol
				);
				return NF_ACCEPT;
			}
			p = p->next;
		}

		int i=1;
		for(;i<=nat_list_sum;i++){
			if(saddr == nat_list[i].inner_ip){
				NAT_connect_list * new_connect1 = (NAT_connect_list*)kmalloc(sizeof(NAT_connect_list),GFP_KERNEL);
				new_connect1->inner_ip = saddr;
				new_connect1->inner_port = sport;
				new_connect1->outside_ip = nat_list[i].outside_ip;
				new_connect1->sock = NULL;
				new_connect1->outside_port = generate_random_port(IPPROTO_UDP, new_connect1);
				new_connect1->protocol = IPPROTO_UDP;
				new_connect1->time = get_second() + NAT_CONNECT_STATUE_TIME;
				new_connect1->id = 0;
                new_connect1->last = NULL;
                new_connect1->next = NULL;

				unsigned int index1 = dport;
				unsigned int index2 = ((daddr)>>16) & 0xffff;

				NAT_connect_list * p = NAT_connect_index[index1^index2];
				if(p == NULL){
					NAT_connect_index[index1^index2] = new_connect1;
				}
				else {
					while(p->next != NULL) p = p->next;
					p->next = new_connect1;
					new_connect1->last = p;
				}

				iph->saddr = new_connect1->outside_ip;
				udph->source = htons((unsigned short)new_connect1->outside_port);
				update_checksum(skb);
				printk("NAT Get a package from %d.%d.%d.%d:%d to %d.%d.%d.%d:%d type:%d\n",
					((unsigned char *)&iph->saddr)[0],
					((unsigned char *)&iph->saddr)[1],
					((unsigned char *)&iph->saddr)[2],
					((unsigned char *)&iph->saddr)[3],
					ntohs(udph->source),
					((unsigned char *)&iph->daddr)[0],
					((unsigned char *)&iph->daddr)[1],
					((unsigned char *)&iph->daddr)[2],
					((unsigned char *)&iph->daddr)[3],
					ntohs(udph->dest),
					iph->protocol
				);
				break;
			}
		}
	}
	else if(type == IPPROTO_ICMP){
		struct icmphdr *icmph;
		icmph = (void *)iph + iph->ihl*4;

		NAT_connect_list *p = NAT_connect_index[((daddr>>16)&0xffff)^0];
		while(p != NULL){
			if(p->time < get_second()){
				NAT_connect_list * n = p->next;
				delete_nat_connect(((daddr>>16)&0xffff)^0, p);
				if(n == NULL) break;
				p = n;
			}
			if(saddr == p->inner_ip && IPPROTO_ICMP == p->protocol && icmph->un.echo.id == p->id){
				iph->saddr = p->outside_ip;
				p->time = get_second() + NAT_CONNECT_STATUE_TIME;
				update_checksum(skb);
				return NF_ACCEPT;
			}
			p = p->next;
		}

		int i=1;
		for(;i<=nat_list_sum;i++){
			if(saddr == nat_list[i].inner_ip){
				NAT_connect_list * new_connect1 = (NAT_connect_list*)kmalloc(sizeof(NAT_connect_list),GFP_KERNEL);
				new_connect1->inner_ip = saddr;
				new_connect1->inner_port = 0;
				new_connect1->outside_ip = nat_list[i].outside_ip;
				new_connect1->sock = NULL;
				new_connect1->outside_port = 0;
				new_connect1->protocol = IPPROTO_ICMP;
				new_connect1->time = get_second() + NAT_CONNECT_STATUE_TIME;
				new_connect1->id = icmph->un.echo.id;
                new_connect1->last = NULL;
                new_connect1->next = NULL;

				unsigned int index1 = 0;
				unsigned int index2 = ((daddr)>>16) & 0xffff;

				NAT_connect_list * p = NAT_connect_index[index1^index2];
				if(p == NULL){
					NAT_connect_index[index1^index2] = new_connect1;
				}
				else {
					while(p->next != NULL) p = p->next;
					p->next = new_connect1;
					new_connect1->last = p;
				}
				iph->saddr = new_connect1->outside_ip;
				update_checksum(skb);
				break;
			}
		}
	}
	return NF_ACCEPT;
}

/*
	NAT收包转换实现程序
*/
unsigned int telnet_NAT_in(void *priv,
			struct sk_buff *skb,
			const struct nf_hook_state *state)
{
	struct iphdr *iph;
	iph = ip_hdr(skb);
	unsigned int type = iph->protocol;
	unsigned int saddr = iph->saddr;
	unsigned int daddr = iph->daddr;

	if(type == IPPROTO_TCP){
		unsigned int sport = 0;
		unsigned int dport = 0;
		struct tcphdr *tcph;
		tcph = (void *)iph + iph->ihl*4;
		sport = ntohs(tcph->source);
		dport = ntohs(tcph->dest);
		NAT_connect_list *p = NAT_connect_index[((saddr>>16)&0xffff)^sport];
		while(p != NULL){
			if(p->time < get_second()){
				NAT_connect_list * n = p->next;
				delete_nat_connect(((saddr>>16)&0xffff)^sport, p);
				if(n == NULL) break;
				p = n;
			}
			if(daddr == p->outside_ip && dport == p->outside_port && IPPROTO_TCP == p->protocol){
				iph->daddr = p->inner_ip;
				tcph->dest = htons((unsigned short)p->inner_port);
				p->time = get_second() + NAT_CONNECT_STATUE_TIME;
				update_checksum(skb);
				break;
			}
			p = p->next;
		}
	}
	else if(type == IPPROTO_UDP){
		unsigned int sport = 0;
		unsigned int dport = 0;
		struct udphdr *udph;
		udph = (void *)iph + iph->ihl*4;
		sport = ntohs(udph->source);
		dport = ntohs(udph->dest);
		NAT_connect_list *p = NAT_connect_index[((saddr>>16)&0xffff)^sport];
		while(p != NULL){
			if(p->time < get_second()){
				NAT_connect_list * n = p->next;
				delete_nat_connect(((saddr>>16)&0xffff)^sport, p);
				if(n == NULL) break;
				p = n;
			}
			if(daddr == p->outside_ip && dport == p->outside_port && IPPROTO_UDP == p->protocol){
				iph->daddr = p->inner_ip;
				udph->dest = htons((unsigned short)p->inner_port);;
				p->time = get_second() + 2;
				update_checksum(skb);
				break;
			}
			p = p->next;
		}
	}
	else if(type == IPPROTO_ICMP){
		struct icmphdr *icmph;
		icmph = (void *)iph + iph->ihl*4;
		NAT_connect_list *p = NAT_connect_index[((saddr>>16)&0xffff)^0];
		while(p != NULL){
			if(p->time < get_second()){
				NAT_connect_list * n = p->next;
				delete_nat_connect(((saddr>>16)&0xffff)^0, p);
				if(n == NULL) break;
				p = n;
			}
			if(daddr == p->outside_ip && icmph->un.echo.id == p->id && IPPROTO_ICMP == p->protocol){
				iph->daddr = p->inner_ip;
				update_checksum(skb);
				break;
			}
			p = p->next;
		}
	}
	return NF_ACCEPT;
}

/*
	netlink 发送函数
*/
int nltest_ksend(char *reply, int pid, unsigned int rlen)
{
	struct sk_buff *skb;
	struct nlmsghdr *nlh;
	int retval;

	rlen++;

	skb = nlmsg_new(rlen, GFP_ATOMIC);
	if (skb == NULL) {
		printk("alloc reply nlmsg skb failed!\n");
		return -1;
	}

	nlh = nlmsg_put(skb, 0, 0, 0, NLMSG_SPACE(rlen) - NLMSG_HDRLEN, 0);
	memcpy(NLMSG_DATA(nlh), reply, rlen);
	printk("[kernel space] nlmsglen = %d\n", nlh->nlmsg_len);

	//NETLINK_CB(skb).pid = 0;
	NETLINK_CB(skb).dst_group = 0;

	printk("[kernel space] skb->data send to user: '%s'\n", (char *) NLMSG_DATA(nlh));

	retval = netlink_unicast(nlsk, skb, pid, MSG_DONTWAIT);
	printk("[kernel space] netlink_unicast return: %d\n", retval);
	return 0;
}

/*
	netlink 接收函数
*/
void nltest_krecv(struct sk_buff *skb)
{
	struct nlmsghdr *nlh = NULL;
	char *data;
	int pid;
	
	nlh = nlmsg_hdr(skb);
	if ((nlh->nlmsg_len < NLMSG_HDRLEN) || (skb->len < nlh->nlmsg_len)) {
		printk("Illegal netlink packet!\n");
		return;
	}
	
	data = (char *) NLMSG_DATA(nlh);
	
	pid = nlh->nlmsg_pid;
	printk("[kernel space] user_pid = %d\n", pid);
	if(data != NULL &&((unsigned int *)data)[0] == 0x01010101){
		//清空连接状态
		mepty_connect();
		//更新规则表
		printk("Receive a new rule list!\n");
		rule_list_sum = ((unsigned int *)data)[1];
		Rule_list * p = (Rule_list *)(data + 8);
		int i = 1;
		for(i; i<=rule_list_sum; i++){
			rule_list[i] = p[i-1];
			printk("%x,%x,%x,%x,%d,%d,%d\n",
				rule_list[i].src_ip,
				rule_list[i].src_port,
				rule_list[i].des_ip,
				rule_list[i].des_port,
				rule_list[i].type,
				rule_list[i].rule,
				rule_list[i].log
			);
		}
	}
	else if(data != NULL && ((unsigned int *)data)[0] == 0x02020202){
		char send_data[6004];
		unsigned int * p =(unsigned int *)send_data;
		p++;
		int i = 0;
		unsigned int tot = 0;
		for(;i<=65535;i++){
			Connect_list *q = Connect_index[i];
			while(q!=NULL){
				if(q->time < get_second()){
					Connect_list * n = q->next;
					delete_connect(i, q);
					if(n == NULL) break;
					q = n;
				}
				tot++;
				p[0] = q->src_ip;
				p[1] = q->src_port;
				p[2] = q->des_ip;
				p[3] = q->des_port;
				p[4] = q->type;
				p[5] = q->log;
				p += 6;
				q = q->next;
			}
		}
		((unsigned int *)send_data)[0] = tot;
		send_data[tot*24 + 4] = 0;
		nltest_ksend(send_data, pid, tot*24 + 4);
	}
	else if(data != NULL && ((unsigned int *)data)[0] == 0x03030303){
		LOG_pid = pid;
	}
	else if(data != NULL && ((unsigned int *)data)[0] == 0x04040404){
		//更新NAT表
		printk("Receive a new NAT list!\n");
		mepty_nat_connect();
		nat_list_sum = ((unsigned int *)data)[1];
		NAT_list * p = (NAT_list *)(data + 8);
		int i = 1;
		for(i; i<=nat_list_sum; i++){
			nat_list[i] = p[i-1];
			printk("%x,%x\n",
				nat_list[i].inner_ip,
				nat_list[i].outside_ip
			);
		}
	}
	else if(data != NULL && ((unsigned int *)data)[0] == 0x05050505){
		//发送现在建立的NAT转换表
		char send_data[6004];
		unsigned int * p =(unsigned int *)send_data;
		p++;
		int i = 0;
		unsigned int tot = 0;
		for(;i<=65535;i++){
			NAT_connect_list *q = NAT_connect_index[i];
			while(q!=NULL){
				if(q->time < get_second()){
					NAT_connect_list * n = q->next;
					delete_nat_connect(i, q);
					if(n == NULL) break;
					q = n;
				}
				tot++;
				p[0] = q->inner_ip;
				p[1] = q->inner_port;
				p[2] = q->outside_ip;
				p[3] = q->outside_port;
				p[4] = q->protocol;
				p += 5;
				q = q->next;
			}
		}
		((unsigned int *)send_data)[0] = tot;
		send_data[tot*20 + 4] = 0;
		nltest_ksend(send_data, pid, tot*20 + 4);
	}
	
}

struct netlink_kernel_cfg nltest_cfg = {
	0,	//groups
	0,	//flags
	nltest_krecv,	//input
	NULL,	//cb_mutex
	NULL,	//bind
	NULL,	//unbind
	NULL,	//compare
};

static int myfw_init(void)
{
	//初始化netlink
	nlsk = netlink_kernel_create(&init_net, NETLINK_TEST, &nltest_cfg);
	if (!nlsk) {
		printk("can not create a netlink socket\n");
		return -1;
	}
	printk("netlink_kernel_create() success, nlsk = %p\n", nlsk);
	printk("my firewall module loaded.\n");

	fw_in.hook = telnet_filter;
	fw_in.pf = PF_INET;
	fw_in.hooknum = NF_INET_LOCAL_IN;
	fw_in.priority = NF_IP_PRI_FIRST; //new version, maybe changed to NF_INET_PRI_FIRST

	nf_register_net_hook(&init_net, &fw_in);

    fw_out.hook = telnet_filter;
	fw_out.pf = PF_INET;
	fw_out.hooknum = NF_INET_LOCAL_OUT;
	fw_out.priority = NF_IP_PRI_FIRST; //new version, maybe changed to NF_INET_PRI_FIRST

	nf_register_net_hook(&init_net, &fw_out);
	

	nat_in.hook = telnet_NAT_in;
	nat_in.pf = PF_INET;
	nat_in.hooknum = NF_INET_PRE_ROUTING;
	nat_in.priority = NF_IP_PRI_FIRST; //new version, maybe changed to NF_INET_PRI_FIRST

	nf_register_net_hook(&init_net, &nat_in);

    nat_out.hook = telnet_NAT_out;
	nat_out.pf = PF_INET;
	nat_out.hooknum = NF_INET_POST_ROUTING;
	nat_out.priority = NF_IP_PRI_FIRST; //new version, maybe changed to NF_INET_PRI_FIRST

	nf_register_net_hook(&init_net, &nat_out);

	return 0;
}

static void myfw_exit(void)
{
	printk("my firewall module exit ...\n");
	sock_release(nlsk->sk_socket);
	nf_unregister_net_hook(&init_net, &fw_in);
    nf_unregister_net_hook(&init_net, &fw_out);
	nf_unregister_net_hook(&init_net, &nat_in);
    nf_unregister_net_hook(&init_net, &nat_out);
}

module_init(myfw_init);
module_exit(myfw_exit);
