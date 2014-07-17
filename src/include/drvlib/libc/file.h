/****************************************************************************
*
*           		SciTech Nucleus Graphics Architecture
*
*               Copyright (C) 1991-1998 SciTech Software, Inc.
*                            All rights reserved.
*
*					  Portions Copyright P.J. Plauger.
*
*  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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
*  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* Language:     ANSI C
* Environment:  Any
*
* Description:  Header file for the internal libc runtime library exports.
*
****************************************************************************/

#ifndef __LIBC_FILE_H
#define __LIBC_FILE_H

#ifndef	__WIN32_VXD__
#include <sys/types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define ___O_RDONLY     0x0001
#define ___O_WRONLY     0x0002
#define ___O_RDWR       0x0004

#define ___O_BINARY     0x0008
#define ___O_TEXT       0x0010
#define ___O_CREAT      0x0100
#define ___O_EXCL       0x0200
#define ___O_TRUNC      0x0800
#define ___O_APPEND  	0x1000

#ifndef	__BUILDING_PE_LOADER__

#define F_OK			0x00
#define R_OK			0x04
#define W_OK			0x02
#define X_OK			0x01
#define D_OK			0x00

#define O_RDONLY		___O_RDONLY
#define O_WRONLY		___O_WRONLY
#define O_RDWR			___O_RDWR

#define O_BINARY		___O_BINARY
#define O_TEXT			___O_TEXT
#define O_CREAT			___O_CREAT
#define O_EXCL			___O_EXCL
#define O_TRUNC			___O_TRUNC
#define O_APPEND		___O_APPEND

int     _CDECL open(const char *_path, int _oflag, unsigned _mode);
int   	_CDECL access(const char *_path, int _amode);
int     _CDECL close(int _fildes);
off_t   _CDECL lseek(int _fildes, off_t _offset, int _whence);
size_t	_CDECL read(int _fildes, void *_buf, size_t _nbyte);
int     _CDECL unlink(const char *_path);
size_t 	_CDECL write(int _fildes, const void *_buf, size_t _nbyte);
int    	_CDECL isatty(int _fildes);

#define _Fclose(str)			close((str)->_Handle)
#define _Fread(str, buf, cnt)	read((str)->_Handle, buf, cnt)
#define _Fwrite(str, buf, cnt)	write((str)->_Handle, buf, cnt)

#endif /* !__BUILDING_PE_LOADER__ */

#ifdef __cplusplus
}
#endif

#endif /* __LIBC_FILE_H */
