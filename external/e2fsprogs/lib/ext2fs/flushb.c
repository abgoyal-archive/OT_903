
#include <stdio.h>
#if HAVE_ERRNO_H
#include <errno.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#if HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif
#if HAVE_SYS_MOUNT_H
#include <sys/param.h>
#include <sys/mount.h>		/* This may define BLKFLSBUF */
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

#ifdef __linux__
#ifndef BLKFLSBUF
#define BLKFLSBUF	_IO(0x12,97)	/* flush buffer cache */
#endif
#ifndef FDFLUSH
#define FDFLUSH		_IO(2,0x4b)	/* flush floppy disk */
#endif
#endif

errcode_t ext2fs_sync_device(int fd, int flushb)
{
	/*
	 * We always sync the device in case we're running on old
	 * kernels for which we can lose data if we don't.  (There
	 * still is a race condition for those kernels, but this
	 * reduces it greatly.)
	 */
	if (fsync (fd) == -1)
		return errno;

	if (flushb) {

#ifdef BLKFLSBUF
		if (ioctl (fd, BLKFLSBUF, 0) == 0)
			return 0;
#else
#ifdef __GNUC__
 #warning BLKFLSBUF not defined
#endif /* __GNUC__ */
#endif
#ifdef FDFLUSH
		ioctl (fd, FDFLUSH, 0);   /* In case this is a floppy */
#else
#ifdef __GNUC__
 #warning FDFLUSH not defined
#endif /* __GNUC__ */
#endif
	}
	return 0;
}
