/*
 * AppArmor security module
 *
 * This file contains AppArmor file mediation function definitions.
 *
 * Copyright 2014 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2 of the
 * License.
 *
 *
 * This is a file of helper fns backported from newer kernels to support
 * backporting of apparmor to older kernels. Fns prefixed with code they
 * are copied of modified from
 */

#include "include/backport.h"

/* copied from commit c3c073f808b22dfae15ef8412b6f7b998644139a */
int iterate_fd(struct files_struct *files, unsigned n,
               int (*f)(const void *, struct file *, unsigned),
               const void *p)
{
	struct fdtable *fdt;
	struct file *file;
	int res = 0;
	if (!files)
		return 0;
	spin_lock(&files->file_lock);
	fdt = files_fdtable(files);
	while (!res && n < fdt->max_fds) {
		file = rcu_dereference_check_fdtable(files, fdt->fd[n++]);
		if (file)
			res = f(p, file, n);
	}
	spin_unlock(&files->file_lock);
	return res;
}
