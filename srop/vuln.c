#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
 
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
 
#define PAGE_SIZE 0x1000
#define PORT 7777
 
// in .bss
char data[PAGE_SIZE * 2];

void init();
void handle_error(char *);
int handle_conn(int);
int welcome(int);

 
void init()
{
	struct sockaddr_in sa;
	int s, c, size, k = 1;
 
	sa.sin_family = AF_INET;
	sa.sin_port = htons(PORT);
	sa.sin_addr.s_addr = INADDR_ANY;
 
	size = sizeof(struct sockaddr);
 
	if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		handle_error("socket failed\n");
	}
 
	if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &k, sizeof(int)) == -1) {
		handle_error("setsockopt failed\n");
  }
 
	if(bind(s, (struct sockaddr *)&sa, size)) {
		handle_error("bind failed\n");
	}
 
	if(listen(s, 3) < 0) {
		handle_error("listen failed\n");
	}
 
	while(1) {
		if((c = accept(s, (struct sockaddr *)NULL, NULL)) < 0) {
			handle_error("accept failed\n");
		}
		handle_conn(c);
	}
}
 
int handle_conn(int c)
{
	char input[0x400];
	int amt;
	//too large data !!!
	if((amt = read(c, input, PAGE_SIZE) < 0)) {
		handle_error("receive failed\n");
	}
	memcpy(data, input, PAGE_SIZE);
	welcome(c);
	close(c);
	return 0;
 
}
 
int welcome(int c)
{
	int amt;
	const char *msg = "I'm vulnerable program running with root priviledges!!\nPlease do not exploit me";
 
	write(c, msg, strlen(msg));
 
	if((amt = write(c, data, strlen(data))) < 0) {
		handle_error("send failed\n");
	}
	return 0;
}
 
void handle_error(char *msg)
{
	perror(msg);
	exit(-1);
}
 
void gadget()
{
	asm("mov $0xf,%rax\n");
	asm("retq\n");
}
 
int main()
{
	init();
	return 0;
}
