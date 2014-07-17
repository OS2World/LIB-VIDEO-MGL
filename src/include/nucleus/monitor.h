/****************************************************************************
*
*           		SciTech Nucleus Graphics Architecture
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
* Description:  Header file for the monitor configuration information
*				for SciTech Nucleus, Graphics Architecture.
*
****************************************************************************/

#ifndef __NUCLEUS_MONITOR_H
#define __NUCLEUS_MONITOR_H

#include "nucleus/common.h"

/*---------------------- Macros and type definitions ----------------------*/

#pragma pack(1)

#define	MONITOR_MFR_LEN		40
#define	MONITOR_MODEL_LEN	60
#define	MDBX_SIG_LEN		15
#define	MDBX_SIGNATURE		"MONITOR.DBX"

/****************************************************************************
REMARKS:
Error codes returned by monitor database routines

HEADER:
nucleus/monitor.h

MEMBERS:
MDBX_ok					- No error
MDBX_fileNotFound		- Database file not found
MDBX_corrupt			- Database is corrupted
MDBX_outOfMemory		- Not enough memory to load DB
MDBX_notFound			- Entry was not found in DB
MDBX_IOError			- Fatal I/O error
MDBX_parseError			- Error parsing monitor database
MDBX_invalidDB			- Database handle is invalid
****************************************************************************/
typedef enum {
	MDBX_ok,
	MDBX_fileNotFound,
	MDBX_corrupt,
	MDBX_outOfMemory,
	MDBX_notFound,
	MDBX_IOError,
	MDBX_parseError,
	MDBX_invalidDB
	} MDBX_errCodes;

/****************************************************************************
REMARKS:
This enumeration defines the flags for the capabilities for monitors as
defined in the GA_monitor record.

HEADER:
nucleus/monitor.h

MEMBERS:
Monitor_DPMSEnabled 	- Monitor supports DPMS Power Management
Monitor_GTFEnabled		- Monitor supports VESA Generalised Timing Formula
Monitor_FixedFreq		- Monitor is a fixed frequency monitor
Monitor_HSyncNeg		- HSync- is required for fixed frequency
Monitor_VSyncNeg		- VSync- is required for fixed frequency
Monitor_16to9			- Monitor supports 16:9 aspect ratio modes
Monitor_Exclude4to3		- Driver should execlude all 4:3 aspect modes
****************************************************************************/
typedef enum {
	Monitor_DPMSEnabled 	= 0x01,
	Monitor_GTFEnabled		= 0x02,
	Monitor_FixedFreq		= 0x04,
	Monitor_HSyncNeg		= 0x08,
	Monitor_VSyncNeg		= 0x10,
	Monitor_16to9			= 0x20,
	Monitor_Exclude4to3		= 0x40,
	} GA_monitorFlagsType;

/****************************************************************************
REMARKS:
Monitor configuration information structure. This structure defines the
capabilities of the attached display monitor, and is used internally
in Nucleus to decide what features the driver should report for the
attached monitor.

HEADER:
nucleus/monitor.h

MEMBERS:
mfr				- Monitor manufacturer (key)
model			- Monitor model name (sub-key)
PNPID			- Plug and Play ID (optional)
maxResolution	- Maximum resolution id
minHScan		- Minimum horizontal scan
maxHScan		- Maximum horizontal scan
minVScan		- Minimum vertical scan
maxVScan		- Maximum vertical scan
flags			- Capabilities flags
****************************************************************************/
typedef struct {
	char	mfr[MONITOR_MFR_LEN+1];
	char	model[MONITOR_MODEL_LEN+1];
	char	PNPID[8];
	uchar	maxResolution;
	uchar	minHScan;
	uchar	maxHScan;
	uchar	minVScan;
	uchar	maxVScan;
	uchar	flags;
	} GA_monitor;

/* {secret} */
typedef struct {
	char	signature[MDBX_SIG_LEN];	/* Signature 					*/
	long    entries;					/* Number of entries in database*/
	/* Data goes in here */
	} MDBX_header;

#pragma pack()

/*------------------------ Function Prototypes ----------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/* Return error code for previous operation. The error code will only be
 * updated if the function actually failed, and is not cleared after
 * being returned.
 */

int		NAPI MDBX_getErrCode(void);
char *	NAPI MDBX_getErrorMsg(void);

/* Routines for reading/writing monitors from the MONITOR.DBX database */

ibool	NAPI MDBX_open(const char *filename,ibool shared);
void	NAPI MDBX_close(void);
int		NAPI MDBX_first(GA_monitor *rec);
int		NAPI MDBX_last(GA_monitor *rec);
int		NAPI MDBX_next(GA_monitor *rec);
int		NAPI MDBX_prev(GA_monitor *rec);
int		NAPI MDBX_insert(GA_monitor *rec);
int 	NAPI MDBX_update(GA_monitor *rec);
int		NAPI MDBX_flush(void);
int		NAPI MDBX_importINF(const char *filename,char *mfr);

#ifdef __cplusplus
}
#endif

#endif	/* __NUCLEUS_MONITOR_H */
