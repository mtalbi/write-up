#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#define MAX_AGENT 256
 
void main_menu(void);
 
void agent_create(void);
void agent_show(void);
void agent_select(void);
void agent_delete(void);
void agent_edit(void);
 
int agent_edit_name(char *buffer, int size);
 
typedef struct agent {
	unsigned int   size;
	unsigned long  reserved_0;
	char		  *name;
	unsigned int   id;
	char		   reserved_1[128];
} agent_t;
 
agent_t *agents[MAX_AGENT];
unsigned int agent_count = 0;
unsigned int agent_sel = 0;
unsigned int global_id = 0;
 
int main(int argc, char *argv[])
{
	main_menu();
	return 0;
}
 
void main_menu(void)
{
	int op = 0;
	char opt[2];
 
	printf("\n\t\t\t\t[1] Create new agent");
	printf("\n\t\t\t\t[2] Select agent");
	printf("\n\t\t\t\t[3] Show agent");
	printf("\n\t\t\t\t[4] Edit agent");
	printf("\n\t\t\t\t[5] Delete agent");
	printf("\n\t\t\t\t[0] <- EXIT");
	printf("\n\t\t\t\tSelect your option:");
	fflush(stdout);
	fgets(opt, 3, stdin);
 
	op = atoi(opt);
 
	switch (op) {
		case 1:
			agent_create();
			break;
		case 2:
			agent_select();
			break;
		case 3:
			agent_show();
			break;
		case 4:
			agent_edit();
			break;
		case 5:
			agent_delete();
			break;
		case 0:
			exit(0);
		default:
			break;
	}
 
	main_menu();
}
 
void agent_create(void)
{
 
	char buffer[4096];
	int len;
 
	if (agent_count < MAX_AGENT) {
		agents[agent_count] = malloc(sizeof(agent_t));
 
		len = agent_edit_name(buffer, 4096);
		agents[agent_count]->name = malloc(len + 1);
		strncpy(agents[agent_count]->name, "A_", 2);
		memcpy(agents[agent_count]->name + 2, buffer, len);
		agents[agent_count]->name[len + 2] = '\0';
		agents[agent_count]->size = len + 2 + 1;
 
		agents[agent_count]->reserved_0 = 0;
		memset(agents[agent_count]->reserved_1, '\0', 128);
 
		agents[agent_count]->id = global_id++;
 
		agent_sel = agent_count++;
		printf("\n[+] Agent %d selected.", agents[agent_sel]->id);
	}
}
 
void agent_select(void)
{
	char ag_id[4];
	int ag, i = 0;
	printf("\nWrite agent number:");
	fflush(stdout);
	read(0, ag_id, 3);
	ag = atoi(ag_id);
 
	while (i < agent_count && agents[i]->id != ag) {
		i++;
	}
 
	if (i == agent_count) {
		printf("\n[!] No such agent [%d], select another", ag);
	}
	else {
		agent_sel = i;
		printf("\n[+] Agent %d selected.", agents[agent_sel]->id);
	}
}
 
void agent_edit(void)
{
	char buffer[4096];
	int len;
	if (agent_count > 0) {
		len = agent_edit_name(buffer, 4096);
		if (len + 1 > agents[agent_sel]->size) {
			agents[agent_sel]->name = realloc(agents[agent_sel]->name, len + 1);
		}
		memcpy(agents[agent_sel]->name, buffer, len);
		agents[agent_sel]->name[len] = '\0';
	}
	else {
		printf("\n[!] No agents to edit");
	}
}
 
int agent_edit_name(char *buffer, int size)
{
	int len = 0;
	printf("\nEdit agent name:");
	fflush(stdout);
	len = read(0, buffer, size - 1);
	if (len > 0 && buffer[len-1] == '\n') len--;
	buffer[len] = '\0';
	return len;
}
 
void agent_delete(void)
{
	if (agent_count > 0) {
		free(agents[agent_sel]->name);
		free(agents[agent_sel]);
		agent_count--;
		if (agent_count != agent_sel) {
			agents[agent_sel] = agents[agent_count];
			printf("\n[+] Agent %d selected.", agents[agent_sel]->id);
		}
		else {
			agent_sel = 0;
			if (agent_count > 0) {
				printf("\n[+] Agent %d selected.", agents[agent_sel]->id);
			}
			else {
				printf("\n[+] No more agents.");
			}
		}
	}
	else {
		printf("\n[!] No agents to delete");
	}
}
 
void agent_show(void)
{
	if (agent_count > 0) {
		printf("\n[+] Agent %d: ", agents[agent_sel]->id);
		fflush(stdout);
		write(1, agents[agent_sel]->name, agents[agent_sel]->size);
	}
	else {
		printf("\n[!] No available agents");
	}
}
