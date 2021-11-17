#ifndef _BABBLER_FILE_H
#define _BABBLER_FILE_H

#include "babbler.h"
#include <stdbool.h>

#define NAME_MAX 254

struct file_babbler {
	struct babbler b;
	char name[NAME_MAX+1];
	bool is_sysfs;
};


#endif
