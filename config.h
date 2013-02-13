/* See LICENSE file for copyright and license details */

/************************
 * Configuration Options
 ************************/

/* Specify your timezone: */
#define TIMEZONE "Europe/Rome"

/* Specify network device, usually wlan0 or eth0
 * systemd has some new naming conventions you can turn on with longer names.
 */
#define NET_DEVICE_PRIMARY "eth0"
/* Currently not of any use 
#define NET_DEVICE_SECONDARY "enp0s25" */

/* Specity path to your battery */
#define BATT_PATH "/sys/class/power_supply/BAT0"

/* Specify your temperature sensor information. */
#define TEMP_SENSOR_PATH "/sys/class/hwmon/hwmon0/device"
#define TEMP_SENSOR_UNIT "temp3_input"
