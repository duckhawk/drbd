/*
  drbd.h
  Kernel module for 2.2.x Kernels
  
  This file is part of drbd by Philipp Reisner.

  drbd is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.
  
  drbd is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with drbd; see the file COPYING.  If not, write to
  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

*/
#ifndef DRBD_H
#define DRBD_H

#include <asm/types.h>

#ifdef __KERNEL__
#include <linux/types.h>
#include <linux/ioctl.h>
#else
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <limits.h>
#endif

#ifdef __KERNEL__
#define IN const
#define OUT
#define INOUT
#else
#define IN
#define OUT const
#define INOUT
#endif                                 


#define MAX_SOCK_ADDR	128		/* 108 for Unix domain - 
					   16 for IP, 16 for IPX,
					   24 for IPv6,
					   about 80 for AX.25 
					   must be at least one bigger than
					   the AF_UNIX size (see net/unix/af_unix.c
					   :unix_mkname()).  
					 */

struct disk_config {
	IN int      lower_device;
	IN unsigned int disk_size;  /* Size given by user */
        IN int      do_panic;  /* Panic on error upon LL_DEV */
};

struct net_config {
	IN char     my_addr[MAX_SOCK_ADDR];
	IN int      my_addr_len;
	IN char     other_addr[MAX_SOCK_ADDR];
	IN int      other_addr_len;
	IN int      timeout;
	IN int      sync_rate; /* KB/sec */
	IN int      skip_sync; 
	IN int      tl_size; /* size of the transfer log */
	IN int      wire_protocol;  
	IN int      try_connect_int;  /* seconds */
	IN int      ping_int;         /* seconds */
};

enum ret_codes { 
	NoError=0,
	LAAlreadyInUse,
	OAAlreadyInUse,
	LDFDInvalid,
	LDAlreadyInUse,
	LDNoBlockDev,
	LDOpenFailed,
	LDDeviceTooSmall,
	LDNoConfig
};

struct ioctl_disk_config {
	struct disk_config    config;
	OUT enum ret_codes    ret_code;
};

struct ioctl_net_config {
	struct net_config     config;
	OUT enum ret_codes    ret_code;
};

#define DRBD_PROT_A   1
#define DRBD_PROT_B   2
#define DRBD_PROT_C   3

typedef enum { Unknown=0, Primary=1, Secondary=2 } Drbd_State;
#define PRIMARY_PLUS 5

typedef enum { 
  Unconfigured,
  StandAllone,    
  Unconnected,
  Timeout,
  BrokenPipe,
  WFConnection,
  WFReportParams,     /* The order of these constants is important.   */
  Connected,          /* The lower ones (<WFReportParams) indicate */
  SyncingAll,         /* that there is no socket! */
  SyncingQuick        /* >=WFReportParams ==> There is a socket */
} Drbd_CState; 

struct ioctl_get_config {
	struct net_config     nconf;
	OUT int      lower_device_major;
	OUT int      lower_device_minor;
	OUT unsigned int disk_size_user;
        OUT int      do_panic;
	OUT Drbd_CState cstate; 
};


#define DRBD_MD_PATH   "/var/lib/drbd"
#define DRBD_MD_FILES  DRBD_MD_PATH"/drbd%d"

#define DRBD_MAGIC 0x83740267

#define DRBD_IOCTL_GET_VERSION   _IOR( 'D', 0x00, int )
#define DRBD_IOCTL_SET_STATE     _IOW( 'D', 0x02, Drbd_State )
#define DRBD_IOCTL_WAIT_SYNC     _IOR( 'D', 0x03, int )
#define DRBD_IOCTL_DO_SYNC_ALL   _IO ( 'D', 0x04 )
#define DRBD_IOCTL_SET_DISK_CONFIG _IOW( 'D', 0x06, struct ioctl_disk_config )
#define DRBD_IOCTL_SET_NET_CONFIG _IOW( 'D', 0x07, struct ioctl_net_config )
#define DRBD_IOCTL_UNCONFIG_NET  _IO ( 'D', 0x08 )
#define DRBD_IOCTL_UNCONFIG_BOTH _IO ( 'D', 0x09 )
#define DRBD_IOCTL_GET_CONFIG    _IOW( 'D', 0x0A, struct ioctl_get_config)
#define DRBD_IOCTL_WAIT_CONNECT  _IOR( 'D', 0x0B, int )
#define DRBD_IOCTL_SECONDARY_REM _IOR( 'D', 0x0C, int )

#endif

