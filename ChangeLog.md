# Changelog
All notable changes to this project will be documented in this file.

## 2020-02-22

### Added
- ChangeLog.md: new file
- src/include/sudba.h: macros DB_NAME, DB_WD, DB_PIDFILE, DB_SCHEMA_EXT, DB_DATA_EXT, DB_INDEX_EXT
- src/include/sudba.h/makeString(): new function prototype
- src/makefile: support for sudba-utils.c and some missing dependencies
- src/sudba-main.c: sys/types.h, glob.h, string.h, sys/stat.h, fcntl.h
- src/sudba-main.c: global variables sudba_tables, sudba_tab_count
- src/sudba-main.c:/sudba_read_tables(): new function
- src/sudba-main.c/sudba_store_table_name(): new function
- src/sudba-main.c/main(): call to sudba_read_tables()
- src/sudba-utils.c: new file
- src/sudba-utils.c/makeString(): new function
- src/susql.l: int, float, id, string token values
- src/susql.y: %union for token values
- src/susql.y: %token types
