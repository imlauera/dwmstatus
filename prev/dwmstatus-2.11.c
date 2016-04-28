#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#define WLP_IFACEC "/sys/class/net/wlp4s0/carrier"
#define ENP_IFACEC "/sys/class/net/enp0s25/carrier"
#define CPU0_FREQ "/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq"
#define CPU0_TEMP "/sys/class/hwmon/hwmon1/temp1_input"
#define BAT0_DIR "/sys/class/power_supply/BAT0"
#define BAT0_CAP "/sys/class/power_supply/BAT0/capacity"
#define MEMINFO "/proc/meminfo"

/* Network Connections */
static char* net(void) {
	static FILE *west;
	static FILE *east;
	static unsigned short wlan, eth;

	west = fopen(WLP_IFACEC, "r");
	east = fopen(ENP_IFACEC, "r");

	fscanf(west, "%hu", &wlan);
	fscanf(east, "%hu", &eth);
	fclose(west);
	fclose(east);

	if (wlan == 1)
		return "<--->";
	else if (eth == 1)
		return "[---]";
	else
		return "!-/--";
}

/*static unsigned int mem(void) {
	FILE * meminfo = fopen(MEMINFO, "r");

	if (meminfo == NULL)
		return 1;

	static unsigned int linen = 0, memuse, factor = 1, add;
	static unsigned int memt = 15, memtotal = 0, memava = 0;
	// , unit = 2;
	static char checks;

	for (int i = 0; i < 9; i++) {
		fgetc(meminfo);
	}

	while ((int)checks < 48 || (int)checks > 57) {
		checks = fgetc(meminfo);
		if (checks == ' ')
			memt--;
	}

	for (int i = 0, n = memt - 1; i < n; i++) {
		factor = factor * 10;
	}


	for (int i = 0; i < memt; i++)  {
		add = checks - '0';
		memtotal = memtotal + add * factor;
		factor = factor / 10;
		checks = fgetc(meminfo);
	}

	while (linen < 2) {
		checks = fgetc(meminfo);
		if (checks == '\n')
			linen++;
	}

	for (int i = 0; i < 12; i++) {
		fgetc(meminfo);
	}

	memt = 11;
	checks = fgetc(meminfo);

	while ((int)checks < 48 || (int)checks > 57) {
		checks = fgetc(meminfo);
		if (checks == ' ') {
			memt--;
		}
	}

	factor = 1;
	for (int i = 0, n = memt - 1; i < n; i++) {
		factor = factor * 10;
	}

	for (int i = 0; i < memt; i++)  {
		add = checks - '0';
		memava = memava + add * factor;
		factor = factor / 10;
		checks = fgetc(meminfo);
	}

	fclose(meminfo);
	memuse = (memtotal - memava) / 1024;

	return memuse;

}*/

/* CPU (core0) freq */
static float freq(void) {
	static FILE *freq;
	static float corefreq;

	freq = fopen(CPU0_FREQ, "r");
	fscanf(freq, "%f", &corefreq);
	corefreq = corefreq * 0.000001;
	fclose(freq);
	return corefreq;
}

/* CPU (core0) temp */
static unsigned int temp(void) {
	static FILE *temps;
	static unsigned int coretemp;

	temps = fopen(CPU0_TEMP, "r");
	fscanf(temps, "%ud", &coretemp);
	coretemp = coretemp * 0.001;
	fclose(temps);
	return coretemp;
}

/* Power */
static int power(void) {
	static FILE *supply;
	DIR *dir = opendir(BAT0_DIR);
	static unsigned short charge;
	if (dir) {
		closedir(dir);
		supply = fopen(BAT0_CAP, "r");
		fscanf(supply, "%hu", &charge);
		fclose(supply);
		return charge;
	}

	else {
		return 0;
	}
}

/* Date/time */
static char *unixtime(void) {
	static char buffer[22];
	time_t date;
	static struct tm *tm_info;

	time(&date);
	tm_info = localtime(&date);
	strftime(buffer, sizeof(buffer), "%A %d/%m %I:%M", tm_info);

	return buffer;
}

int main(void) {
//	printf("%s ┃ %uM ┃ %0.1fGHz ┃ %u°C ┃ [%u%%] ┃ %s\n", net(), mem(), freq(), temp(), power(), unixtime());
	printf("%s ┃ %0.1fGHz ┃ %u°C ┃ [%u%%] ┃ %s\n", net(), freq(), temp(), power(), unixtime());
	sleep(2);

	return 0;
}

//#include <linux/sysinfo.h>

//#define MB 1048576
//#define GB 1073741824

//	wlan = getc(west);
//	wlan = wlan - '0';
//	eth = getc(east);
//	eth = eth - '0';
