/* BATTERY USAGE
 * Linux seems to change the filenames after suspend/hibernate
 * according to a random scheme. So just check for both possibilities.
 */

/**
 * Calculates the battery useage and time remaining
 * Takes the base directory to find your battery information
 * as an argument.
 */

char *
getbattery(char *base)
{
    char *co;
    char *stat;
    char *ret;
    char status;
    int descap;
    int remcap;
    double remaining;
    double using;
    double voltage;
    double current;

    descap = -1;
    remcap = -1;
    using  = -1;
    remaining = -1;
    stat = "Not Charging";

    co = readfile(base, "present");
    if (co == NULL || co[0] != '1') {
        if (co != NULL) free(co);
        return smprintf("not present");
    }
    free(co);

    co = readfile(base, "status");
    if (co == NULL) {
        co = "Not Charging";
    }
    sscanf(co, "%s", &status);
    free(co);

    co = readfile(base, "charge_full_design");
    if (co == NULL) {
        co = readfile(base, "energy_full_design");
        if (co == NULL)
            return smprintf("");
    }
    sscanf(co, "%d", &descap);
    free(co);

    co = readfile(base, "charge_now");
    if (co == NULL) {
        co = readfile(base, "energy_now");
        if (co == NULL)
            return smprintf("");
    }
    sscanf(co, "%d", &remcap);
    free(co);

    co = readfile(base, "power_now"); /* µWattage being used */
    if (co == NULL) {
        co = readfile(base, "voltage_now");
        sscanf(co, "%lf", &voltage);
        free(co);
        co = readfile(base, "current_now");
        sscanf(co, "%lf", &current);
        free(co);
        remcap  = (voltage / 1000.0) * ((double)remcap / 1000.0);
        descap  = (voltage / 1000.0) * ((double)descap / 1000.0);
        using = (voltage / 1000.0) * ((double)current / 1000.0);
    } else {
        sscanf(co, "%lf", &using);
        free(co);
    }

    if (remcap < 0 || descap < 0)
        return smprintf("invalid");

    /* Getting time remaining */
    if (status == 'D') {
        remaining = (double)remcap / using;
        stat = "B";
    } else if (status == 'C') {
        remaining = ((double)descap - (double)remcap) /using;
        stat = "C";
    } else {
        remaining = 0;
        stat = "F";
    }
    /* convert to hour:min:sec */
    int hours, seconds, minutes, secs_rem;
    secs_rem = (int)(remaining * 3600.0);
    hours = secs_rem / 3600;
    seconds = secs_rem - (hours * 3600);
    minutes = seconds / 60;
    seconds -= (minutes *60);

    if (seconds < 0 || minutes < 0 || hours < 0)
        ret = smprintf("%s: Calculating...", stat);
    else
        ret = smprintf("%s: %.2lf%% %02d:%02d:%02d", stat,
            (((double)remcap / (double)descap) * 100), hours, minutes, seconds);
    if(!stat) { free(stat); }
    return ret;
}
