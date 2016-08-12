
#include "utils.h"
void MD5Print (unsigned char digest[16])
{
	unsigned int i;
	//for (i = 0; i < 16; i++)
	//	printf ("%02x ", digest[i]);
	//printf("\n");
}
void MyMD5Print (unsigned char digeststr[0x24])
{
	unsigned int i;
	//for (i = 0; i < 0x20; i++)
	//	printf ("%02x ", digeststr[i]);
	//printf("\n");

}
char* MyMDString(char *string)
{
	MD5_CTX context;
	unsigned char digest[16];
	unsigned int len = strlen (string);
	MD5Init (&context);
	MD5Update (&context, string, len);
	MD5Final (&context);

	memcpy(digest,context.digest, sizeof(digest));
	//printf ("MD5 (\"%s\") = ", string);
	//MD5Print (digest);

	MD5Print(digest);
	
	char enstr[0x24];
	char *p = enstr;
	int i;
	unsigned char c;
	for (i = 0; i < 0x10; i++, p += 2) {
		sprintf (p, "%02x", digest[i]);		
		sprintf(p+1, "%02x", (digest[i] << 4) & 0xff);
	}
	static char save_digest[0x20];
	for (i = 0; i < 0x20; i++) {
		save_digest[i] = enstr[i];
	}
	return save_digest;
}


void create_sharemem()
{
	static key_t VARS_SHMKEY = 0x00322500;
	int shmid = shmget(VARS_SHMKEY, 0x1000, 0666 | IPC_CREAT);

	vars_shmid = shmid;
	char* pdata = shmat(shmid, (void*)0, 0);
	pvars.pdata = pdata;
	return;
}
void release_sharemem()
{
	//detach itself from "shmid",it isn't destroyed.
	shmdt (pvars.pdata);
	//added: 2011-06-10, delete the sharemem.
	shmctl(vars_shmid, IPC_RMID, NULL);
}

int openip(char *userid, char* passwd)
{
	
	
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	fcntl (sock, F_SETFL, O_NONBLOCK);

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));       // added: 2011-6-13
	serveraddr.sin_family      = AF_INET;
	serveraddr.sin_port        = htons(CONTROL_PORT); // control port: 5300
	serveraddr.sin_addr.s_addr = inet_addr(SERVER_IP);

	CmdHead head;
	head.type = 0x2382;
	head.cmd  = 0x1f;
	head.id   = 0;

	strcpy(head.chars1.v, userid);
	changestr(head.chars1.v, -10);
	strcpy(head.chars2.v, "!@#$%%^&*()");
	strcpy(head.chars21.v, "qwertyu");
	head.chars21.u1 = 0x3039; // 12345 = 0x3039
	head.chars21.u2 = 0x1;
	strcpy(head.chars3.v, "ASDFGH");

	char packet[0x300];
	fillcmdpacket(packet, head);

	socklen_t addrlen;
	struct sockaddr_in addr;	
	char buf[0x800];
	/* avoid the noise */
	recvfrom(sock, buf, sizeof(buf), 0, (PA)&addr, &addrlen);
	recvfrom(sock, buf, sizeof(buf), 0, (PA)&addr, &addrlen);
	/* ====================== cmd packet format ==================== */
	/* +-----------------------------------------------------------+ */
	/* | banner | cmd   | id   | un1 | len1 | data1 | len2 | data2 | */
	/* +-----------------------------------------------------------+ */
	/* | 2      | 1     |    4 |   4 |    4 | ...   |    4 | ...   | */
	/* +-----------------------------------------------------------+ */
	/* | len3   | data3 |  KEY | un2 | len4 | data4 |      |       | */
	/* +-----------------------------------------------------------+ */
	/* | 4      | ...   |    4 |   4 |    4 | ...   |      |       | */
	/* +-----------------------------------------------------------+ */
	sendto(sock, packet, 300, 0, (PA)&serveraddr, sizeof(serveraddr));

	addrlen = sizeof(struct sockaddr_in);
	int retry = 0; //int var_163c = 0;
	while (1) {
		int res = recvfrom(sock, buf, sizeof(buf), 0, (PA)&addr, &addrlen);
		//printf("recvfrom: res is %x\n", res);
		if (res > 0) {
			/* if (*(short *)(buf + 0) == 0x2382 && */
			/* 	*(char  *)(buf + 2) == 0x20 ) { */
			if (CMDPACKET_BANNER(buf) == 0x2382 &&
				CMDPACKET_CMD(buf) == 0x20) {

				key = CMDPACKET_KEY(buf) - 3344;
				//printf ("key received is 0x%x, cal KEY is 0x%x\n",
				//		CMDPACKET_KEY(buf),
				//		key);
				head.type = 0x2382;
				head.cmd  = 0x21;
				head.id   = 0;
				head.chars21.u1 = 0x03d61f2d;
				head.chars21.u2 = 0x0024f2cc;
				strcpy(head.chars1.v , "9gdt43745wrwqr");
				strcpy(head.chars21.v, "11:22:33:44:55:66");
				strcpy(head.chars3.v , "qwertyuiop");

				char linebuf[80];
				char linebuf2[80];

				sprintf(linebuf, "%d%s", key, passwd);
                            //    printf("md51:%s\n",linebuf);
				char *enstr = MDString(linebuf);
				strcpy(linebuf2, enstr);
				linebuf2[5] = '\0';
				str2upper(linebuf2);

				sprintf(linebuf, "%s%s", linebuf2, userid);
				enstr = MDString(linebuf);
				strcpy(linebuf2, enstr);
				str2upper(linebuf2);
				strncpy(head.chars2.v, linebuf2, 30);
				head.chars2.v[30] = '\0'; // added

				//printf ("linebuf's len is 0x%x\n, linebuf2 is %s\n",
				//	strlen(linebuf2),
				//	linebuf2);
				//printf("strlen of head.chars2.v is 0x%x\n, chars2.v is %s\n",
				 //  strlen(head.chars2.v),
				//	   head.chars2.v);
				
				fillcmdpacket(packet, head);

				recvfrom(sock, buf, sizeof(buf), 0, (PA)&addr, &addrlen);
				recvfrom(sock, buf, sizeof(buf), 0, (PA)&addr, &addrlen);

				sendto(sock, packet, 300, 0, (PA)&serveraddr, sizeof(serveraddr));
				retry = 0;
				addrlen = sizeof(struct sockaddr_in);

				while (1) {
					int ret = recvfrom(sock, buf, sizeof(buf), 0, (PA)&addr, &addrlen);
					if (ret > 0) {
						if (CMDPACKET_BANNER(buf) != 0x2382 || 
							CMDPACKET_CMD(buf) != 0x22 )
							goto Fail;
						
						/* int n = *(int*) (buf + CMDPACKET_DATA4_OFFSET(buf) ); */
						/* char newbuf[0x100]; */
						/* strncpy(newbuf, buf + CMDPACKET_DATA4_OFFSET(buf) + 4, n); /\* ??? *\/ */
						/* newbuf[n] = '\0'; */

						int resp = CMDPACKET_ID(buf);
						switch (resp) {
						case 0:
							{
								online = 1;
								refreshtick = 0;
								puts("open ip successfully.");
								close(sock);
								return 1;
							} 
						case 1:    
							puts("your ip needn't use ipclient.");
							break;
						case 0xa:  
							puts("your account is expired.");
							break;
						case 0xb:  
							puts("your account is disabled.");
							break;
						case 0x14: 
							puts("your account has not enough money.");
							break;
						case 0x15: 
							puts("your accont has not available hours in this month.");
							break;
						case 0x16: 
							puts("your account has not available flow in this mouth.");
							break;
						case 0x19: 
							puts("your account cannot be used in this IP.");
							break;
						case 0x1e: 
							puts("your account cannot be used in this time.");
							break;
						case 0x1f: 
							puts("please dial later.");
							break;
						case 0x20: 
							puts("too many users are using this account now.");
							break;
						case 0x21: 
							puts("ipclient cannot be used for your account.");
							break;
						case 0x22: 
							puts("please dial later.");
							break;
						case 0x63: 
							puts("userid or password error.");
							break;
						default: break;
						}							
						close(sock);
						return 0;
					}
					if (++retry > 5) {
						goto Receive_fail;
					}
					usleep(100 * 1000);
				}
			}
			else 
				goto Fail;		
		}
		if (++retry > 5) {
			goto Receive_fail;
		}
		usleep(100 * 1000);		
	}	
 Fail:
	puts("connect ip server fail.");
	close(sock);
	return 0;

 Receive_fail:
	puts("receive packet fail");
	close(sock);
	return 0;
}
void daemon_init()
{
	pid_t pid;

	if ((pid = fork()) < 0) {
		return;
	} else if (pid != 0) {
		exit(0);
	} else {
		setsid();
		chdir("/");
		umask(0);
	}
}


void initrefresh()
{
	/* nothing */
}
void sendrefresh()
{
	

	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	fcntl(sock, F_SETFL, O_NONBLOCK);

	struct sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port   = htons(0x14b5); /* port: 5301 */
	serveraddr.sin_addr.s_addr = inet_addr(SERVER_IP);

	RefreshHead head;

	head.type = 0x2382;
	head.cmd  = 0x1e;
	head.id   = key + 1500;
	head.un1  = 0x02863ee4;
	head.un2  = 0xf5c28f5c;
	head.un3  = 0x40dfa9f0;
	
	strcpy(head.chars1.v, userid);
	strcpy(head.chars2.v, "Spiderman");

	char packet[0x200];
	fillrefreshpacket(packet, head);
	
	char buf[0x800];
	struct sockaddr_in addr;
	socklen_t addrlen;

	recvfrom(sock, buf, sizeof(buf), 0,
			 (PA)&addr, &addrlen);
	recvfrom(sock, buf, sizeof(buf), 0,
			 (PA)&addr, &addrlen);
	sendto(sock, packet, 500, 0, 
		   (PA)&serveraddr, sizeof(serveraddr));

	addrlen = sizeof(struct sockaddr_in);
	int retry = 0;
	while (1) {
		int res = recvfrom(sock, buf, sizeof(buf), 0,
						   (PA)&addr, &addrlen);
		if (res > 0) {
			if (*(short *)buf == 0x2382 ) {
				if (buf[2] == 0x1f) {
					close(sock);
					return;
				}
			} 
		Fail:	
			puts("connect ip server fail.");
			close(sock);
			return;
		}
		
		if (++retry > 5) {
			puts("receive packet fail");
			return;
		}
		usleep(0x186a0);
	}
	
}
char* str2upper(char *string)
{
	int len = strlen (string);
	char *p;
	int i;
	for (p = &string[0], i = 0; i < len; i++,p++) {
		char c = *p;
		c = c - 'a';
		if (c > 0x19 || c < 0)			/* not in range ('a','z') */
			continue;
		*p -= 0x20;						/* to upper */
	}
	return string;
}


char * changestr(char *string, char num_to_add)
{
	int len = strlen (string);
	int i;
	char* p;
	if (len > 0) {
		for (p = &string[0],i = 0; i < len; i++,p++) {
			*p += num_to_add;
		}
	}
	return string;
}

void fillcmdpacket(char *packet, CmdHead head)
{
	/* ====================== cmd packet format ==================== */
	/* +-----------------------------------------------------------+ */
	/* | banner | cmd   | id   | un1 | len1 | data1 | len2 | data2 | */
	/* +-----------------------------------------------------------+ */
	/* | 2      | 1     |    4 |   4 |    4 | ...   |    4 | ...   | */
	/* +-----------------------------------------------------------+ */
	/* | len3   | data3 |  KEY | un2 | len4 | data4 |      |       | */
	/* +-----------------------------------------------------------+ */
	/* | 4      | ...   |    4 |   4 |    4 | ...   |      |       | */
	/* +-----------------------------------------------------------+ */

	int len1 = strlen (head.chars1.v);
	int len2 = strlen (head.chars2.v);
	int len3 = strlen (head.chars21.v);
	int len4 = strlen (head.chars3.v);
	
	memset(packet, 0, 300);
	*(short *)(packet + 0)   = head.type;
	*(char  *)(packet + 2)   = head.cmd;
    *(int   *)(packet + 3)   = head.id;

	*(int *)(packet + 0xb) = len1;
	memcpy(packet + 0xf, head.chars1.v, len1);
	packet += len1;

	*(int *)(packet + 0xf) = len2;
	memcpy(packet + 0x13, head.chars2.v, len2);
	packet += len2;

	*(int *)(packet + 0x13) = len3;
	memcpy(packet + 0x17, head.chars21.v, len3);
	packet += len3;
	*(int *)(packet + 0x17) = head.chars21.u1;
	*(int *)(packet + 0x1b) = head.chars21.u2;

	*(int *)(packet + 0x1f) = len4;
	memcpy(packet + 0x23, head.chars3.v, len4);	
}

void fillrefreshpacket(char *packet, RefreshHead head)
{

	/* ======================== Refresh packet=========================== */
	/* +----------------------------------------------------------------+ */
    /* | banner | cmd   | id   | padding1 | un1  | padding2 | un2 | un3 | */
    /* |--------+-------+------+----------+------+----------+-----+-----| */
    /* | 2      | 1     | 4    | 4        | 4    |        4 |   4 |   4 | */
    /* |--------+-------+------+----------+------+----------+-----+-----| */
    /* | len1   | data1 | len2 | data2    | len3 |    data3 |     |     | */
    /* |--------+-------+------+----------+------+----------+-----+-----| */
    /* | 4      | ...   | 4    | ...      | 4    |      ... |     |     | */
    /* |--------+-------+------+----------+------+----------+-----+-----| */
	
	/// sizeof (Chars) == 0x18
	int len1 = strlen (head.chars1.v);
	int len2 = strlen (head.chars2.v);
	int len3 = strlen (head.chars3.v);

	memset(packet, 0, 500);
	*(short*)(packet + 0) = head.type;
	*(char *)(packet + 2) = head.cmd;
	*(int  *)(packet + 3) = head.id;
	/* 4 bytes left(0x07-0x0a) */
	*(int  *)(packet + 0x0b) = head.un1;
	/* 4 bytes left( 0x0f-0x12 */
	*(int  *)(packet + 0x13) = head.un2;
	*(int  *)(packet + 0x17) = head.un3;

	*(int  *)(packet + 0x1b) = len1;
	memcpy(packet + 0x1f, head.chars1.v, len1);
	packet += len1;
	
	*(int  *)(packet + 0x1f) = len2;
	memcpy(packet + 0x23, head.chars2.v, len2);
	packet += len2;

	*(int  *)(packet + 0x23) = len3;
	memcpy(packet + 0x27, head.chars3.v, len3);

}
