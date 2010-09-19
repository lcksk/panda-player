/******************************************************
 *     Video Player für Punk in Wien Ausstellung      *
 *     (C) 2010 Josef Gosch <meibier@gmail.com>       *
 * licensed under the terms of the GNU PUBLIC LICENSE *
 *****************************************************/
#include "player.h"
#include "playlist_sqlite.h"


void playlist_sql_init(panda_player *self, char *dbname) {
	g_debug("initializing database: %s", dbname);
	char **errormsg = NULL;
	self->playlist_db = sqlite_open(dbname, 0, errormsg);
	

}

void playlist_sql_close(panda_player *self) {
	sqlite_close(self->playlist_db);
}

void playlist_sql_reset(panda_player *self) {
	char **errors = (char **)malloc(1024*sizeof(char));
	sqlite_exec(self->playlist_db, "DROP TABLE entries", NULL, NULL, errors);
	if (errors[0]) g_debug("sqlite: %s", errors[0]);
	sqlite_exec(self->playlist_db, "DROP TABLE categories", NULL, NULL, errors);
	if (errors[0]) g_debug("sqlite: %s", errors[0]);
	sqlite_exec(self->playlist_db, "DROP TABLE users", NULL, NULL, errors);
	if (errors[0]) g_debug("sqlite: %s", errors[0]);
	
	sqlite_exec(self->playlist_db, 
				"CREATE TABLE entries ( id INT AUTOINCREMENT, title VARCHAR(256), path VARCHAR(1024), category int )",
				NULL, NULL, errors);
	if (errors[0]) g_debug("sqlite: %s", errors[0]);
	sqlite_exec(self->playlist_db, 
				"CREATE TABLE categories ( id INT AUTOINCREMENT, title VARCHAR(256) )",
				NULL, NULL, errors);
	if (errors[0]) g_debug("sqlite: %s", errors[0]);
	sqlite_exec(self->playlist_db, 
				"CREATE TABLE users ( id INT AUTOINCREMENT, name VARCHAR(256), password VARCHAR(256) )",
				NULL, NULL, errors);
	if (errors[0]) g_debug("sqlite: %s", errors[0]);
}

void playlist_sql_insert(panda_player *self, char *table, char *columns, char *values) {
	char **errors = (char **)malloc(1024*sizeof(char));
	char *query = (char *)malloc(1024*sizeof(char));
	sprintf(query, "INSERT INTO %s (%s) VALUES (%s)", table, columns, values);
	sqlite_exec(self->playlist_db, query, NULL, NULL, errors);
	if (errors[0]) g_debug("sqlite: %s", errors[0]);
}

void playlist_sql_remove(panda_player *self, int id) {
	char *query = NULL;
	sprintf(query, "DELETE FROM entries WHERE id=%d", id);
	sqlite_exec(self->playlist_db, query, NULL, NULL, NULL);
}

void playlist_sql_fill_table(panda_player *self, int (*callback)(void*,int,char**,char**)) {
	
	sqlite_exec(self->playlist_db, "SELECT * FROM entries", callback, self, NULL);
}

void playlist_sql_search(panda_player *self, int (*callback)(void*,int,char**,char**), char *expression) {
	char *query = NULL;
	sprintf(query, "SELECT * FROM entries WHERE %s", expression);
	sqlite_exec(self->playlist_db, query, callback, self, NULL);
}
