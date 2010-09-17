/******************************************************
 *     Video Player für Punk in Wien Ausstellung      *
 *     (C) 2010 Josef Gosch <meibier@gmail.com>       *
 * licensed under the terms of the GNU PUBLIC LICENSE *
 *****************************************************/

#include "player.h"
#include "playlist.h"
#include "callbacks.h"

void playlist_init(panda_player *self) {
	GtkCellRenderer *renderer = gtk_cell_renderer_text_new ();
	self->list = gtk_list_store_new(1, G_TYPE_STRING);
	self->pl_entry_count = -1;
	pl_entry = (playlist_entry *)malloc(sizeof(playlist_entry));
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (self->playlist),
                                               -1,      
                                               "Titel",  
                                               renderer,
                                               "text", 0,
												NULL);
	
	gtk_tree_view_set_model(GTK_TREE_VIEW(self->playlist), GTK_TREE_MODEL(self->list));
}


void playlist_add_entry(panda_player *self, gchar *title, gchar *file) {
	GtkTreeIter iter;
	g_debug("adding playlist entry: name(%s) path(%s)", title, file);
	self->pl_entry_count++;
	
	//playlist_entry *pl = (playlist_entry *)malloc(sizeof(playlist_entry));
	//pl = self->pl_entry;
	
	//pl = realloc(pl, sizeof(playlist_entry) * self->pl_entry_count+1);
	pl_entry = (playlist_entry *)realloc(pl_entry, sizeof(playlist_entry)*(self->pl_entry_count+1));
	sprintf(pl_entry[self->pl_entry_count].name, "%s", title);
	sprintf(pl_entry[self->pl_entry_count].path, "%s", file);
	
	
	gtk_list_store_append(self->list, &iter);
	gtk_list_store_set(self->list, &iter, 0, title, -1);
	
}

void hide_playlist(panda_player *self) {
	self->pl_visible = FALSE;
	gtk_widget_hide(self->scroll);
	gtk_button_set_label(GTK_BUTTON(self->hide_playlist_button), "<");
	gtk_widget_hide(self->controls);
}

void show_playlist(panda_player *self) {
	self->pl_visible = TRUE;
	gtk_widget_show(self->scroll);
	gtk_button_set_label(GTK_BUTTON(self->hide_playlist_button), ">");
	gtk_widget_show(self->controls);
}

void playlist_play_entry(panda_player *self, int line) {
	g_debug("playing playlist entry #%d: %s (%s)", line, pl_entry[line].name, pl_entry[line].path);
	load_uri(self, pl_entry[line].path);
	player_play(self);
}

void load_playlist_from_file(panda_player *self, gchar *filename) {
	FILE *f = fopen(filename, "r");
	char *title, *uri;
	title = (char *)malloc(1024*sizeof(char));
	uri = (char *)malloc(1024*sizeof(char));
	if (f == NULL) {
		g_debug("%s: no such file.", filename);
		return;
	}
	gchar *buf = malloc(1024*sizeof(gchar));
	while (fgets(buf, sizeof(playlist_entry), f)) {
		title = strtok(buf, ";");
		if (title[0] == '#' || title[0] == '\n' || title[0] == ' ')
			continue;
		uri = strtok(NULL, "\n");
		if (!uri)
			continue;
		g_debug("playlist item: title (%s), file: (%s)", title, uri);
		playlist_add_entry(self, title, uri);
	}
	
}

