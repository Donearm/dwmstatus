/*
 * TEMPERATURE STUFF
 */

/**
 * Gets the temperature of your system.
 * Takes the sensor base and sensor as arguments.
 */

char *
gettemperature(char *base, char *sensor)
{
    char *co, *ret;

    co = readfile(base, sensor);
    if (co == NULL) {
        free(co);
        return smprintf("");
    }
    double temp = atof(co);
    free(co);
    ret = smprintf("%02.0f°C", temp / 1000);
    return ret;
}
