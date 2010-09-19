/******************************************************
 *     Video Player für Punk in Wien Ausstellung      *
 *     (C) 2010 Josef Gosch <meibier@gmail.com>       *
 * licensed under the terms of the GNU PUBLIC LICENSE *
 *****************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <glib.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <gst/gst.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <gst/interfaces/xoverlay.h>
#include <stdbool.h>
#include <time.h>
#include <sqlite.h>

typedef struct {
	int argc;
	char **argv;
	GtkWidget *notebook;
	GtkWidget *playlist;
	GtkWidget *window;
	GtkWidget *video_output;
	GtkWidget *hide_playlist_button;
	GtkWidget *pane;
	GtkWidget *play_button;
	GtkWidget *info_widget;
	GtkWidget *scale_seek;
	GtkWidget *controls;
	GtkWidget *scroll;
	GtkListStore *list;
	GtkAdjustment *adj_seek;
	GstElement *pipeline;
	GstElement *videosink;
	gpointer video_window;
	GstElement *bin;
	GstBus *bus;
	char *uri;
	gboolean pl_visible;
	gboolean playing;
	char *playlist_path;
	char *str_videosink;
	char *str_audiosink;
	sqlite *playlist_db;
		
} panda_player;


gboolean bus_call(GstBus *bus, GstMessage *msg, void *user_data);
void player_stop(panda_player *self);
char *correct_uri (const char *filename);
void set_video_widget(panda_player *self, GtkWidget *widget);

void load_uri(panda_player *self, const char *uri);
gboolean player_is_playing(panda_player *self);
char *format_time(gint64 msec_total, gboolean show_msec);
gboolean player_seek(panda_player *self, gint64 pos);
void set_infotext (GtkWidget *info, char *message);

void player_init(panda_player *self);
void player_destroy(panda_player *self);
gint64 player_get_stream_length (panda_player *self);
gint64 player_get_stream_position (panda_player *self);
gboolean player_show_time(panda_player *self);

void player_pause(panda_player *self);
void player_play(panda_player *self);
char *correct_uri(const char *filename);
void set_infotext (GtkWidget *info, char *message);
gboolean player_seek(panda_player *self, gint64 pos);
void player_hide_controls(panda_player *self);
void player_show_controls(panda_player *self);

void load_config(panda_player *self, const char *path);
