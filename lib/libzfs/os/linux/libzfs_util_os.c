/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */


#include <errno.h>
#include <fcntl.h>
#include <libintl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/mnttab.h>
#include <sys/mntent.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <libzfs.h>
#include <libzfs_core.h>

static int zfs_major_ver, zfs_minor_ver;

#include "libzfs_impl.h"
#include "zfs_prop.h"
#include <libzutil.h>
#include <sys/zfs_sysfs.h>

#define	ZDIFF_SHARESDIR		"/.zfs/shares/"

int
zfs_ioctl(libzfs_handle_t *hdl, int request, zfs_cmd_t *zc)
{
	zfs_cmd_V065_t zc_065;
	int rc;

	if ((zfs_major_ver * 100) + zfs_minor_ver >= 7)
		return (ioctl(hdl->libzfs_fd, request, zc));

	memcpy(zc_065.zc_name, zc->zc_name, sizeof zc_065.zc_name);
	zc_065.zc_nvlist_src = zc->zc_nvlist_src;
	zc_065.zc_nvlist_src_size = zc->zc_nvlist_src_size;
	zc_065.zc_nvlist_dst = zc->zc_nvlist_dst;
	zc_065.zc_nvlist_dst_size = zc->zc_nvlist_dst_size;
	zc_065.zc_nvlist_dst_filled = zc->zc_nvlist_dst_filled;
	zc_065.zc_pad2 = zc->zc_pad2;
	zc_065.zc_history = zc->zc_history;
	memcpy(zc_065.zc_value, zc->zc_value, sizeof zc_065.zc_value);
	memcpy(zc_065.zc_string, zc->zc_string, sizeof zc_065.zc_string);
	zc_065.zc_guid = zc->zc_guid;
	zc_065.zc_nvlist_conf = zc->zc_nvlist_conf;
	zc_065.zc_nvlist_conf_size = zc->zc_nvlist_conf_size;
	zc_065.zc_cookie = zc->zc_cookie;
	zc_065.zc_objset_type = zc->zc_objset_type;
	zc_065.zc_perm_action = zc->zc_perm_action;
	zc_065.zc_history_len = zc->zc_history_len;
	zc_065.zc_history_offset = zc->zc_history_offset;
	zc_065.zc_obj = zc->zc_obj;
	zc_065.zc_iflags = zc->zc_iflags;
	zc_065.zc_share = zc->zc_share;
	zc_065.zc_objset_stats = zc->zc_objset_stats;
	zc_065.zc_begin_record = zc->zc_begin_record;

	zc_065.zc_inject_record.zi_objset = zc->zc_inject_record.zi_objset;
	zc_065.zc_inject_record.zi_object = zc->zc_inject_record.zi_object;
	zc_065.zc_inject_record.zi_start = zc->zc_inject_record.zi_start;
	zc_065.zc_inject_record.zi_end = zc->zc_inject_record.zi_end;
	zc_065.zc_inject_record.zi_guid = zc->zc_inject_record.zi_guid;
	zc_065.zc_inject_record.zi_level = zc->zc_inject_record.zi_level;
	zc_065.zc_inject_record.zi_error = zc->zc_inject_record.zi_error;
	zc_065.zc_inject_record.zi_type = zc->zc_inject_record.zi_type;
	zc_065.zc_inject_record.zi_freq = zc->zc_inject_record.zi_freq;
	zc_065.zc_inject_record.zi_failfast = zc->zc_inject_record.zi_failfast;
	memcpy(zc_065.zc_inject_record.zi_func, zc->zc_inject_record.zi_func, sizeof zc_065.zc_inject_record.zi_func);
	zc_065.zc_inject_record.zi_iotype = zc->zc_inject_record.zi_iotype;
	zc_065.zc_inject_record.zi_duration = zc->zc_inject_record.zi_duration;
	zc_065.zc_inject_record.zi_timer = zc->zc_inject_record.zi_timer;
	zc_065.zc_inject_record.zi_cmd = zc->zc_inject_record.zi_cmd;
	zc_065.zc_inject_record.zi_pad = 0;

	zc_065.zc_defer_destroy = zc->zc_defer_destroy;
	zc_065.zc_flags = zc->zc_flags;
	zc_065.zc_action_handle = zc->zc_action_handle;
	zc_065.zc_cleanup_fd = zc->zc_cleanup_fd;
	zc_065.zc_simple = zc->zc_simple;
	memcpy(zc_065.zc_pad, zc->zc_pad, sizeof zc_065.zc_pad);
	zc_065.zc_sendobj = zc->zc_sendobj;
	zc_065.zc_fromobj = zc->zc_fromobj;
	zc_065.zc_createtxg = zc->zc_createtxg;
	zc_065.zc_stat = zc->zc_stat;

	rc = ioctl(hdl->libzfs_fd, request, &zc_065);

	memcpy(zc->zc_name, zc_065.zc_name, sizeof zc->zc_name);
	zc->zc_nvlist_src = zc_065.zc_nvlist_src;
	zc->zc_nvlist_src_size = zc_065.zc_nvlist_src_size;
	zc->zc_nvlist_dst = zc_065.zc_nvlist_dst;
	zc->zc_nvlist_dst_size = zc_065.zc_nvlist_dst_size;
	zc->zc_nvlist_dst_filled = zc_065.zc_nvlist_dst_filled;
	zc->zc_pad2 = zc_065.zc_pad2;
	zc->zc_history = zc_065.zc_history;
	memcpy(zc->zc_value, zc_065.zc_value, sizeof zc->zc_value);
	memcpy(zc->zc_string, zc_065.zc_string, sizeof zc->zc_string);
	zc->zc_guid = zc_065.zc_guid;
	zc->zc_nvlist_conf = zc_065.zc_nvlist_conf;
	zc->zc_nvlist_conf_size = zc_065.zc_nvlist_conf_size;
	zc->zc_cookie = zc_065.zc_cookie;
	zc->zc_objset_type = zc_065.zc_objset_type;
	zc->zc_perm_action = zc_065.zc_perm_action;
	zc->zc_history_len = zc_065.zc_history_len;
	zc->zc_history_offset = zc_065.zc_history_offset;
	zc->zc_obj = zc_065.zc_obj;
	zc->zc_iflags = zc_065.zc_iflags;
	zc->zc_share = zc_065.zc_share;
	zc->zc_objset_stats = zc_065.zc_objset_stats;
	zc->zc_begin_record = zc_065.zc_begin_record;

	zc->zc_inject_record.zi_objset = zc_065.zc_inject_record.zi_objset;
	zc->zc_inject_record.zi_object = zc_065.zc_inject_record.zi_object;
	zc->zc_inject_record.zi_start = zc_065.zc_inject_record.zi_start;
	zc->zc_inject_record.zi_end = zc_065.zc_inject_record.zi_end;
	zc->zc_inject_record.zi_guid = zc_065.zc_inject_record.zi_guid;
	zc->zc_inject_record.zi_level = zc_065.zc_inject_record.zi_level;
	zc->zc_inject_record.zi_error = zc_065.zc_inject_record.zi_error;
	zc->zc_inject_record.zi_type = zc_065.zc_inject_record.zi_type;
	zc->zc_inject_record.zi_freq = zc_065.zc_inject_record.zi_freq;
	zc->zc_inject_record.zi_failfast = zc_065.zc_inject_record.zi_failfast;
	memcpy(zc->zc_inject_record.zi_func, zc_065.zc_inject_record.zi_func, sizeof zc->zc_inject_record.zi_func);
	zc->zc_inject_record.zi_iotype = zc_065.zc_inject_record.zi_iotype;
	zc->zc_inject_record.zi_duration = zc_065.zc_inject_record.zi_duration;
	zc->zc_inject_record.zi_timer = zc_065.zc_inject_record.zi_timer;
	zc->zc_inject_record.zi_nlanes = 0;
	zc->zc_inject_record.zi_cmd = zc_065.zc_inject_record.zi_cmd;
	zc->zc_inject_record.zi_dvas = 0;

	zc->zc_defer_destroy = zc_065.zc_defer_destroy;
	zc->zc_flags = zc_065.zc_flags;
	zc->zc_action_handle = zc_065.zc_action_handle;
	zc->zc_cleanup_fd = zc_065.zc_cleanup_fd;
	zc->zc_simple = zc_065.zc_simple;
	memcpy(zc->zc_pad, zc_065.zc_pad, sizeof zc->zc_pad);
	zc->zc_sendobj = zc_065.zc_sendobj;
	zc->zc_fromobj = zc_065.zc_fromobj;
	zc->zc_createtxg = zc_065.zc_createtxg;
	zc->zc_stat = zc_065.zc_stat;

	return rc;
}

const char *
libzfs_error_init(int error)
{
	switch (error) {
	case ENXIO:
		return (dgettext(TEXT_DOMAIN, "The ZFS modules are not "
		    "loaded.\nTry running '/sbin/modprobe zfs' as root "
		    "to load them."));
	case ENOENT:
		return (dgettext(TEXT_DOMAIN, "/dev/zfs and /proc/self/mounts "
		    "are required.\nTry running 'udevadm trigger' and 'mount "
		    "-t proc proc /proc' as root."));
	case ENOEXEC:
		return (dgettext(TEXT_DOMAIN, "The ZFS modules cannot be "
		    "auto-loaded.\nTry running '/sbin/modprobe zfs' as "
		    "root to manually load them."));
	case EACCES:
		return (dgettext(TEXT_DOMAIN, "Permission denied the "
		    "ZFS utilities must be run as root."));
	default:
		return (dgettext(TEXT_DOMAIN, "Failed to initialize the "
		    "libzfs library."));
	}
}

static int
libzfs_module_loaded(const char *module)
{
	FILE *fp;
	const char path_prefix[] = "/sys/module/";
	char path[256];

	memcpy(path, path_prefix, sizeof (path_prefix) - 1);
	strcpy(path + sizeof (path_prefix) - 1, module);

	strcpy(path + sizeof (path_prefix) - 1 + strlen(module), "/version");
	fp = fopen(path, "r");
	if (fp) {
		if (fscanf(fp, "%d.%d", &zfs_major_ver, &zfs_minor_ver) != 2) {
			zfs_major_ver = 0;
			zfs_minor_ver = 0;
		}
		fclose(fp);
	}

	return (access(path, F_OK) == 0);
}

/*
 * Verify the required ZFS_DEV device is available and optionally attempt
 * to load the ZFS modules.  Under normal circumstances the modules
 * should already have been loaded by some external mechanism.
 *
 * Environment variables:
 * - ZFS_MODULE_LOADING="YES|yes|ON|on" - Attempt to load modules.
 * - ZFS_MODULE_TIMEOUT="<seconds>"     - Seconds to wait for ZFS_DEV
 */
static int
libzfs_load_module_impl(const char *module)
{
	char *argv[4] = {"/sbin/modprobe", "-q", (char *)module, (char *)0};
	char *load_str, *timeout_str;
	long timeout = 10; /* seconds */
	long busy_timeout = 10; /* milliseconds */
	int load = 0, fd;
	hrtime_t start;

	/*
	 * If inside a container, set the timeout to zero (LP: #1760173),
	 * however, this can be over-ridden by ZFS_MODULE_TIMEOUT just
	 * in case the user explicitly wants to set the timeout for some
	 * reason just for backward compatibilty
	 */
	if (access("/run/systemd/container", R_OK) == 0)
		timeout = 0;

	/* Optionally request module loading */
	if (!libzfs_module_loaded(module)) {
		load_str = getenv("ZFS_MODULE_LOADING");
		if (load_str) {
			if (!strncasecmp(load_str, "YES", strlen("YES")) ||
			    !strncasecmp(load_str, "ON", strlen("ON")))
				load = 1;
			else
				load = 0;
		}

		if (load) {
			if (libzfs_run_process("/sbin/modprobe", argv, 0))
				return (ENOEXEC);
		}

		if (!libzfs_module_loaded(module))
			return (ENXIO);
	}

	/*
	 * Device creation by udev is asynchronous and waiting may be
	 * required.  Busy wait for 10ms and then fall back to polling every
	 * 10ms for the allowed timeout (default 10s, max 10m).  This is
	 * done to optimize for the common case where the device is
	 * immediately available and to avoid penalizing the possible
	 * case where udev is slow or unable to create the device.
	 */
	timeout_str = getenv("ZFS_MODULE_TIMEOUT");
	if (timeout_str) {
		timeout = strtol(timeout_str, NULL, 0);
		timeout = MAX(MIN(timeout, (10 * 60)), 0); /* 0 <= N <= 600 */
	}

	start = gethrtime();
	do {
		fd = open(ZFS_DEV, O_RDWR | O_CLOEXEC);
		if (fd >= 0) {
			(void) close(fd);
			return (0);
		} else if (errno != ENOENT) {
			return (errno);
		} else if (NSEC2MSEC(gethrtime() - start) < busy_timeout) {
			sched_yield();
		} else {
			usleep(10 * MILLISEC);
		}
	} while (NSEC2MSEC(gethrtime() - start) < (timeout * MILLISEC));

	return (ENOENT);
}

int
libzfs_load_module(void)
{
	return (libzfs_load_module_impl(ZFS_DRIVER));
}

int
find_shares_object(differ_info_t *di)
{
	char fullpath[MAXPATHLEN];
	struct stat64 sb = { 0 };

	(void) strlcpy(fullpath, di->dsmnt, MAXPATHLEN);
	(void) strlcat(fullpath, ZDIFF_SHARESDIR, MAXPATHLEN);

	if (stat64(fullpath, &sb) != 0) {
		(void) snprintf(di->errbuf, sizeof (di->errbuf),
		    dgettext(TEXT_DOMAIN, "Cannot stat %s"), fullpath);
		return (zfs_error(di->zhp->zfs_hdl, EZFS_DIFF, di->errbuf));
	}

	di->shares = (uint64_t)sb.st_ino;
	return (0);
}

/*
 * Fill given version buffer with zfs kernel version read from ZFS_SYSFS_DIR
 * Returns 0 on success, and -1 on error (with errno set)
 */
int
zfs_version_kernel(char *version, int len)
{
	int _errno;
	int fd;
	int rlen;

	if ((fd = open(ZFS_SYSFS_DIR "/version", O_RDONLY | O_CLOEXEC)) == -1)
		return (-1);

	if ((rlen = read(fd, version, len)) == -1) {
		version[0] = '\0';
		_errno = errno;
		(void) close(fd);
		errno = _errno;
		return (-1);
	}

	version[rlen-1] = '\0';  /* discard '\n' */

	if (close(fd) == -1)
		return (-1);

	return (0);
}
