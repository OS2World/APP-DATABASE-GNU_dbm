-LL

(-DSYSV -Za

# dbm files
dbminit.c	{gdbmdefs.h gdbmconst.h systems.h extern.h gdbmerrno.h}
delete.c	{gdbmdefs.h gdbmconst.h systems.h extern.h}
fetch.c		{gdbmdefs.h gdbmconst.h systems.h extern.h}
store.c		{gdbmdefs.h gdbmconst.h systems.h extern.h}
seq.c		{gdbmdefs.h gdbmconst.h systems.h extern.h}

# ndbm files
dbmopen.c	{gdbmdefs.h gdbmconst.h systems.h extern.h gdbmerrno.h}
dbmdelet.c	{gdbmdefs.h gdbmconst.h systems.h extern.h}
dbmfetch.c	{gdbmdefs.h gdbmconst.h systems.h extern.h}
dbmstore.c	{gdbmdefs.h gdbmconst.h systems.h extern.h}
dbmseq.c	{gdbmdefs.h gdbmconst.h systems.h extern.h}
dbmclose.c	{gdbmdefs.h gdbmconst.h systems.h systems.h}
dbmpagfn.c	{gdbmdefs.h gdbmconst.h systems.h extern.h}
dbmdirfn.c	{gdbmdefs.h gdbmconst.h systems.h extern.h}

# gdbm files
gdbmclos.c	{gdbmdefs.h gdbmconst.h systems.h}
gdbmdele.c	{gdbmdefs.h gdbmconst.h systems.h gdbmerrno.h}
gdbmfetc.c	{gdbmdefs.h gdbmconst.h systems.h gdbmerrno.h}
gdbmopen.c	{gdbmdefs.h gdbmconst.h systems.h gdbmerrno.h}
gdbmreor.c	{gdbmdefs.h gdbmconst.h systems.h gdbmerrno.h extern.h}
gdbmseq.c	{gdbmdefs.h gdbmconst.h systems.h}
gdbmstor.c	{gdbmdefs.h gdbmconst.h systems.h gdbmerrno.h}

# gdbm support files
bucket.c	{gdbmdefs.h gdbmconst.h systems.h}
falloc.c	{gdbmdefs.h gdbmconst.h systems.h}
findkey.c	{gdbmdefs.h gdbmconst.h systems.h}
global.c	{gdbmdefs.h gdbmconst.h systems.h gdbmerrno.h}
hash.c		{gdbmdefs.h gdbmconst.h systems.h}
update.c	{gdbmdefs.h gdbmconst.h systems.h}
version.c

)
