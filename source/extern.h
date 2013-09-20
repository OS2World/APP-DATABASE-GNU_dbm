/* extern.h - The collection of external definitions needed. */

/*  This file is part of GDBM, the GNU data base manager, by Philip A. Nelson.
    Copyright (C) 1990, 1991  Free Software Foundation, Inc.

    GDBM is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 1, or (at your option)
    any later version.

    GDBM is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GDBM; see the file COPYING.  If not, write to
    the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

    You may contact the author by:
       e-mail:  phil@cs.wwu.edu
      us-mail:  Philip A. Nelson
                Computer Science Department
                Western Washington University
                Bellingham, WA 98226
        phone:  (206) 676-3035

*************************************************************************/


/* The global variables used for the "original" interface. */
extern gdbm_file_info  *_gdbm_file;

/* Memory for return data for the "original" interface. */
extern datum _gdbm_memory;
extern char *_gdbm_fetch_val;


/* External routines used. */
#ifdef __STDC__
extern gdbm_file_info *gdbm_open (char *file, int block_size, int read_write, int mode, void (*fatal_func)());
extern datum gdbm_fetch (gdbm_file_info *dbf, datum key);
extern datum gdbm_firstkey (gdbm_file_info *dbf);
extern datum gdbm_nextkey (gdbm_file_info *dbf, datum key);
extern int gdbm_delete (gdbm_file_info *dbf, datum key);
extern int gdbm_reorganize (gdbm_file_info *dbf);
#else /* not __STDC__ */
extern gdbm_file_info *gdbm_open ();
extern datum gdbm_fetch ();
extern datum gdbm_firstkey ();
extern datum gdbm_nextkey ();
extern int gdbm_delete ();
extern int gdbm_store ();
#endif /* not __STDC__ */

extern void gdbm_close ();
