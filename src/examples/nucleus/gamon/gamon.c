/****************************************************************************
*
*                  SciTech Nucleus Monitor Selection program
*
*               Copyright (C) 1991-1998 SciTech Software, Inc.
*                            All rights reserved.
*
*  ======================================================================
*  |REMOVAL OR MODIFICATION OF THIS HEADER IS STRICTLY PROHIBITED BY LAW|
*  |                                                                    |
*  |This copyrighted computer code contains proprietary technology      |
*  |owned by SciTech Software, Inc., located at 505 Wall Street,        |
*  |Chico, CA 95928 USA (http://www.scitechsoft.com).                   |
*  |                                                                    |
*  |The contents of this file are subject to the SciTech Nucleus        |
*  |License; you may *not* use this file or related software except in  |
*  |compliance with the License. You may obtain a copy of the License   |
*  |at http://www.scitechsoft.com/nucleus-license.txt                   |
*  |                                                                    |
*  |Software distributed under the License is distributed on an         |
*  |"AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or      |
*  |implied. See the License for the specific language governing        |
*  |rights and limitations under the License.                           |
*  |                                                                    |
*  |REMOVAL OR MODIFICATION OF THIS HEADER IS STRICTLY PROHIBITED BY LAW|
*  ======================================================================
*
* Language:     ANSI C
* Environment:  Any 32-bit protected mode environment
*
* Description:  Simple program that allows a monitor to be selected and
*				made the active monitor for Nucleus programs.
*
****************************************************************************/

#include "scitech.h"
#include "nucleus/graphics.h"
#include "pmapi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef	__LINUX__
#define	stricmp	strcasecmp
#define	strnicmp strncasecmp
#endif
#ifndef	_MAX_PATH
#define	_MAX_PATH	256
#endif

/*----------------------------- Implementation ----------------------------*/

/****************************************************************************
REMARKS:
Display the sign on banner.
****************************************************************************/
static void banner(void)
{
	printf("GAMonitor - SciTech Nucleus Monitor Selection Program\n");
	printf("            Copyright (C) 1998-1999 SciTech Software, Inc.\n");
	printf("\n");
}

/****************************************************************************
REMARKS:
Display the help screen.
****************************************************************************/
static void help(void)
{
	banner();
	printf("Valid commands are:\n");
	printf("\n");
	printf("  gamon mfr [mfrname]\n");
	printf("\n");
	printf("     To list all known monitor manufacturer names. You can optionally include\n");
	printf("     a partial manufacturer name that will be used to narrow the search.\n");
	printf("\n");
	printf("  gamon model <mfr>\n");
	printf("\n");
	printf("     To list all known monitor models for a specific manufacturer.\n");
	printf("\n");
	printf("  gamon select <mfr> <model> [device]\n");
	printf("\n");
	printf("     To select a monitor for the specified device. If the device number\n");
	printf("     is not included, we assume the default monitor (ie: device 0). Note also\n");
	printf("     that the passed in manufacturer and model strings can be partial matches.\n");
	printf("\n");
	printf("  gamon enable|disable <DPMS | GTF | 16:9 | 4:3> [device]\n");
	printf("\n");
	printf("     To enable or disable specific features for the selected monitor.\n");
	printf("\n");
	printf("  gamon import [filename.INF]\n");
	printf("\n");
	printf("     To import a Windows INF monitor file.\n");
	exit(-1);
}

/****************************************************************************
REMARKS:
Open the monitor database on disk.
****************************************************************************/
static void OpenDB(void)
{
	char	filename[_MAX_PATH];

	/* Open the monitor database */
	strcpy(filename,PM_getNucleusConfigPath());
	PM_backslash(filename);
	strcat(filename,"monitor.dbx");
	if (!MDBX_open(filename,true)) {
		printf("Unable to open monitor database!\n");
		exit(-1);
		}
}

/****************************************************************************
PARAMETERS:
deviceId	- Device ID to open the monitor configuraiton file for
mode		- Mode to open the file in (passed to fopen)

REMARKS:
Open the monitor configuration file on disk.
****************************************************************************/
static FILE *OpenMonitor(
	int deviceId,
	char *mode)
{
	char	buf[_MAX_PATH],filename[_MAX_PATH];

	/* Open the monitor configuration file for writing*/
	sprintf(buf,"mon%02d.dat", deviceId);
	strcpy(filename,PM_getNucleusConfigPath());
	PM_backslash(filename);
	strcat(filename,buf);
	return fopen(filename,mode);
}

/****************************************************************************
PARAMETERS:
mfrFilter	- String to filter manufacturers on

REMARKS:
Display the list of all manufacturers in the database for a given string.
****************************************************************************/
static void ListMFR(
	const char *mfrFilter)
{
	int			len = strlen(mfrFilter);
	char		mfr[MONITOR_MFR_LEN+1] = "";
	GA_monitor	monitor;

	printf("List of monitor manufacturer names:\n\n");
	OpenDB();
	if (MDBX_first(&monitor) == MDBX_ok) {
		do {
			if (strcmp(monitor.mfr,mfr) != 0) {
				strcpy(mfr,monitor.mfr);
				if (strnicmp(mfr,mfrFilter,len) == 0)
					printf("%s\n", mfr);
				}
			} while (MDBX_next(&monitor) == MDBX_ok);
		}
	MDBX_close();
}

/****************************************************************************
PARAMETERS:
mfrFilter	- String to filter manufacturers on

REMARKS:
Display the list of all models in the database for a given string.
****************************************************************************/
static void ListModel(
	const char *mfrFilter)
{
	int			len = strlen(mfrFilter);
	GA_monitor	monitor;

	printf("List of monitor model names:\n\n");
	OpenDB();
	if (MDBX_first(&monitor) == MDBX_ok) {
		do {
			if (len == 0) {
				printf("%-7s %s: %s\n", monitor.PNPID, monitor.mfr, monitor.model);
				}
			else if (strnicmp(monitor.mfr,mfrFilter,len) == 0) {
				printf("%s: %s\n", monitor.mfr, monitor.model);
				}
			} while (MDBX_next(&monitor) == MDBX_ok);
		}
	MDBX_close();
}

/****************************************************************************
PARAMETERS:
mfrFilter	- String to filter manufacturers on
modelFilter	- String to filter models on

REMARKS:
Select a particular model and write the monitor information to disk.
****************************************************************************/
static void SelectModel(
	const char *mfrFilter,
	const char *modelFilter,
	int deviceId)
{
	FILE		*f;
	int			mfrLen = strlen(mfrFilter),found = false;
	char		model[80],filter[80];
	GA_monitor	monitor;

	/* Find the monitor in the database */
	strcpy(filter,modelFilter);
	strlwr(filter);
	OpenDB();
	if (MDBX_first(&monitor) == MDBX_ok) {
		do {
			strcpy(model,monitor.model);
			strlwr(model);
			if (strnicmp(monitor.mfr,mfrFilter,mfrLen) == 0 && strstr(model,filter) != NULL) {
				found = true;
				break;
				}
			} while (MDBX_next(&monitor) == MDBX_ok);
		}
	MDBX_close();

	/* Now write the selected record to disk */
	if (found) {
		if ((f = OpenMonitor(deviceId,"wb")) == NULL) {
			printf("Unable to write monitor configuration file!\n");
			exit(-1);
			}
		printf("Selected monitor (device %d):\n\n", deviceId);
		printf("%s: %s\n", monitor.mfr, monitor.model);
		fwrite(&monitor,1,sizeof(monitor),f);
		fclose(f);
		}
	else
		printf("Monitor not found!\n");
}

/****************************************************************************
REMARKS:
Return the string for the maximum resolution for the monitor.
****************************************************************************/
const char *GetMaxRes(
	int maxRes)
{
	switch (maxRes) {
		case MaxRes_640x480:	return "640 x 480";		break;
		case MaxRes_800x600:	return "800 x 600";		break;
		case MaxRes_1024x768:	return "1024 x 768";	break;
		case MaxRes_1152x864:	return "1152 x 864";	break;
		case MaxRes_1280x1024:	return "1280 x 1024";	break;
		case MaxRes_1600x1200:	return "1600 x 1200";	break;
		case MaxRes_1800x1350:	return "1800 x 1350";	break;
		case MaxRes_1920x1440:	return "1920 x 1440";	break;
		case MaxRes_2048x1536:	return "2048 x 1536";	break;
		}
	return "Unknown!";
}

/****************************************************************************
REMARKS:
Displays the currently select monitor information.
****************************************************************************/
ibool DisplayMonitor(
	int deviceId)
{
	FILE		*f;
	GA_monitor	monitor;
	ibool		comma;

	if ((f = OpenMonitor(deviceId,"rb")) != NULL) {
		fread(&monitor,1,sizeof(monitor),f);
		fclose(f);
		printf("\nMonitor %d:\n", deviceId);
		printf("  Manufacturer:    %s\n", monitor.mfr);
		printf("  Model:           %s\n",monitor.model);
		printf("  Max Resolution:  %s\n",GetMaxRes(monitor.maxResolution));
		printf("  Horizontal Scan: %dKHz - %dKHz\n",monitor.minHScan,monitor.maxHScan);
		printf("  Vertical Scan:   %dHz - %dHz\n",monitor.minVScan,monitor.maxVScan);
		if (monitor.flags & (Monitor_DPMSEnabled | Monitor_GTFEnabled | Monitor_16to9)) {
			printf("  Capabilities:    ");
			comma = false;
			if (monitor.flags & Monitor_DPMSEnabled) {
				printf("DPMS");
				comma = true;
				}
			if (monitor.flags & Monitor_GTFEnabled) {
				if (comma)
					printf(", ");
				printf("GTF");
				comma = true;
				}
			if (monitor.flags & Monitor_16to9) {
				if (comma)
					printf(", ");
				printf("16:9");
				comma = true;
				}
			if (monitor.flags & Monitor_Exclude4to3) {
				if (comma)
					printf(", ");
				printf("Exclude 4:3");
				}
			printf("\n");
			}
		return true;
		}
	return false;
}

/****************************************************************************
REMARKS:
Enable a specific feature for the monitor
****************************************************************************/
void EnableFeature(
	char *feature,
	int enable,
	int deviceId)
{
	FILE 		*f;
	GA_monitor	monitor;

	if ((f = OpenMonitor(deviceId,"rb")) != NULL) {
		fread(&monitor,1,sizeof(monitor),f);
		fclose(f);
		if (stricmp(feature,"DPMS") == 0) {
			if (enable)
				monitor.flags |= Monitor_DPMSEnabled;
			else
				monitor.flags &= ~Monitor_DPMSEnabled;
			}
		if (stricmp(feature,"GTF") == 0) {
			if (enable)
				monitor.flags |= Monitor_GTFEnabled;
			else
				monitor.flags &= ~Monitor_GTFEnabled;
			}
		if (stricmp(feature,"16:9") == 0) {
			if (enable)
				monitor.flags |= Monitor_16to9;
			else
				monitor.flags &= ~Monitor_16to9;
			}
		if (stricmp(feature,"4:3") == 0) {
			if (enable)
				monitor.flags |= Monitor_Exclude4to3;
			else
				monitor.flags &= ~Monitor_Exclude4to3;
			}
		if ((f = OpenMonitor(deviceId,"wb")) == NULL) {
			printf("Unable to write monitor configuration file!\n");
			exit(-1);
			}
		fwrite(&monitor,1,sizeof(monitor),f);
		fclose(f);
		DisplayMonitor(deviceId);
		}
	else {
		printf("Monitor not selected for device!\n");
		}
}

/****************************************************************************
REMARKS:
Imports a Windows INF file.
****************************************************************************/
void ImportINF(
	char *INFPath)
{
	char	filename[_MAX_PATH];

	strcpy(filename,PM_getNucleusConfigPath());
	PM_backslash(filename);
	strcat(filename,"monitor.dbx");
	if (!MDBX_open(filename,false))
		PM_fatalError(MDBX_getErrorMsg());
	if (MDBX_importINF(INFPath,NULL) != MDBX_ok)
		PM_fatalError(MDBX_getErrorMsg());
	if (MDBX_flush() != MDBX_ok)
		PM_fatalError(MDBX_getErrorMsg());
	MDBX_close();
	printf("INF file successfully imported!");
}

/****************************************************************************
REMARKS:
Displays the currently select monitor information.
****************************************************************************/
void DisplayCurrent(void)
{
	int	deviceId,found = false;

	for (deviceId = 0; deviceId < GA_MAX_DEVICES; deviceId++) {
		if (DisplayMonitor(deviceId))
			found = true;
		}
	if (!found)
		help();
}

/****************************************************************************
REMARKS:
Main program entry point
****************************************************************************/
int main(
	int argc,
	char *argv[])
{
	/* Now handle the query */
	if (argc >= 2 && stricmp(argv[1],"mfr") == 0) {
		if (argc > 2)
			ListMFR(argv[2]);
		else
			ListMFR("");
		}
	else if (argc == 3 && stricmp(argv[1],"model") == 0) {
		if (stricmp(argv[2],"all") == 0)
			ListModel("");
		else
			ListModel(argv[2]);
		}
	else if (argc >= 4 && stricmp(argv[1],"select") == 0) {
		if (argc == 4)
			SelectModel(argv[2],argv[3],0);
		else
			SelectModel(argv[2],argv[3],atoi(argv[4]));
		}
	else if (argc >= 3 && stricmp(argv[1],"enable") == 0) {
		if (argc == 3)
			EnableFeature(argv[2],true,0);
		else
			EnableFeature(argv[2],true,atoi(argv[3]));
		}
	else if (argc >= 3 && stricmp(argv[1],"disable") == 0) {
		if (argc == 3)
			EnableFeature(argv[2],false,0);
		else
			EnableFeature(argv[2],false,atoi(argv[3]));
		}
	else if (argc == 3 && stricmp(argv[1],"import") == 0) {
		ImportINF(argv[2]);
		}
	else if (argc >= 2)
		help();
	else
		DisplayCurrent();
	return 0;
}
