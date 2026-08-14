// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
typedef struct __bfDIR DIR;
struct dirent { unsigned long d_ino; char d_name[256]; };
DIR *opendir(const char *name);
struct dirent *readdir(DIR *d);
int closedir(DIR *d);
