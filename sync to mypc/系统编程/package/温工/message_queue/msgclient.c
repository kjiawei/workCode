#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct msgbuf {
	long mtype;       /* message type, must be > 0 */
	char mtext[10];    /* message data */
};

int main(void)
{
	int msgid;
	key_t key = ftok(".", 1);

	msgid = msgget(key, IPC_CREAT|0666);
	if(msgid == -1)
	{
		printf("get msgid failed\n");
		return -1;
	}

	struct msgbuf mbuf;


	while(1)
	{
		msgrcv(msgid, &mbuf, sizeof(mbuf.mtext), 1, 0);
		printf("mbuf.mtext=%s\n", mbuf.mtext);
	}

	
	return 0;
}
