/* testgdbm.c - Driver program to test the database routines and to
   help debug gdbm.  Uses inside information to show "system" information */

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


#include "gdbmdefs.h"
#include "gdbmerrno.h"
#include "extern.h"

extern gdbm_error gdbm_errno;

extern char * gdbm_version;

/* For getopt. */
extern char *optarg;
extern int optind, opterr;

gdbm_file_info *gdbm_file;

#ifdef __STDC__
void _gdbm_print_bucket_cache (gdbm_file_info *dbf);
static void print_bucket (hash_bucket *bucket, char *mesg);
static void _gdbm_print_avail_list (gdbm_file_info *dbf);
void main (int argc, char **argv);
void usage (char *s);
void gdbm_perror (char *msg);
extern int getopt (int argc, char **argv, const char *shortopts);
#endif /* __STDC__ */


/* access GDBM_ERRNO.  [tho]  */

void
gdbm_perror (char *msg)
{
  char *err_msg;

  switch (gdbm_errno)
    {
    case GDBM_NO_ERROR:
      err_msg = "no error";
      break;
    case GDBM_MALLOC_ERROR:
      err_msg = "can't malloc";
      break;
    case GDBM_BLOCK_SIZE_ERROR:
      err_msg = "bad block size";
      break;
    case GDBM_FILE_OPEN_ERROR:
      err_msg = "can't open file";
      break;
    case GDBM_FILE_WRITE_ERROR:
      err_msg = "can't write file";
      break;
    case GDBM_FILE_SEEK_ERROR:
      err_msg = "can't seek file";
      break;
    case GDBM_FILE_READ_ERROR:
      err_msg = "can't read file";
      break;
    case GDBM_BAD_MAGIC_NUMBER:
      err_msg = "bad magic number";
      break;
    case GDBM_EMPTY_DATABASE:
      err_msg = "empty database";
      break;
    case GDBM_CANT_BE_READER:
      err_msg = "can't be reader";
      break;
    case GDBM_CANT_BE_WRITER:
      err_msg = "can't be writer";
      break;
    case GDBM_READER_CANT_DELETE:
      err_msg = "can't delete";
      break;
    case GDBM_READER_CANT_STORE:
      err_msg = "can't store";
      break;
    case GDBM_READER_CANT_REORGANIZE:
      err_msg = "can't reorganize";
      break;
    case GDBM_UNKNOWN_UPDATE:
      err_msg = "unkown update";
      break;
    case GDBM_ITEM_NOT_FOUND:
      err_msg = "item not found";
      break;
    case GDBM_REORGANIZE_FAILED:
      err_msg = "reorganization failed";
      break;
    case GDBM_CANNOT_REPLACE:
      err_msg = "can't replace";
      break;
    default:
      err_msg = "unknown error";
    }

  fprintf (stderr, "%s (GDBM error: %s)\n", msg, err_msg);
}


/* Debug procedure to print the contents of the current hash bucket. */
void
print_bucket (bucket, mesg)
     hash_bucket *bucket;
     char *mesg;
{
  int  index;

  printf ("******* %s **********\n\nbits = %d\ncount= %d\nHash Table:\n",
	 mesg, bucket->bucket_bits, bucket->count);
  printf ("     #    hash value     key size    data size     data adr  home\n");
  for (index = 0; index < gdbm_file->header->bucket_elems; index++)
#ifdef MSDOS
    printf ("  %4d  %12lx  %11d  %11d  %11ld %5d\n", index,
	   bucket->h_table[index].hash_value,
	   bucket->h_table[index].key_size,
	   bucket->h_table[index].data_size,
	   bucket->h_table[index].data_pointer,
	   (int) (bucket->h_table[index].hash_value % gdbm_file->header->bucket_elems));
#else /* not MSDOS */
    printf ("  %4d  %12x  %11d  %11d  %11d %5d\n", index,
	   bucket->h_table[index].hash_value,
	   bucket->h_table[index].key_size,
	   bucket->h_table[index].data_size,
	   bucket->h_table[index].data_pointer,
	   bucket->h_table[index].hash_value % gdbm_file->header->bucket_elems);
#endif /* not MSDOS */

  printf ("\nAvail count = %1d\n", bucket->av_count);
  printf ("Avail  adr     size\n");
  for (index = 0; index < bucket->av_count; index++)
#ifdef MSDOS
    printf ("%9ld%9d\n", bucket->bucket_avail[index].av_adr,
#else /* not MSDOS */
    printf ("%9d%9d\n", bucket->bucket_avail[index].av_adr,
#endif /* not MSDOS */
	                bucket->bucket_avail[index].av_size);
}


void
_gdbm_print_avail_list (dbf)
     gdbm_file_info *dbf;
{
  long temp;
  int size;
  avail_block *av_stk;

  /* Print the the header avail block.  */
  printf ("\nheader block\nsize  = %d\ncount = %d\n",
	  dbf->header->avail.size, dbf->header->avail.count);
  for (temp = 0; temp < dbf->header->avail.count; temp++)
    {
#ifdef MSDOS
      printf ("  %15d   %10ld \n", dbf->header->avail.av_table[temp].av_size,
#else /* not MSDOS */
      printf ("  %15d   %10d \n", dbf->header->avail.av_table[temp].av_size,
#endif /* not MSDOS */
	      dbf->header->avail.av_table[temp].av_adr);
    }

  /* Initialize the variables for a pass throught the avail stack. */
  temp = dbf->header->avail.next_block;
  size = ( ( (dbf->header->avail.size * sizeof (avail_elem)) >> 1)
	  + sizeof (avail_block));
  av_stk = (avail_block *) alloca (size);
#ifdef MSDOS
  if (av_stk == (avail_block *) 0)
    {
      fprintf (stderr, "Error: alloca() failed in gdbm (%s).\n", __FILE__);
      exit (-2);
    }
#endif /* MSDOS */

  /* Print the stack. */
  while (FALSE)
    {
      lseek (dbf->desc, temp, L_SET);
      read  (dbf->desc, av_stk, size);

      /* Print the block! */
      printf ("\nblock = %d\nsize  = %d\ncount = %d\n", temp,
	      av_stk->size, av_stk->count);
      for (temp = 0; temp < av_stk->count; temp++)
	{
#ifdef MSDOS
	  printf ("  %15d   %10ld \n", av_stk->av_table[temp].av_size,
#else /* not MSDOS */
	  printf ("  %15d   %10d \n", av_stk->av_table[temp].av_size,
#endif /* not MSDOS */
	    av_stk->av_table[temp].av_adr);
	}
      temp = av_stk->next_block;
    }
}

void _gdbm_print_bucket_cache (dbf)
     gdbm_file_info *dbf;
{
  int index;
  char changed;

  printf ("Bucket Cache:\n  Index:  Address  Changed  Data_Hash \n");
  for (index=0; index < CACHE_SIZE; index++)
    {
      changed = dbf->bucket_cache[index].ca_changed;
#ifdef MSDOS
      printf ("  %5d:  %7ld  %7s  %lx\n",
#else /* not MSDOS */
      printf ("  %5d:  %7d  %7s  %x\n",
#endif /* not MSDOS */
	      index,
	      dbf->bucket_cache[index].ca_adr,
	      (changed ? "True" : "False"),
	      dbf->bucket_cache[index].ca_data.hash_val);
    }
}

void usage (s)
     char *s;
{
  printf ("Usage: %s [-r or -n] [gdbm-file] \n",s);
  exit (2);
}


/* The test program allows one to call all the routines plus the hash function.
   The commands are single letter commands.  The user is prompted for all other
   information.  See the help command (?) for a list of all commands. */

void
main (argc, argv)
     int argc;
     char *argv[];

{

  int  cmd_ch;
  int  opt;

  datum key_data;
  datum data_data;
  datum return_data;

  char key_line[500];
  char data_line[1000];

  char done = FALSE;
  char reader = FALSE;
  char newdb = FALSE;

  char *file_name;


  /* Argument checking. */
  while ((opt = getopt (argc, argv, "rn")) != -1)
    switch (opt) {
    case 'r':  reader = TRUE;
               if (newdb) usage (argv[0]);
               break;
    case 'n':  newdb = TRUE;
               if (reader) usage (argv[0]);
               break;
    }
  if (argc > optind+1) usage (argv[0]);

  if (optind < argc)
    {
      file_name = argv[optind];
    }
  else
    {
#ifdef MSDOS
      file_name = "junk.gdb";
#else
      file_name = "junk.gdbm";
#endif
    }

  /* Initialize variables. */
  key_data.dptr = NULL;
  data_data.dptr = data_line;

  if (reader)
    gdbm_file = gdbm_open (file_name, 512, GDBM_READER, 00664, NULL);
  else if (newdb)
    gdbm_file = gdbm_open (file_name, 512, GDBM_NEWDB, 00664, NULL);
  else
    gdbm_file = gdbm_open (file_name, 512, GDBM_WRCREAT, 00664, NULL);
  if (gdbm_file == NULL)
    {
      gdbm_perror ("gdbm_open failed");
      exit (2);
    }

  /* Welcome message. */
  printf ("\nWelcome to the gdbm test program.  Type ? for help.\n\n");

  while (!done)
    {
      printf ("com -> ");
#ifdef MSDOS			/* shut up the compiler */
      cmd_ch = (char) getchar ();
#else /* not MSDOS */
      cmd_ch = getchar ();
#endif /* not MSDOS */
      if (cmd_ch != '\n')
	{
	  char temp;
	  do
#ifdef MSDOS			/* shut up the compiler */
	      temp = (char) getchar ();
#else /* not MSDOS */
	      temp = getchar ();
#endif /* not MSDOS */
	  while (temp != '\n' && temp != EOF);
	}
      if (cmd_ch == EOF) cmd_ch = 'q';
      switch (cmd_ch)
	{

	/* Standard cases found in all test{dbm,ndbm,gdbm} programs. */
	case '\n':
	  printf ("\n");
	  break;

	case 'c':
	  {
	    int temp;
	    temp = 0;
	    if (key_data.dptr != NULL) free (key_data.dptr);
	    return_data = gdbm_firstkey (gdbm_file);
	    while (return_data.dptr != NULL)
	      {
		temp++;
		key_data = return_data;
		return_data = gdbm_nextkey (gdbm_file, key_data);
		free (key_data.dptr);
	      }
	    printf ("There are %d items in the database.\n\n", temp);
	    key_data.dptr = NULL;
	  }
	  break;

	case 'd':
	  if (key_data.dptr != NULL) free (key_data.dptr);
	  printf ("key -> ");
	  gets (key_line);
	  key_data.dptr = key_line;
	  key_data.dsize = strlen (key_line)+1;
	  if (gdbm_delete (gdbm_file, key_data) != 0)
	    gdbm_perror ("Item not found or deleted");
	  printf ("\n");
	  key_data.dptr = NULL;
	  break;

	case 'f':
	  if (key_data.dptr != NULL) free (key_data.dptr);
	  printf ("key -> ");
	  gets (key_line);
	  key_data.dptr = key_line;
	  key_data.dsize = strlen (key_line)+1;
	  return_data = gdbm_fetch (gdbm_file, key_data);
	  if (return_data.dptr != NULL)
	    {
	      printf ("data is ->%s\n\n", return_data.dptr);
	      free (return_data.dptr);
	    }
	  else
	    printf ("No such item found.\n\n");
	  key_data.dptr = NULL;
	  break;

	case 'n':
	  if (key_data.dptr != NULL) free (key_data.dptr);
	  printf ("key -> ");
	  gets (key_line);
	  key_data.dptr = key_line;
	  key_data.dsize = strlen (key_line)+1;
	  return_data = gdbm_nextkey (gdbm_file, key_data);
	  if (return_data.dptr != NULL)
	    {
	      key_data = return_data;
	      printf ("key is  ->%s\n", key_data.dptr);
	      return_data = gdbm_fetch (gdbm_file, key_data);
	      printf ("data is ->%s\n\n", return_data.dptr);
	      free (return_data.dptr);
	    }
	  else
	    {
	      gdbm_perror ("No such item found");
	      key_data.dptr = NULL;
	    }
	  break;

	case 'q':
	  done = TRUE;
	  break;

	case 's':
	  if (key_data.dptr != NULL) free (key_data.dptr);
	  printf ("key -> ");
	  gets (key_line);
	  key_data.dptr = key_line;
	  key_data.dsize = strlen (key_line)+1;
	  printf ("data -> ");
	  gets (data_line);
	  data_data.dsize = strlen (data_line)+1;
	  if (gdbm_store (gdbm_file, key_data, data_data, GDBM_REPLACE) != 0)
	    gdbm_perror ("Item not inserted");
	  printf ("\n");
	  key_data.dptr = NULL;
	  break;

	case '1':
	  if (key_data.dptr != NULL) free (key_data.dptr);
	  key_data = gdbm_firstkey (gdbm_file);
	  if (key_data.dptr != NULL)
	    {
	      printf ("key is  ->%s\n", key_data.dptr);
	      return_data = gdbm_fetch (gdbm_file, key_data);
	      printf ("data is ->%s\n\n", return_data.dptr);
	      free (return_data.dptr);
	    }
	  else
	    gdbm_perror ("No such item found");
	  break;

	case '2':
	  return_data = gdbm_nextkey (gdbm_file, key_data);
	  if (return_data.dptr != NULL)
	    {
	      free (key_data.dptr);
	      key_data = return_data;
	      printf ("key is  ->%s\n", key_data.dptr);
	      return_data = gdbm_fetch (gdbm_file, key_data);
	      printf ("data is ->%s\n\n", return_data.dptr);
	      free (return_data.dptr);
	    }
	  else
	    gdbm_perror ("No such item found");
	  break;


	/* Special cases for the testgdbm program. */
	case 'r':
	  {
	    if (gdbm_reorganize (gdbm_file))
	      gdbm_perror ("Reorganization failed");
	    else
	      printf ("Reorganization succeeded. \n\n");
	  }
	  break;

	case 'A':
	  _gdbm_print_avail_list (gdbm_file);
	  printf ("\n");
	  break;

	case 'B':
	  {
	    int temp;
	    char number[80];

	    printf ("bucket? ");
	    gets (number);
	    sscanf (number,"%d",&temp);

	    if (temp >= gdbm_file->header->dir_size /4)
	      {
		gdbm_perror ("Not a bucket");
		break;
	      }
	    _gdbm_get_bucket (gdbm_file, temp);
	  }
	  printf ("Your bucket is now ");

	case 'C':
	  print_bucket (gdbm_file->bucket, "Current bucket");
	  printf ("\n current directory entry = %d.\n", gdbm_file->bucket_dir);
#ifdef MSDOS
	  printf (" current bucket address  = %ld.\n\n",
#else /* not MSDOS */
	  printf (" current bucket address  = %d.\n\n",
#endif /* not MSDOS */
		  gdbm_file->cache_entry->ca_adr);
	  break;

	case 'D':
	  printf ("Hash table directory.\n");
	  printf ("  Size =  %d.  Bits = %d. \n\n",gdbm_file->header->dir_size,
		  gdbm_file->header->dir_bits);
	  {
	    int temp;

	    for (temp = 0; temp < gdbm_file->header->dir_size / 4; temp++)
	      {
#ifdef MSDOS
		printf ("  %10d:  %12ld\n", temp, gdbm_file->dir[temp]);
#else /* not MSDOS */
		printf ("  %10d:  %12d\n", temp, gdbm_file->dir[temp]);
#endif /* not MSDOS */
		if ( (temp+1) % 20 == 0 && isatty (0))
		  {
		    printf ("*** CR to continue: ");
		    while (getchar () != '\n') /* Do nothing. */;
		  }
	      }
	  }
	  printf ("\n");
	  break;

	case 'F':
	  {
	    printf ("\nFile Header: \n\n");
	    printf ("  table        = %ld\n", gdbm_file->header->dir);
	    printf ("  table size   = %d\n", gdbm_file->header->dir_size);
	    printf ("  table bits   = %d\n", gdbm_file->header->dir_bits);
	    printf ("  block size   = %d\n", gdbm_file->header->block_size);
	    printf ("  bucket elems = %d\n", gdbm_file->header->bucket_elems);
	    printf ("  bucket size  = %d\n", gdbm_file->header->bucket_size);
	    printf ("  header magic = %lx\n", gdbm_file->header->header_magic);
	    printf ("  next block   = %d\n", gdbm_file->header->next_block);
	    printf ("  avail size   = %d\n", gdbm_file->header->avail.size);
	    printf ("  avail count  = %d\n", gdbm_file->header->avail.count);
	    printf ("  avail nx blk = %d\n", gdbm_file->header->avail.next_block);
	    printf ("\n");
	  }
	  break;

        case 'H':
	  if (key_data.dptr != NULL) free (key_data.dptr);
	  printf ("key -> ");
	  gets (key_line);
	  key_data.dptr = key_line;
	  key_data.dsize = strlen (key_line)+1;
#ifdef MSDOS
	  printf ("hash value = %lx. \n\n", _gdbm_hash (key_data));
#else /* not MSDOS */
	  printf ("hash value = %x. \n\n", _gdbm_hash (key_data));
#endif /* not MSDOS */
	  key_data.dptr = NULL;
	  break;

	case 'K':
	  _gdbm_print_bucket_cache (gdbm_file);
	  break;

	case 'V':
	  printf ("%s\n\n", gdbm_version);
	  break;

	case '?':
	  printf ("c - count (number of entries)\n");
	  printf ("d - delete\n");
	  printf ("f - fetch\n");
	  printf ("n - nextkey\n");
	  printf ("q - quit\n");
	  printf ("s - store\n");
	  printf ("1 - firstkey\n");
	  printf ("2 - nextkey on last key (from n, 1 or 2)\n\n");

	  printf ("r - reorganize\n");
	  printf ("A - print avail list\n");
	  printf ("B - get and print current bucket n\n");
	  printf ("C - print current bucket\n");
	  printf ("D - print hash directory\n");
	  printf ("F - print file header\n");
	  printf ("H - hash value of key\n");
	  printf ("K - print the bucket cache\n");
	  printf ("V - print version of gdbm\n");
	  break;

	default:
	  printf ("What? \n\n");
	  break;

	}
    }

  /* Quit normally. */
  exit (0);

}
