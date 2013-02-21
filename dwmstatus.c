#define _BSD_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/wait.h>

#include <X11/Xlib.h>

/* Files */
#include "config.h"
#include "include/util.h"
#include "include/battery.h"
#include "include/network.h"
#include "include/temp.h"

/* 
 * setup the status bar here
 */
int
status(int tostatusbar)
{
    char *status = NULL;
    char *avgs = NULL;
    char *time = NULL;
    char *batt = NULL;
    char *net = NULL;
    char *temp = NULL;
    char *ipaddr = NULL;
    char *net_device_up = NET_DEVICE_PRIMARY;
    time_t count60 = 0;
    time_t count10 = 0;

    if (!(dpy = XOpenDisplay(NULL)) && tostatusbar == 0) {
        fprintf(stderr, "dwmstatus: cannot open display.\n");
        return 1;
    }

    for (;;sleep(0)) {
        /* Update every minute */
        if (runevery(&count60, 60)) {
            free(time);

            time  = mktimes("%Y/%m/%d %H:%M", TIMEZONE);
        }
        /* Update every 10 seconds */
        if (runevery(&count10, 10)) {
            free(avgs);
            free(temp);
            free(batt);

            avgs   = loadavg();
            temp   = gettemperature(TEMP_SENSOR_PATH, TEMP_SENSOR_UNIT);
            batt   = getbattery(BATT_PATH);
            if(!temp) free(temp);
        }
        /* Update every second */
        net    = get_netusage(net_device_up);
        ipaddr = get_ip_addr(net_device_up);

        /* Format of display */
        status = smprintf("%s (%s) | %s [%s] T %s | %s",
                net, ipaddr, batt, avgs, temp, time);
        if(!ipaddr) free(ipaddr);
        free(net);

        if(tostatusbar == 0)
            setstatus(status);
        else
            puts(status);

        free(status);
    }
    return 0;
}

int
main(int argc, char *argv[])
{
    if(argc == 2 && !strcmp("-v", argv[1])) {
        printf(
            "dwmstatus %s © 2012-2013 William Giokas.\n\
              See LICENSE for more details.\n",
                VERSION);
        return 0;
    }
    else if(argc == 2 && !strcmp("-o", argv[1])) {
        status(1);
        return 0;
    }
    else if(argc != 1) {
        printf("usage: dwmstatus [-v] [-o]\n");
        return 1;
    }
    else
        status(0);
    XCloseDisplay(dpy);
    return 0;
}
