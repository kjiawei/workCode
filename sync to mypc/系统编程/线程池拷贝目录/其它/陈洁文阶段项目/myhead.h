#ifndef _MYHEAD_H_
#define _MYHEAD_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <pthread.h>
#include <sys/msg.h>
#include <stdbool.h>

struct flag
{
	char source_name[100];
	char target_name[100];
};

#endif