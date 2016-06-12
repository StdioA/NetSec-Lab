#ifndef _UTIL_H_
#define _UTIL_H_

struct netstat
{
	unsigned long netmask;
	unsigned long ip;
};
netstat *get_stat(char *iface);
typedef void sigfunc(int);
sigfunc* signal(int signo, sigfunc* func);

#endif

