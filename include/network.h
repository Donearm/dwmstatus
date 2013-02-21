/* 
 * NETWORK STUFF
 * TODO ip address display (need new func, not in net/dev)
 * TODO connection status (Put in ip address func?)
 */
#include <ifaddrs.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netdb.h>
#include <net/if.h>

#define NI_NUMERICHOST 1

const char
*gai_strerror(int errcode);

int
getnameinfo(const struct sockaddr *sa, socklen_t salen,
        char *host, size_t hostlen,
        char *serv, size_t servlen, int flags);

/**
 * Used to parse the /proc/net/dev file to get the network useage.
 */

int 
parse_netdev(unsigned long long int *receivedabs,
    unsigned long long int *sentabs, char *netdevice)
{
    char *buf;
    char *netstart;
    int netdevlength;
    static int bufsize;
    FILE *devfd;

    buf = (char *) calloc(255, 1);
    bufsize = 255;
    devfd = fopen("/proc/net/dev", "r");

    /* ignore the first two lines of the file */
    fgets(buf, bufsize, devfd);
    fgets(buf, bufsize, devfd);

    while (fgets(buf, bufsize, devfd)) {
        if ((netstart = strstr(buf, netdevice)) != NULL) {
            netdevlength = sizeof(netdevice) / sizeof(char);
            /* Thanks to the conky project: http://conky.sourceforge.net/ */
            sscanf(netstart + netdevlength,
                "%llu  %*d     %*d  %*d  %*d  %*d   %*d        %*d       %llu",
                   receivedabs, sentabs);
            fclose(devfd);
            free(buf);
            return 0;
        }
    }
    fclose(devfd);
    free(buf);
    return 1;
}

/**
 * Calculates the per second network useage.
 * Note: This function takes 1 (one) second to execute,
 * so take that into account in your status formatting.
 * (Subtract one from the refresh rate you want to get
 * the refresh rate you should be using.
 */

char *
get_netusage(char *netdevice)
{
    unsigned long long int oldrec, oldsent, newrec, newsent;
    double downspeed, upspeed;
    char *downspeedstr = NULL;
    char *upspeedstr = NULL;
    char *retstr;
    int retval;

    retval = parse_netdev(&oldrec, &oldsent, netdevice);
    if (retval) {
        fprintf(stdout, "Error when parsing /proc/net/dev file.\n");
        exit(1);
    }

    sleep(1);
    retval = parse_netdev(&newrec, &newsent, netdevice);
    if (retval) {
        fprintf(stdout, "Error when parsing /proc/net/dev file.\n");
        exit(1);
    }

    downspeedstr = (char *) malloc(15);
    upspeedstr = (char *) malloc(15);
    retstr = (char *) malloc(42);

    downspeed = (newrec - oldrec) / 1024.0;
    if (downspeed > 1024.0) {
        downspeed /= 1024.0;
        sprintf(downspeedstr, "%.3fMB/s", downspeed);
    } else {
        sprintf(downspeedstr, "%.2fKB/s", downspeed);
    }

    upspeed = (newsent - oldsent) / 1024.0;
    if (upspeed > 1024.0) {
        upspeed /= 1024.0;
        sprintf(upspeedstr, "%.3fMB/s", upspeed);
    } else {
        sprintf(upspeedstr, "%.2fKB/s", upspeed);
    }
    sprintf(retstr, "%s: d %s u %s", netdevice, downspeedstr, upspeedstr);

    free(downspeedstr);
    free(upspeedstr);
    return retstr;
}

/**
 * Gets your current ip address on 'interface'
 * Shamelessly taken from i3status, because i3status is
 * pretty awesome.
 */
char *
get_ip_addr(const char *interface)
{
    static char part[512];
    socklen_t len = sizeof(struct sockaddr_in);
    memset(part, 0, sizeof(part));
    struct ifaddrs *ifaddr, *addrp;
    bool found = false;

    getifaddrs(&ifaddr);

    if(ifaddr == NULL)
        return NULL;

    for (addrp = ifaddr;
            (addrp != NULL &&
             (strcmp(addrp->ifa_name, interface) != 0 ||
              addrp->ifa_addr == NULL ||
              addrp->ifa_addr->sa_family != AF_INET));
            addrp = addrp->ifa_next) {
            /* check if interface is down/up */
                if (strcmp(addrp->ifa_name, interface) != 0)
                    continue;
            found = true;
            if (strcmp(addrp->ifa_name, interface) != 0) {
                    freeifaddrs(ifaddr);
                    return NULL;
                }
        }

        if (addrp == NULL) {
                freeifaddrs(ifaddr);
                return (found ? "no IP" : NULL);
            }

        int ret;
    if ((ret = getnameinfo(addrp->ifa_addr, len, part, sizeof(part), NULL, 0,
            NI_NUMERICHOST)) != 0) {
            fprintf(stderr, "dwmstatus: getnameinfo(): %s\n",
                gai_strerror(ret));
            freeifaddrs(ifaddr);
            return "no IP";
        }
    freeifaddrs(ifaddr);
    return part;
}
