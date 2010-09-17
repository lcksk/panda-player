/******************************************************
 *     Video Player für Punk in Wien Ausstellung      *
 *     (C) 2010 Josef Gosch <meibier@gmail.com>       *
 * licensed under the terms of the GNU PUBLIC LICENSE *
 *****************************************************/

void hide_playlist(panda_player *self);
void show_playlist(panda_player *self);
void load_playlist_from_file(panda_player *self, gchar *filename);
void playlist_play_entry(panda_player *self, int line);
void playlist_add_entry(panda_player *self, gchar *title, gchar *file);
void playlist_init(panda_player *self);
