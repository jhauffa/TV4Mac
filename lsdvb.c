/*
 * lsdvb - list DVB devices
 *
 * Copyright (C) 2010 Manu Abraham <abraham.manu@gmail.com>
 * modified in 2014 by Jan Hauffa
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* gcc -I webcamd-3.13.0.5/api -L webcamd-3.13.0.5 -lwebcamd lsdvb.c -o lsdvb */

#include <stdio.h>
#include <sys/fcntl.h>
#include <linux/dvb/frontend.h>
#include <webcamd.h>

int main(void)
{
	int entry, i, ret;
	struct cdev_handle *fd;
	struct dvb_frontend_info info;

	static char *fe_type[] = {
		[0] = "FE_QPSK",
		[1] = "FE_QAM",
		[2] = "FE_OFDM",
		[3] = "FE_ATSC",
	};

	fprintf(stderr, "\n\t\tlsdvb: Simple utility to list DVB devices\n");
	fprintf(stderr, "\t\tVersion: 0.0.4x\n");
	fprintf(stderr, "\t\tCopyright (C) Manu Abraham\n");

	linux_parm();
	linux_init();

	for (i = 0; i < F_V4B_SUBDEV_MAX; i++) {
		entry = F_V4B_MKDEV(F_V4B_DVB_FRONTEND, i, 0);
		fd = linux_open(entry, O_RDWR | O_NONBLOCK);
		if (fd == NULL) {
			fprintf(stderr, "ERROR: Open %d failed\n", entry);
			continue;
		}

		ret = linux_ioctl(fd, O_RDONLY, FE_GET_INFO, &info);
		if (ret < 0) {
			fprintf(stderr, "ERROR: IOCTL failed\n");
		} else {
			fprintf(stderr, "FRONTEND:%d (%s) \n\t\t %s Fmin=%dMHz Fmax=%dMHz\n",
					entry,
					info.name,
					fe_type[info.type],
					info.type == 0 ? info.frequency_min / 1000: info.frequency_min / 1000000,
					info.type == 0 ? info.frequency_max / 1000: info.frequency_max / 1000000);
		}

		linux_close(fd);
	}

	linux_exit();
	return 0;
}
