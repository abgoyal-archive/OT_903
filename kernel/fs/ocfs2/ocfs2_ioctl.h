

#ifndef OCFS2_IOCTL_H
#define OCFS2_IOCTL_H

#define OCFS2_IOC_GETFLAGS	_IOR('f', 1, long)
#define OCFS2_IOC_SETFLAGS	_IOW('f', 2, long)
#define OCFS2_IOC32_GETFLAGS	_IOR('f', 1, int)
#define OCFS2_IOC32_SETFLAGS	_IOW('f', 2, int)

struct ocfs2_space_resv {
	__s16		l_type;
	__s16		l_whence;
	__s64		l_start;
	__s64		l_len;		/* len == 0 means until end of file */
	__s32		l_sysid;
	__u32		l_pid;
	__s32		l_pad[4];	/* reserve area			    */
};

#define OCFS2_IOC_ALLOCSP		_IOW ('X', 10, struct ocfs2_space_resv)
#define OCFS2_IOC_FREESP		_IOW ('X', 11, struct ocfs2_space_resv)
#define OCFS2_IOC_RESVSP		_IOW ('X', 40, struct ocfs2_space_resv)
#define OCFS2_IOC_UNRESVSP	_IOW ('X', 41, struct ocfs2_space_resv)
#define OCFS2_IOC_ALLOCSP64	_IOW ('X', 36, struct ocfs2_space_resv)
#define OCFS2_IOC_FREESP64	_IOW ('X', 37, struct ocfs2_space_resv)
#define OCFS2_IOC_RESVSP64	_IOW ('X', 42, struct ocfs2_space_resv)
#define OCFS2_IOC_UNRESVSP64	_IOW ('X', 43, struct ocfs2_space_resv)

/* Used to pass group descriptor data when online resize is done */
struct ocfs2_new_group_input {
	__u64 group;		/* Group descriptor's blkno. */
	__u32 clusters;		/* Total number of clusters in this group */
	__u32 frees;		/* Total free clusters in this group */
	__u16 chain;		/* Chain for this group */
	__u16 reserved1;
	__u32 reserved2;
};

#define OCFS2_IOC_GROUP_EXTEND	_IOW('o', 1, int)
#define OCFS2_IOC_GROUP_ADD	_IOW('o', 2,struct ocfs2_new_group_input)
#define OCFS2_IOC_GROUP_ADD64	_IOW('o', 3,struct ocfs2_new_group_input)

/* Used to pass 2 file names to reflink. */
struct reflink_arguments {
	__u64 old_path;
	__u64 new_path;
	__u64 preserve;
};
#define OCFS2_IOC_REFLINK	_IOW('o', 4, struct reflink_arguments)

#endif /* OCFS2_IOCTL_H */
