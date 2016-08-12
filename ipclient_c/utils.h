/**
 * @file   utils.h
 * @author gan xiangle <ganxiangle@gmail.com>
 * @date   Mon Jun  6 01:59:38 2011
 * 
 * @brief  
 * 
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "md5.h"

union PVARS{
	char *pdata;
	int v;
};
extern union PVARS pvars;

extern int vars_shmid;
extern int  key;
extern char userid[0x600];
extern char passwd[0x600];
extern int  online;
extern int  refreshtick;

#define PA    struct sockaddr *
//#define SERVER_IP "192.168.1.13"
#define SERVER_IP "202.193.160.123"
#define CONTROL_PORT  5300
#define REFRESH_PORT  5301

/*=====================CMD raw packet format=====================*/
/* +-----------------------------------------------------------+ */
/* | banner | cmd   | id ? | un1 | len1 | data1 | len2 | data2 | */
/* +-----------------------------------------------------------+ */
/* | 2      | 1     |    4 |   4 |    4 | ...   |    4 | ...   | */
/* +-----------------------------------------------------------+ */
/* | len3   | data3 |  KEY | un2 | len4 | data4 |      |       | */
/* +-----------------------------------------------------------+ */
/* | 4      | ...   |    4 |   4 |    4 | ...   |      |       | */
/* +-----------------------------------------------------------+ */

#define	CMDPACKET_BANNER_OFFSET(rawpacket)   0
#define	CMDPACKET_CMD_OFFSET(rawpacket)      2
#define	CMDPACKET_ID_OFFSET(rawpacket)       3
#define	CMDPACKET_UN1_OFFSET(rawpacket)      7
#define	CMDPACKET_LEN1_OFFSET(rawpacket)     0xb
#define	CMDPACKET_DATA1_OFFSET(rawpacket)    0xf
#define CMDPACKET_LEN2_OFFSET(rawpacket)								\
	(CMDPACKET_DATA1_OFFSET(rawpacket) +								\
							*(int*)(rawpacket + CMDPACKET_LEN1_OFFSET(rawpacket)))
#define CMDPACKET_DATA2_OFFSET(rawpacket)		\
	(CMDPACKET_LEN2_OFFSET(rawpacket) + 4)
#define CMDPACKET_LEN3_OFFSET(rawpacket)					\
	(CMDPACKET_DATA2_OFFSET(rawpacket) +					\
	 *(int*)(rawpacket + CMDPACKET_LEN2_OFFSET(rawpacket)))
#define CMDPACKET_DATA3_OFFSET(rawpacket)		\
	(CMDPACKET_LEN3_OFFSET(rawpacket) + 4)
#define CMDPACKET_KEY_OFFSET(rawpacket)						\
	(CMDPACKET_DATA3_OFFSET(rawpacket) +					\
	 *(int*)(rawpacket + CMDPACKET_LEN3_OFFSET(rawpacket)))
#define CMDPACKET_UN2_OFFSET(rawpacket)			\
	(CMDPACKET_KEY_OFFSET(rawpacket) + 4) 
#define CMDPACKET_LEN4_OFFSET(rawpacket)		\
	(CMDPACKET_UN2_OFFSET(rawpacket) + 4)
#define CMDPACKET_DATA4_OFFSET(rawpacket)		\
	(CMDPACKET_LEN4_OFFSET(rawpacket) + 4)

#define CMDPACKET_BANNER(rawpacket)\
	(*(short*)(rawpacket + CMDPACKET_BANNER_OFFSET(rawpacket)))
#define	CMDPACKET_CMD(rawpacket)\
	(*(char*)(rawpacket + CMDPACKET_CMD_OFFSET(rawpacket)))
#define	CMDPACKET_ID(rawpacket)\
	(*(int*)(rawpacket + CMDPACKET_ID_OFFSET(rawpacket)))
#define CMDPACKET_UN1(rawpacket)\
	(*(int*)(rawpacket + CMDPACKET_UN1_OFFSET(rawpacket)))
#define CMDPACKET_LEN1(rawpacket)\
	(*(int*)(rawpacket + CMDPACKET_LEN1_OFFSET(rawpacket)))
#define CMDPACKET_LEN2(rawpacket)\
	(*(int*)(rawpacket + CMDPACKET_LEN2_OFFSET(rawpacket)))
#define CMDPACKET_LEN3(rawpacket)\
	(*(int*)(rawpacket + CMDPACKET_LEN3_OFFSET(rawpacket)))
#define CMDPACKET_LEN4(rawpacket)\
	(*(int*)(rawpacket + CMDPACKET_LEN4_OFFSET(rawpacket)))
#define CMDPACKET_KEY(rawpacket)\
	(*(int*)(rawpacket + CMDPACKET_KEY_OFFSET(rawpacket)))
#define CMDPACKET_UN2(rawpacket)\
	(*(int*)(rawpacket + CMDPACKET_UN2(rawpacket)))


typedef struct _chars {
	char v[0x24];
}Chars;
	
typedef struct _chars2 {
	char v[0x14];
	int  u1;							/* unKown */
	int  u2;							/* unKown */
	char padding[4];					/* pad to 0x20 bytes */
}Chars2;

typedef struct _head {
	short type;							/* banner */
	char  cmd;							/* sub cmd maybe */
	int   id;							/* may be the response cmd */
	int   un;							/* unkown,not used, just padding */
	struct _chars  chars1;
	struct _chars  chars2;
	struct _chars2 chars21;
	struct _chars  chars3;
	char reserve[0x40]; 				/* up to 0xD8 */
}CmdHead;


typedef struct _refchars {
	char v[0x18];
}RefreshChars;

typedef struct _refhead {
	short type;							/* banner */
	char cmd;							/* sub cmd maybe */
	int  id;							/* may be the sequence */
	int  un1;							/* unkown */
	int  un2;
	int  un3;
	int  un4;
	struct _refchars chars1;
	struct _refchars chars2;
	struct _refchars chars3;
}RefreshHead;

#define MDString MyMDString
void MD5Print(unsigned char digest[16]);
char* MyMDString(char *string);

void create_sharemem();
void release_sharemem();

int openip(char *userid, char* passwd);
void daemon_init();

void initrefresh();
void sendrefresh();

char* str2upper(char *string);
char* changestr(char *string, char num_to_add);

void fillcmdpacket(char *packet, CmdHead head);

void fillrefreshpacket(char *packet, RefreshHead head);
#endif
