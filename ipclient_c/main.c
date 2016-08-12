/**
 * @file   main.c
 * @author gan xiangle <ganxiangle@gmail.com>
 * @date   Mon Jun  6 01:26:57 2011
 * 
 * @brief  disasm of "ipclient for linux"
 * 
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "utils.h"

int vars_shmid;
union PVARS pvars;

int  key;
char userid[0x600];
char passwd[0x600];
int  online;
int  refreshtick;

int main(int argc, char *argv[])
{
	int userpass_input = 0;
	/* create_sharemem() set pvars to addr of sharemem */
	create_sharemem();					
	
	if (argc == 2) {
		/* run "ipclient remove" will just return */
		char *p = argv[1];
		if (strncmp("remove", p, 7) == 0) {
			*(int*)(pvars.pdata) = 0;
			puts("ipclient quits from memory.");
			exit(0);
		}		
	} 
	if (argc == 3) {
		strcpy(userid, argv[1]);
		strcpy(passwd, argv[2]);	
		userpass_input = 1;
	}

	*(int*)(pvars.pdata) = 0;
	usleep(100 * 1000);
	pid_t pid = getpid();
	*(int*)(pvars.pdata) = pid;
	
	puts("*********************************************");
	puts("*                                           *");
	puts("*     IPClient for Linux. Version 1.0       *");
	puts("*     Usage: ipclient                       *");
	puts("*     Usage: ipclient userid password       *");
	puts("*     Usage: ipclient remove                *");
	puts("*                                           *");
	puts("*                                           *");
	puts("*********************************************");
	puts("bug report: qky@guet.edu.cn");
	
	online = 0;

	if (userpass_input == 0) {
		printf("Please input your userid: ");
		scanf("%s", userid);
		printf("Please input your password: ");
		scanf("%s", passwd);
	}

	int userid_len = strlen (userid);
	if (userid_len > 0x12) {
		puts("userid is too long.");
		return -1;
	}
    int passwd_len = strlen (passwd);
	if (passwd_len > 0x12) {
		puts("passwd is too long.");
		return -1;
	}
	/* if openip OK, it will set online = 1 */
	openip(userid, passwd);

	if (online == 0) {
		exit(1);
	}
	puts("ipclient residents in memory.");

	daemon_init();

	while (1) {
		// refreshing per 30 seconds 
		usleep(50 * 1000);
		
		if (*(int*)pvars.pdata == 0) { // or pvars.pdata == NULL
			release_sharemem();
			return 0;
		}

		if (++refreshtick > 600)
			refreshtick = 0;
			
		if (refreshtick == 590)
			sendrefresh();
	}

    return 0;
}

