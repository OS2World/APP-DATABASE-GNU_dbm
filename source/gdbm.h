/* gdbm.h  -  The include file for dbm users.  */

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

/* Parameters to gdbm_open for READERS, WRITERS, and WRITERS who
   can create the database. */
#define  GDBM_READER  0		/* A reader. */
#define  GDBM_WRITER  1		/* A writer. */
#define  GDBM_WRCREAT 2		/* A writer.  Create the db if needed. */
#define  GDBM_NEWDB   3		/* A writer.  Always create a new db. */

/* Parameters to gdbm_store for simple insertion or replacement in the
   case that the key is already in the database. */
#define  GDBM_INSERT  0		/* Never replace old data with new. */
#define  GDBM_REPLACE 1		/* Always replace old data with new. */


/* The data and key structure.  This structure is defined for compatibility. */
typedef struct {
	char *dptr;
	int   dsize;
      } datum;


/* The file information header. This is good enough for most applications. */
typedef struct {int dummy[10];} *GDBM_FILE;


/* These are the routines! */

#ifdef __STDC__
extern GDBM_FILE gdbm_open (char *, int, int, int, void (*) ());
extern void	 gdbm_close (gdbm_file_info *);
extern datum	 gdbm_fetch (gdbm_file_info *, datum);
extern int	 gdbm_store (gdbm_file_info *, datum, datum, int);
extern int	 gdbm_delete (gdbm_file_info *, datum);
extern datum	 gdbm_firstkey (gdbm_file_info *);
extern datum	 gdbm_nextkey (gdbm_file_info *, datum);
extern int	 gdbm_reorganize (gdbm_file_info *);
#else
extern GDBM_FILE gdbm_open ();
extern void	 gdbm_close ();
extern datum	 gdbm_fetch ();
extern int	 gdbm_store ();
extern int	 gdbm_delete ();
extern datum	 gdbm_firstkey ();
extern datum	 gdbm_nextkey ();
extern int	 gdbm_reorganize ();
#endif

/* gdbm sends back the following error codes in the variable gdbm_errno. */
typedef enum {	GDBM_NO_ERROR,
		GDBM_MALLOC_ERROR,
		GDBM_BLOCK_SIZE_ERROR,
		GDBM_FILE_OPEN_ERROR,
		GDBM_FILE_WRITE_ERROR,
		GDBM_FILE_SEEK_ERROR,
		GDBM_FILE_READ_ERROR,
		GDBM_BAD_MAGIC_NUMBER,
		GDBM_EMPTY_DATABASE,
		GDBM_CANT_BE_READER,
	        GDBM_CANT_BE_WRITER,
		GDBM_READER_CANT_DELETE,
		GDBM_READER_CANT_STORE,
		GDBM_READER_CANT_REORGANIZE,
		GDBM_UNKNOWN_UPDATE,
		GDBM_ITEM_NOT_FOUND,
		GDBM_REORGANIZE_FAILED,
		GDBM_CANNOT_REPLACE,
		GDBM_ILLEGAL_DATA}
	gdbm_error;
