/******************************************************
 *     Video Player für Punk in Wien Ausstellung      *
 *     (C) 2010 Josef Gosch <meibier@gmail.com>       *
 * licensed under the terms of the GNU PUBLIC LICENSE *
 *****************************************************/

#include <sqlite.h>

void playlist_sql_search(panda_player *self, int (*callback)(void*,int,char**,char**), char *expression);
void playlist_sql_fill_table(panda_player *self, int (*callback)(void*,int,char**,char**));
void playlist_sql_remove(panda_player *self, int id);
void playlist_sql_insert(panda_player *self, char *table, char *columns, char *values);
void playlist_sql_reset(panda_player *self);
void playlist_sql_close(panda_player *self);
void playlist_sql_init(panda_player *self, char *dbname);
