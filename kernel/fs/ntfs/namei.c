

#include <linux/dcache.h>
#include <linux/exportfs.h>
#include <linux/security.h>
#include <linux/slab.h>

#include "attrib.h"
#include "debug.h"
#include "dir.h"
#include "mft.h"
#include "ntfs.h"

static struct dentry *ntfs_lookup(struct inode *dir_ino, struct dentry *dent,
		struct nameidata *nd)
{
	ntfs_volume *vol = NTFS_SB(dir_ino->i_sb);
	struct inode *dent_inode;
	ntfschar *uname;
	ntfs_name *name = NULL;
	MFT_REF mref;
	unsigned long dent_ino;
	int uname_len;

	ntfs_debug("Looking up %s in directory inode 0x%lx.",
			dent->d_name.name, dir_ino->i_ino);
	/* Convert the name of the dentry to Unicode. */
	uname_len = ntfs_nlstoucs(vol, dent->d_name.name, dent->d_name.len,
			&uname);
	if (uname_len < 0) {
		if (uname_len != -ENAMETOOLONG)
			ntfs_error(vol->sb, "Failed to convert name to "
					"Unicode.");
		return ERR_PTR(uname_len);
	}
	mref = ntfs_lookup_inode_by_name(NTFS_I(dir_ino), uname, uname_len,
			&name);
	kmem_cache_free(ntfs_name_cache, uname);
	if (!IS_ERR_MREF(mref)) {
		dent_ino = MREF(mref);
		ntfs_debug("Found inode 0x%lx. Calling ntfs_iget.", dent_ino);
		dent_inode = ntfs_iget(vol->sb, dent_ino);
		if (likely(!IS_ERR(dent_inode))) {
			/* Consistency check. */
			if (is_bad_inode(dent_inode) || MSEQNO(mref) ==
					NTFS_I(dent_inode)->seq_no ||
					dent_ino == FILE_MFT) {
				/* Perfect WIN32/POSIX match. -- Case 1. */
				if (!name) {
					ntfs_debug("Done.  (Case 1.)");
					return d_splice_alias(dent_inode, dent);
				}
				/*
				 * We are too indented.  Handle imperfect
				 * matches and short file names further below.
				 */
				goto handle_name;
			}
			ntfs_error(vol->sb, "Found stale reference to inode "
					"0x%lx (reference sequence number = "
					"0x%x, inode sequence number = 0x%x), "
					"returning -EIO. Run chkdsk.",
					dent_ino, MSEQNO(mref),
					NTFS_I(dent_inode)->seq_no);
			iput(dent_inode);
			dent_inode = ERR_PTR(-EIO);
		} else
			ntfs_error(vol->sb, "ntfs_iget(0x%lx) failed with "
					"error code %li.", dent_ino,
					PTR_ERR(dent_inode));
		kfree(name);
		/* Return the error code. */
		return (struct dentry *)dent_inode;
	}
	/* It is guaranteed that @name is no longer allocated at this point. */
	if (MREF_ERR(mref) == -ENOENT) {
		ntfs_debug("Entry was not found, adding negative dentry.");
		/* The dcache will handle negative entries. */
		d_add(dent, NULL);
		ntfs_debug("Done.");
		return NULL;
	}
	ntfs_error(vol->sb, "ntfs_lookup_ino_by_name() failed with error "
			"code %i.", -MREF_ERR(mref));
	return ERR_PTR(MREF_ERR(mref));
	// TODO: Consider moving this lot to a separate function! (AIA)
handle_name:
   {
	MFT_RECORD *m;
	ntfs_attr_search_ctx *ctx;
	ntfs_inode *ni = NTFS_I(dent_inode);
	int err;
	struct qstr nls_name;

	nls_name.name = NULL;
	if (name->type != FILE_NAME_DOS) {			/* Case 2. */
		ntfs_debug("Case 2.");
		nls_name.len = (unsigned)ntfs_ucstonls(vol,
				(ntfschar*)&name->name, name->len,
				(unsigned char**)&nls_name.name, 0);
		kfree(name);
	} else /* if (name->type == FILE_NAME_DOS) */ {		/* Case 3. */
		FILE_NAME_ATTR *fn;

		ntfs_debug("Case 3.");
		kfree(name);

		/* Find the WIN32 name corresponding to the matched DOS name. */
		ni = NTFS_I(dent_inode);
		m = map_mft_record(ni);
		if (IS_ERR(m)) {
			err = PTR_ERR(m);
			m = NULL;
			ctx = NULL;
			goto err_out;
		}
		ctx = ntfs_attr_get_search_ctx(ni, m);
		if (unlikely(!ctx)) {
			err = -ENOMEM;
			goto err_out;
		}
		do {
			ATTR_RECORD *a;
			u32 val_len;

			err = ntfs_attr_lookup(AT_FILE_NAME, NULL, 0, 0, 0,
					NULL, 0, ctx);
			if (unlikely(err)) {
				ntfs_error(vol->sb, "Inode corrupt: No WIN32 "
						"namespace counterpart to DOS "
						"file name. Run chkdsk.");
				if (err == -ENOENT)
					err = -EIO;
				goto err_out;
			}
			/* Consistency checks. */
			a = ctx->attr;
			if (a->non_resident || a->flags)
				goto eio_err_out;
			val_len = le32_to_cpu(a->data.resident.value_length);
			if (le16_to_cpu(a->data.resident.value_offset) +
					val_len > le32_to_cpu(a->length))
				goto eio_err_out;
			fn = (FILE_NAME_ATTR*)((u8*)ctx->attr + le16_to_cpu(
					ctx->attr->data.resident.value_offset));
			if ((u32)(fn->file_name_length * sizeof(ntfschar) +
					sizeof(FILE_NAME_ATTR)) > val_len)
				goto eio_err_out;
		} while (fn->file_name_type != FILE_NAME_WIN32);

		/* Convert the found WIN32 name to current NLS code page. */
		nls_name.len = (unsigned)ntfs_ucstonls(vol,
				(ntfschar*)&fn->file_name, fn->file_name_length,
				(unsigned char**)&nls_name.name, 0);

		ntfs_attr_put_search_ctx(ctx);
		unmap_mft_record(ni);
	}
	m = NULL;
	ctx = NULL;

	/* Check if a conversion error occurred. */
	if ((signed)nls_name.len < 0) {
		err = (signed)nls_name.len;
		goto err_out;
	}
	nls_name.hash = full_name_hash(nls_name.name, nls_name.len);

	dent = d_add_ci(dent, dent_inode, &nls_name);
	kfree(nls_name.name);
	return dent;

eio_err_out:
	ntfs_error(vol->sb, "Illegal file name attribute. Run chkdsk.");
	err = -EIO;
err_out:
	if (ctx)
		ntfs_attr_put_search_ctx(ctx);
	if (m)
		unmap_mft_record(ni);
	iput(dent_inode);
	ntfs_error(vol->sb, "Failed, returning error code %i.", err);
	return ERR_PTR(err);
   }
}

const struct inode_operations ntfs_dir_inode_ops = {
	.lookup	= ntfs_lookup,	/* VFS: Lookup directory. */
};

static struct dentry *ntfs_get_parent(struct dentry *child_dent)
{
	struct inode *vi = child_dent->d_inode;
	ntfs_inode *ni = NTFS_I(vi);
	MFT_RECORD *mrec;
	ntfs_attr_search_ctx *ctx;
	ATTR_RECORD *attr;
	FILE_NAME_ATTR *fn;
	unsigned long parent_ino;
	int err;

	ntfs_debug("Entering for inode 0x%lx.", vi->i_ino);
	/* Get the mft record of the inode belonging to the child dentry. */
	mrec = map_mft_record(ni);
	if (IS_ERR(mrec))
		return (struct dentry *)mrec;
	/* Find the first file name attribute in the mft record. */
	ctx = ntfs_attr_get_search_ctx(ni, mrec);
	if (unlikely(!ctx)) {
		unmap_mft_record(ni);
		return ERR_PTR(-ENOMEM);
	}
try_next:
	err = ntfs_attr_lookup(AT_FILE_NAME, NULL, 0, CASE_SENSITIVE, 0, NULL,
			0, ctx);
	if (unlikely(err)) {
		ntfs_attr_put_search_ctx(ctx);
		unmap_mft_record(ni);
		if (err == -ENOENT)
			ntfs_error(vi->i_sb, "Inode 0x%lx does not have a "
					"file name attribute.  Run chkdsk.",
					vi->i_ino);
		return ERR_PTR(err);
	}
	attr = ctx->attr;
	if (unlikely(attr->non_resident))
		goto try_next;
	fn = (FILE_NAME_ATTR *)((u8 *)attr +
			le16_to_cpu(attr->data.resident.value_offset));
	if (unlikely((u8 *)fn + le32_to_cpu(attr->data.resident.value_length) >
			(u8*)attr + le32_to_cpu(attr->length)))
		goto try_next;
	/* Get the inode number of the parent directory. */
	parent_ino = MREF_LE(fn->parent_directory);
	/* Release the search context and the mft record of the child. */
	ntfs_attr_put_search_ctx(ctx);
	unmap_mft_record(ni);

	return d_obtain_alias(ntfs_iget(vi->i_sb, parent_ino));
}

static struct inode *ntfs_nfs_get_inode(struct super_block *sb,
		u64 ino, u32 generation)
{
	struct inode *inode;

	inode = ntfs_iget(sb, ino);
	if (!IS_ERR(inode)) {
		if (is_bad_inode(inode) || inode->i_generation != generation) {
			iput(inode);
			inode = ERR_PTR(-ESTALE);
		}
	}

	return inode;
}

static struct dentry *ntfs_fh_to_dentry(struct super_block *sb, struct fid *fid,
		int fh_len, int fh_type)
{
	return generic_fh_to_dentry(sb, fid, fh_len, fh_type,
				    ntfs_nfs_get_inode);
}

static struct dentry *ntfs_fh_to_parent(struct super_block *sb, struct fid *fid,
		int fh_len, int fh_type)
{
	return generic_fh_to_parent(sb, fid, fh_len, fh_type,
				    ntfs_nfs_get_inode);
}

const struct export_operations ntfs_export_ops = {
	.get_parent	= ntfs_get_parent,	/* Find the parent of a given
						   directory. */
	.fh_to_dentry	= ntfs_fh_to_dentry,
	.fh_to_parent	= ntfs_fh_to_parent,
};
