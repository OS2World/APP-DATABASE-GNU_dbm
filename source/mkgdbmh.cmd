@echo off
cp gdbm.proto gdbm.h
grep _ gdbmerrno.h >> gdbm.h
