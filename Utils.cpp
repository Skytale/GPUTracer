/*
	Copyright 2010 Peter Hofmann

	This file is part of GPUTracer.

	GPUTracer is free software: you can redistribute it and/or modify it
	under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	GPUTracer is distributed in the hope that it will be useful, but
	WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with GPUTracer. If not, see <http://www.gnu.org/licenses/>.
*/


#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include "Utils.hpp"

char *readFile(const char *path)
{
	struct stat statbuf;
	char *databuf = NULL;
	FILE *fp = NULL;

	/* Determine file size. */
	if (stat(path, &statbuf) == -1)
	{
		perror("Could not stat file");
		return NULL;
	}

	/* Allocate memory for the content. */
	databuf = (char *)malloc(statbuf.st_size + 1);
	if (databuf == NULL)
	{
		fprintf(stderr, "Out of memory.\n");
		return NULL;
	}

	fp = fopen(path, "rb");
	if (!fp)
	{
		perror("Could not open file");
		return NULL;
	}

	/* Read 1 element of size "statbuf.st_size". fread() returns the
	 * number of items successfully read. Thus, a return value of "1"
	 * means "success" and anything else is an error. */
	if (fread(databuf, statbuf.st_size, 1, fp) != 1)
	{
		fprintf(stderr, "Unexpected end of file.\n");
		fclose(fp);
		return NULL;
	}

	fclose(fp);

	/* Add NULL terminator. */
	databuf[statbuf.st_size] = 0;

	return databuf;
}
