/******************************************************
 *     Video Player für Punk in Wien Ausstellung      *
 *     (C) 2010 Josef Gosch <meibier@gmail.com>       *
 * licensed under the terms of the GNU PUBLIC LICENSE *
 *****************************************************/

#include "player.h"
#include "playlist.h"
#include "callbacks.h"

G_MODULE_EXPORT void cb_hide_playlist(GtkButton *button, gpointer userdata) {
	panda_player *self = (panda_player *)userdata;
	if (self->pl_visible)
		hide_playlist(self);
	else
		show_playlist(self);
}

G_MODULE_EXPORT void cb_player_play_pause(GtkButton *button, gpointer userdata) {
	panda_player *self = (panda_player *)userdata;

	if (player_is_playing(self))
		player_pause(self);
	else
		player_play(self);
	
}

G_MODULE_EXPORT void cb_player_stop(GtkButton *button, gpointer userdata) {
	panda_player *self = (panda_player *)userdata;
	player_stop(self);
}

G_MODULE_EXPORT void cb_player_next(GtkButton *button, gpointer userdata) {
	
}

G_MODULE_EXPORT void cb_player_previous(GtkButton *button, gpointer userdata) {
	
}

G_MODULE_EXPORT void on_main_window_destroy (GtkObject *object, gpointer user_data) {
	panda_player *self = (panda_player *) user_data;
	g_debug("quitting.\n");
	player_stop(self);
	player_destroy(self);
	gtk_main_quit();
	
}

G_MODULE_EXPORT void cb_mouse_move (GtkWidget *object, GdkEventMotion *event, gpointer user_data) {
	g_print("mouse moved\n");
}

G_MODULE_EXPORT void cb_player_seek (GtkRange *range, GtkScrollType *scroll, gdouble value, gpointer user_data) {
	panda_player *self = (panda_player *)user_data;
	g_debug("seek request: second %f", value);
	player_seek(self, (gint64)value*GST_SECOND);
}

void cb_movie_select(GtkTreeView *view, GtkTreePath *path,
                        GtkTreeViewColumn *col, gpointer userdata) {
	GtkTreeIter   iter;
	GtkTreeModel *model;
	GtkTreeSelection *selection;
	panda_player *self = (panda_player *)userdata;
	

	model = gtk_tree_view_get_model(view);
	selection = gtk_tree_view_get_selection(view); 
	if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
		GtkTreePath *path;
		gint *indexes;

		path = gtk_tree_model_get_path(model, &iter);
		indexes = gtk_tree_path_get_indices(path);
		playlist_play_entry(self, indexes[0]);
				
	} 
}

