/*
 * MS-DOS port (c) 1990 by Thorsten Ohl, td12@@ddagsi3.bitnet
 *
 * To this port, the same copying conditions apply as to the
 * original release.
 *
 * IMPORTANT:
 * These files are not identical to the original GNU release!
 * You should have received this code as patch to the official
 * GNU release.
 *
 * MORE IMPORTANT:
 * This port comes with ABSOLUTELY NO WARRANTY.
 */

Note: This port was done for 1.4.
I have applied a part of it to 1.5 and made some minor further changes

Kai Uwe Rommel
rommel@lan.informatik.tu-muenchen.dbp.de
-----------


This is release 1.5 of GNU dbm.  Better documentation will be written
soon.  For now, this file briefly describes the contents of this
release and how to use it.

The files are:

COPYING		- Copying information.
README		- This file.

bucket.c, extern.h, falloc.c, findkey.c, gdbm.proto, gdbmclose.c,
gdbmconst.h gdbmdefs.h, gdbmdelete.c, gdbmerrno.h, gdbmfetch.c,
gdbmopen.c, gdbmreorg.c, gdbmseq.c, gdbmstore.c, global.c, gndbm.h,
hash.c systems.h, update.c, version.c - Source for GNU dbm library.

dbm.h dbminit.c, delete.c, fetch.c, seq.c, store.c - Source for the
DBM interface.

dbmclose.c, dbmdelete.c, dbmdirfno.c, dbmfetch.c, dbmopen.c, dbmpagfno.c
dbmseq.c, dbmstore.c, ndbm.h - Source for the NDBM interface.

Makefile 	- Makefile, will make gdbm.a  (BSD and SYSV)
testgdbm.c	- A simple test program.
testdbm.c	- A simple test program.
testndbm.c	- A simple test program.
conv2gdbm.c	- A dbm database conversion program.

CHANGES from 1.4 to 1.5:
  1.  Minor bug fixes.  See the ChangeLog.
  2.  Added gdbmconst.h to allow users to change the size of the
      bucket cache in the systems.h file.

CHANGES from 1.0 to 1.4:
  1.  Mainly bug fixes.
  2.  A define for "dbmclose()" was added to dbm.h for those few
      implementaions that need that call.
  3.  For details, see the ChangeLog.

CHANGES from 0.9 to 1.0:
  1.  Makefiles were combined into one and a few new things added to it.
  2.  Several minor bugs were fixed including a cache bug.
  3.  Two new calls (dbm_pagfno, dbm_dirfno) were added to the NDBM interface.
  3.  A conversion program from dbm files to gdbm files was added.
  4.  Reorganize was changed to allow complex file names. (dir/file form)
  5.  testgdbm, testndbm, and testdbm were modified to return key and data
      pairs where needed and to take an optional file name as an argument.
      testgdbm had some command characters changed.
  6.  The DBM and NDBM interfaces were separated.
  7.  An include file for dbm users was added. (dbm.h)
  8.  The include file for ndbm users was renamed ndbm.h. (It was gndbm.h.)

CHANGES from 0.8 to 0.9:
  1.  The hash function changed.
  2.  The file format changed.
  3.  There was a complete rewrite of falloc.c.
  4.  There were added compatiblity routines for ndbm.
  5.  The file names for dbm compatibility routines were made to
      look like dbm.
  6.  Test programs changed.
  7.  Support for System V.
  8.  Various other small changes.
  9.  The need for recovery and associated code was removed.


GNU dbm is a set of database routines that use extendible hashing and
works similar to the standard UNIX dbm routines.  The basic unit of
data is the structure

  typedef struct {
             char *dptr;
             int   dsize;
          } datum;

The following is a quick list of the routines.  After this list, a
longer description of each routine will be given.  An include file
will be produced that can be included by the user. The file is
"gdbm.h".  The following routines are defined in terms of gdbm.h:

  GDBM_FILE gdbm_open ( name, block_size, read_write, mode, fatal_func )

  void gdbm_close ( dbf )

  int gdbm_store ( dbf, key, content, flags )

  datum gdbm_fetch ( dbf, key )

  int gdbm_delete ( dbf, key )

  datum gdbm_firstkey ( dbf )

  datum gdbm_nextkey ( dbf, key )

  int gdbm_reorganize ( dbf )


For compatibility with the standard dbm, the following routines are
defined.  There is an include file for dbm users called "dbm.h".

  int dbminit ( name )

  int store ( key, content )

  datum fetch ( key )

  int delete ( key )

  datum firstkey ()

  datum nextkey ( key )


There are also compatibility routines for ndbm.  For ndbm compatiblity
routines, you need the include file "ndbm.h".  The routines are:

DBM *dbm_open (name, flags, mode)

void dbm_close (file)

datum dbm_fetch (file, key)

int dbm_store (file, key, content, flags)

int dbm_delete (file, key)

datum dbm_firstkey (file)

datum dbm_nextkey (file)

int dbm_error (file)

int dbm_clearerr (file)

int dbm_pagfno (file)

int dbm_dirfno (file)


Description of GNU dbm routines.
--------------------------------

GNU dbm allows multiple data files.  A routine that opens a gdbm file
is designated as a "reader" or a "writer".  Only one writer may open a
gdbm file and many readers may open the file.  Readers and writers can
not open the gdbm file at the same time. The procedure for opening a
gdbm file is:

GDBM_FILE dbf;

dbf = gdbm_open ( name, block_size, read_write, mode, fatal_func )

The parameters are:
  char *name - the name of the file (the complete name, gdbm does
	  not append any characters to this name)
  int block_size - the size of a single transfer from disk to memory.
	  This parameter is ignored unless the file is a new file.
	  The minimum size is 512.  If it is less than 512, dbm will
	  use the stat block size for the file system.
  int read_write - 0 => reader, 1 => writer, 2 => writer (if the database
	  does not exist, create a new one, 3 => writer and create a new
	  database regardless if one exists.  (Defined in gdbm.h as
	  GDBM_READER, GDBM_WRITER, GDBM_WRCREAT, GDBM_NEWDB.)
  int mode - file mode (see chmod(2) and open(2)) if the file is created.
  void (*fatal_func) () - a function for dbm to call if it detects a
	  fatal error. The only parameter of this function is a string.
	  If the value of 0 is provided, gdbm will use a default function.

The return value, dbf, is the pointer needed by all other routines to
access that gdbm file.  If the return is the NULL pointer, gdbm_open
was not successful.  The errors can be found in "gdbm_errno" for gdbm
errors and in "errno" for file system errors.  (For error codes, see
gdbmerrno.h.)

In all of the following calls, the parameter "dbf" refers to the pointer
returned from gdbm_open.

It is important that every file opened is also closed.  This is needed to
update the reader/writer count on the file.  This is done by:

  gdbm_close(dbf);


The database is used by 3 primary routines.  The first stores data in the
database.

  ret = gdbm_store ( dbf, key, content, flag )

  The parameters are:
     char *dbf - the pointer returned by gdbm_open
     datum key - the key data
     datum content - the data to be associated with the key
     int   flag - 0 => insert only, generate an error if key exists.
	          1 => replace contents if key exists.  (Defined in
	          gdbm.h as GDBM_INSERT and GDBM_REPLACE.)

  If a reader calls store, ret gets -1.  If called with GDBM_INSERT and
  key is in the database, ret gets 1.  Otherwise, ret is 0.
  NOTICE: If you store data for a key that is already in the data base,
  gdbm replaces the old data with the new data if called with GDBM_REPLACE.
  You do not get two data items for the same key and you do not get an
  error from gdbm_store.
  NOTICE: The size in gdbm is not restricted like dbm or ndbm.  Your data
  can be as large as your "want".
  NOTICE: Both key and content must have the dptr field be a non-NULL
  value.  Since a NULL dptr field is used by other routines to indicate an
  error, a NULL field cannot be valid data.  If either key or content have
  a null dptr field, gdbm_open will return an error.

To search for some data:

  content = gdbm_fetch ( dbf, key )

  The parameters are:
     char *dbf - the pointer returned by gdbm_open
     datum key - the key data

  The "datum" returned in content is a pointer to the data found.  If the
  dptr is NULL, no data was found.  If dptr is not NULL, then it points
  to data allocated by malloc.  gdbm does not automatically free this data.
  The user must free this storage when done using it.  This eliminates the
  need to copy the result to save it for later use. (You just save the
  pointer.)

To remove some data from the database:

  ret = gdbm_delete ( dbf, key )

  The parameters are:
     char *dbf - the pointer returned by gdbm_open
     datum key - the key data

  The ret value is -1 if the item is not present or the requester is a reader.
  The ret value is 0 if there was a successful delete.


The next two routines allow for accessing all items in the database.  This
access is not key sequential, but it is guaranteed to visit every key in
the database once.  (The order has to do with the hash values.)

  key = gdbm_firstkey ( dbf )

  nextkey = gdbm_nextkey ( dbf, key )

  The parameters are:
     char *dbf - the pointer returned by gdbm_open
     datum key - the key data

  The return values are both datum.  If key.dptr or nextkey.dptr is NULL,
  there is no first key or next key.  Again notice that dptr points to
  data allocated by malloc and gdbm will not free it for you.

The following routine should be used very seldom.

  ret = gdbm_reorganize ( dbf )

  If you have had a lot of deletions and would like to shrink the space
  used by the gdbm file, the this routine will reorganize the database.
  gdbm will not shorten the length of a gdbm file except by using this
  reorganization.  (Deleted file space will be reused.)


The following two are variables that may need to be used:

  gdbm_error gdbm_errno   -   the variable that contains more information
                              about gdbm errors.  ( gdbm.h has the
			      definitions of the error values. )

  char * gdbm_version     -   the string containing the version information.


There are a few more things of interest.  First, gdbm files are not
"sparse".  You can copy them with the UNIX cp command and they will
not expand in the copying process.  Also, there is a compatibility
mode for use with programs that already use UNIX dbm.  In this
compatibility mode, no gdbm file pointer is required by the user.
Only one file may be opened at a time.  All users in compatibility
mode are assumed to be writers.  If the gdbm file is a read only, it
will fail as a writer, but will also try to open it as a reader.  All
returned pointers in datum structures point to data that gdbm WILL
free.  They should be treated as static pointers (as standard UNIX dbm
does).  The compatibility routine names are the same as the UNIX dbm
routine names.  Their definitions follow:

  int dbminit ( name )

  int store ( key, content )

  datum fetch ( key )

  int delete ( key )

  datum firstkey ()

  datum nextkey ( key )

NOTE: Some implementations have an include file "dbm.h".  That file is just
a file that defines datum and the above routines.  Many original dbm
sites do not have a "dbm.h" file.  One is included here for those who
want it.

WARNING: standard UNIX dbm and GNU dbm do not have the same data format in the
file.  You cannot access a standard UNIX dbm file with GNU dbm!  If you want
to use an old database with GNU dbm, you must use the convert program.

Also, GNU dbm has compatibility routines for ndbm.  For ndbm compatiblity
routines, you need the include file "ndbm.h".

WARNING: If you have ndbm and gdbm, there is a conflict in the names of the
include file for the ndbm interface and the original ndbm package.  Do not
blindly copy "ndbm.h" to your include directory.

The routines are:

    DBM *dbm_open (name, flags, mode)

    void dbm_close (file)

    datum dbm_fetch (file, key)

    int dbm_store (file, key, content, flags)

    int dbm_delete (file, key)

    datum dbm_firstkey (file)

    datum dbm_nextkey (file)

    int dbm_error (file)

    int dbm_clearerr (file)

    int dbm_dirfno (file)

    int dbm_pagfno (file)

Again, just like ndbm, any returned datum can be assumed to be static
storage.  You do not have to free that memory, the ndbm compatibility
routines will do it for you.


Notes on making GNU dbm.
------------------------

The "Makefile" will make both "gdbm.a", the collection of gdbm routines and
three simple test programs that uses the gdbm routines.  Two test programs
test the dbm and the ndbm interface routines.  The third was use to help
debug gdbm and contains "inside knowledge."   The first two can be linked
using the original dbm and ndbm routines.  The makefile also makes the
conversion program, conv2gdbm.   The make commands are:

   make gdbm.a		makes the gdbm.a archive.
   make testgdbm	makes both gdbm.a and the gdbm test program.
   make testdbm 	makes both gdbm.a and the dbm test program with gdbm.
   make testndbm	makes both gdbm.a and the ndbm test program with gdbm.
   make allgdbm		makes all of the above.
   make tdbm		makes the dbm test program linked with dbm, not gdbm.
   make tndbm		makes the ndbm test program linked with ndbm, not gdbm.
   make alldbm		makes the two previous programs.
   make conv2gdbm	makes the conversion program.
   make all		makes all the above.
   make install		installs gdbm.a as /usr/lib/libgdbm.a and gdbm.h.


CONV2GDBM
---------

The program conv2gdbm has been provided to help you convert from dbm
databases to gdbm.  The usage is:

   conv2gdbm [-q] [-b block_size] dbm_file [gdbm_file]

The optional "block_size" is the same as in gdbm_open.  The dbm_file is
the name of the dbm file without the .pag or .dir extensions.  The
optional gdbm_file is the complete file name.  If not included, the
gdbm file name is the same as the dbm file name without any extensions.
That is "conv2gdbm dbmfile" converts the files "dbmfile.pag" and
"dbmfile.dir" into a gdbm file called "dbmfile".  The -q option causes
conv2gdbm to work quietly.


System V support  (And other systems.)
--------------------------------------

There is now support for System V.  This done via the systems.h file.
This is the place where all system dependencies should go.  The
makefile should make gdbm with a few changes.  The makefile describes
the changes necessary for use with System V.  Also, read the makefile
and edit it if you use gcc.  There are several places where changes
are needed to use gcc.
  1)  uncomment: #CC=gcc
  2)  change the lines to make the test programs. (See the comments.)

If you port gdbm to another system, try to follow the change style used for
System V changes.  Please send your changes to phil@cs.wwu.edu if you would
like your changes included in a future release of gdbm.

Please send bug reports to bug-gnu-utils@prep.ai.mit.edu.

Thank you.
