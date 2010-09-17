/******************************************************
 *     Video Player für Punk in Wien Ausstellung      *
 *     (C) 2010 Josef Gosch <meibier@gmail.com>       *
 * licensed under the terms of the GNU PUBLIC LICENSE *
 *****************************************************/

void cb_movie_select(GtkTreeView *view, GtkTreePath *path, GtkTreeViewColumn *col, gpointer userdata);
G_MODULE_EXPORT void cb_player_seek (GtkRange *range, GtkScrollType *scroll, gdouble value, gpointer user_data);
G_MODULE_EXPORT void cb_mouse_move (GtkWidget *object, GdkEventMotion *event, gpointer user_data);
G_MODULE_EXPORT void on_main_window_destroy (GtkObject *object, gpointer user_data);
G_MODULE_EXPORT void cb_player_previous(GtkButton *button, gpointer userdata);
G_MODULE_EXPORT void cb_player_next(GtkButton *button, gpointer userdata);
G_MODULE_EXPORT void cb_player_stop(GtkButton *button, gpointer userdata);
G_MODULE_EXPORT void cb_player_play_pause(GtkButton *button, gpointer userdata);
G_MODULE_EXPORT void cb_hide_playlist(GtkButton *button, gpointer userdata);
                    
