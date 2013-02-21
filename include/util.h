/* Utilities for dwmstatus.c */

#include <sys/time.h>
#include <time.h>

static Display *dpy;

/**
 * return formatted string
 * see printf(3)
 */

char *
smprintf(char *fmt, ...)
{
    va_list fmtargs;
    char *ret;
    int len;

    va_start(fmtargs, fmt);
    len = vsnprintf(NULL, 0, fmt, fmtargs);
    va_end(fmtargs);

    ret = malloc(++len);
    if (ret == NULL) {
        perror("malloc");
        exit(1);
    }

    va_start(fmtargs, fmt);
    vsnprintf(ret, len, fmt, fmtargs);
    va_end(fmtargs);

    return ret;
}

/**
 * sets your timezone
 */

void
settz(char *tzname)
{
    setenv("TZ", tzname, 1);
}

/**
 * returns the actual time
 */

char *
mktimes(char *fmt, char *tzname)
{
    char buf[129];
    time_t tim;
    struct tm *timtm;

    memset(buf, 0, sizeof(buf));
    settz(tzname);
    tim = time(NULL);
    timtm = localtime(&tim);
    if (timtm == NULL) {
        perror("localtime");
        exit(1);
    }

    if (!strftime(buf, sizeof(buf)-1, fmt, timtm)) {
        fprintf(stderr, "strftime == 0\n");
        exit(1);
    }

    return smprintf("%s", buf);
}

/**
 * for setting the status
 */

void
setstatus(char *str)
{
    XStoreName(dpy, DefaultRootWindow(dpy), str);
    XSync(dpy, False);
}

/**
 * gets and returns the load average as a set of three
 * space separated values
 */

char *
loadavg(void)
{
    double avgs[3];

    if (getloadavg(avgs, 3) < 0) {
        perror("getloadavg");
        exit(1);
    }

    return smprintf("%.2f %.2f %.2f", avgs[0], avgs[1], avgs[2]);
}

/**
 * read a file and return the contents
 * used in battery.h
 */

char *
readfile(char *base, char *file)
{
    char *path, line[513];
    FILE *fd;

    memset(line, 0, sizeof(line));

    path = smprintf("%s/%s", base, file);
    fd = fopen(path, "r");
    if (fd == NULL) {
        free(path);
        return NULL;
    }
    free(path);

    if (fgets(line, sizeof(line)-1, fd) == NULL) {
        fclose(fd);
        return NULL;
    }
    fclose(fd);

    return smprintf("%s", line);
}

/**
 * used to run a command at different intervals than the main
 * status command.
 */

int
runevery(time_t *ltime, int sec)
{
    time_t now = time(NULL);

    if (difftime(now, *ltime) >= sec) {
        *ltime = now;
        return(1);
    }
    else
        return(0);
}
