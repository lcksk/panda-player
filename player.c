/******************************************************
 *     Video Player für Punk in Wien Ausstellung      *
 *     (C) 2010 Josef Gosch <meibier@gmail.com>       *
 * licensed under the terms of the GNU PUBLIC LICENSE *
 *****************************************************/

#include "player.h"
#include "playlist.h"
#include "callbacks.h"

gboolean bus_call(GstBus *bus, GstMessage *msg, void *user_data)
{
	panda_player *self = (panda_player *)user_data;
	
	switch (GST_MESSAGE_TYPE(msg)) {
		case GST_MESSAGE_EOS: {
			g_debug("End-of-stream");
			player_stop(self);
			break;
		}
		case GST_MESSAGE_ERROR: {
			gchar *debug;
			GError *err;
			gst_message_parse_error(msg, &err, &debug);
			g_free(debug);
 
			g_error("%s", err->message);
			g_error_free(err);
 
			player_stop(self);
			break;
		}
		default:
			break;
	}
 
	return false;
}

void player_stop(panda_player *self) {
	gst_element_set_state(GST_ELEMENT(self->pipeline), GST_STATE_NULL);
	gst_object_unref (GST_OBJECT (self->pipeline));
	self->pipeline = NULL;
	gtk_widget_set_sensitive(self->play_button, FALSE);
	self->playing = FALSE;
}

char *correct_uri (const char *filename) {
	gchar *uri;
	
	
	if (gst_uri_is_valid (filename))
	{
		uri = g_strdup (filename);
	}
	else if (g_path_is_absolute (filename))
	{
		uri = g_filename_to_uri (filename, NULL, NULL);
	}
	else
	{
		gchar *tmp;
		tmp = g_build_filename (g_get_current_dir (), filename, NULL);
		uri = g_filename_to_uri (tmp, NULL, NULL);
		g_free (tmp);
	}

	g_debug ("%s", uri);

	return uri;
}

void set_video_widget(panda_player *self, GtkWidget *widget) {
	self->video_window = (GINT_TO_POINTER (GDK_WINDOW_XWINDOW (widget->window)));
}





void load_uri(panda_player *self, const char *filename) {
	player_stop (self);
	set_video_widget(self, self->video_output);
	self->uri = correct_uri(filename);
	
	if (self->uri == NULL)
		return;
	set_infotext(self->info_widget, "lade video...");
	self->pipeline = gst_pipeline_new ("gst-player");
	self->bin = gst_element_factory_make ("playbin", "bin");
	g_object_set (G_OBJECT (self->bin), "uri", self->uri, NULL);
	self->videosink = gst_element_factory_make ("xvimagesink", "videosink");
	//self->videosink = gst_element_factory_make ("sdlvideosink", "videosink");
	
	g_object_set (G_OBJECT (self->bin), "video-sink", self->videosink, NULL);

	gst_bin_add (GST_BIN (self->pipeline), self->bin);

	self->bus = gst_pipeline_get_bus (GST_PIPELINE (self->pipeline));
	gst_bus_add_watch (self->bus, bus_call, self);
	gst_object_unref (self->bus);
	self->playing = TRUE;
	
	

    g_object_set (G_OBJECT (self->videosink), "force-aspect-ratio", TRUE, NULL);

    if (GST_IS_X_OVERLAY (self->videosink))
    {
		//gst_x_overlay_prepare_xwindow_id(self->video_window);
        gst_x_overlay_set_xwindow_id (GST_X_OVERLAY (self->videosink), GPOINTER_TO_INT (self->video_window));
        gst_x_overlay_handle_events (GST_X_OVERLAY (self->videosink), TRUE);
    }
	gtk_widget_set_sensitive(self->play_button, TRUE);
   
}

gboolean player_is_playing(panda_player *self) {
	uint state;
	gst_element_get_state(self->pipeline, &state, NULL, GST_CLOCK_TIME_NONE);
	g_debug("state: %u", state);
	if (state == GST_STATE_PLAYING)
		return TRUE;
	else
		return FALSE;
}

void player_pause(panda_player *self) {
	g_debug("pause.");
	gst_element_set_state (self->pipeline, GST_STATE_PAUSED);
	gtk_button_set_label(GTK_BUTTON(self->play_button), "gtk-media-play");
}

void player_play(panda_player *self) {
	uint state;
	g_debug("play.");
	gtk_widget_set_double_buffered (self->video_output, FALSE);
	gst_element_set_state (self->pipeline, GST_STATE_PLAYING);
	gst_element_get_state(self->pipeline, &state, NULL, GST_CLOCK_TIME_NONE);
	gtk_button_set_label(GTK_BUTTON(self->play_button), "gtk-media-pause");
	gtk_adjustment_set_upper(self->adj_seek, player_get_stream_length(self)/1000);
}

void player_init(panda_player *self) {
	g_debug("initializing gstreamer\n");
	gst_init(&self->argc, &self->argv);
	self->pipeline = gst_element_factory_make("playbin", "player");
	self->videosink = gst_element_factory_make ("xvimagesink", "videosink");
}



char *format_time(gint64 msec_total, gboolean show_msec) {
	int value = msec_total/1000;
	int msec = msec_total % 1000;
	int hours = value / 3600;
	int min = (value - (hours*3600))/60;
	int sec = value - (hours*3600) - (min*60);
	char *buf=(char *)malloc(1024*sizeof(char));
	
	if (show_msec)
		sprintf(buf, "%d:%02d:%02d.%03d", hours, min, sec, msec);
	else
		sprintf(buf, "%d:%02d:%02d", hours, min, sec);
	return buf;
	
	
}

void player_destroy(panda_player *self) {
	g_debug("unloading gstreamer\n");
	gst_object_unref(GST_OBJECT(self->pipeline));	
}


gboolean player_seek(panda_player *self, gint64 pos) {
	g_debug("seeking to second %lld", GST_TIME_AS_SECONDS(pos));
	return gst_element_seek_simple(self->pipeline, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT, pos);
}
gint64 player_get_stream_position (panda_player *self) {
	GstFormat fmt = GST_FORMAT_TIME;
	gint64 pos = 0;
		
	if (gst_element_query_position (self->pipeline, &fmt, &pos))
		return GST_TIME_AS_MSECONDS(pos);
	return 0;
}

gint64 player_get_stream_length (panda_player *self) {
	GstFormat fmt = GST_FORMAT_TIME;
	gint64 len = 0;
	
	if (gst_element_query_duration(self->pipeline, &fmt, &len))
		return GST_TIME_AS_MSECONDS(len);
	return 0;
}

gboolean player_show_time(panda_player *self) {
	
	
	if (!self->playing)
		return TRUE;
	char buf[2048];
	
	
	gint64 pos = player_get_stream_position(self);
	gint64 len = player_get_stream_length(self);
	sprintf(buf, "%s / %s", format_time(pos, FALSE), format_time(len, FALSE));
	set_infotext(self->info_widget, buf);
	gtk_adjustment_set_value(self->adj_seek, pos/1000);
	return TRUE;
}

void set_infotext (GtkWidget *info, char *message) {
	gtk_label_set_text(GTK_LABEL(info), message);
}

void player_show_controls(panda_player *self) {
	gtk_widget_show(self->controls);
}

void player_hide_controls(panda_player *self) {
	gtk_widget_hide(self->controls);
}


int main(int argc, char **argv) {
	gtk_init (&argc, &argv);
	panda_player *self = (panda_player *)malloc(sizeof(panda_player));
	GtkWidget *button;
	char *home = getenv("HOME");
	char *playlist_path = (char *)malloc(1024*sizeof(char));
	sprintf(playlist_path, "%s/.panda_player/playlist", home);
	
	self->argc=argc;
	self->argv=argv;
	
	
	GtkBuilder *builder;
	builder = gtk_builder_new ();
	gtk_builder_add_from_file (builder, "panda_main.ui", NULL);

	self->window = GTK_WIDGET (gtk_builder_get_object (builder, "panda_main_window"));
	self->notebook = GTK_WIDGET (gtk_builder_get_object (builder, "panda_main_notebook"));
	self->playlist = GTK_WIDGET (gtk_builder_get_object (builder, "panda_playlist_tree"));
	self->info_widget = GTK_WIDGET (gtk_builder_get_object (builder, "info_text"));
	self->scale_seek = GTK_WIDGET (gtk_builder_get_object (builder, "scale_seek"));
	self->adj_seek = GTK_ADJUSTMENT (gtk_builder_get_object (builder, "adj_seek"));
	self->scroll = GTK_WIDGET (gtk_builder_get_object (builder, "hbox2"));
	
	self->hide_playlist_button = GTK_WIDGET (gtk_builder_get_object (builder, "hide_playlist_button"));
	self->pane = GTK_WIDGET (gtk_builder_get_object (builder, "vbox_video"));
	self->controls = GTK_WIDGET (gtk_builder_get_object (builder, "controls"));
	
	self->video_output = gtk_drawing_area_new ();
	GdkColor color;
	gdk_color_parse ("black", &color);
	
	gtk_widget_modify_bg (self->video_output, GTK_STATE_NORMAL, &color);
	
	
	
	

	gtk_widget_set_size_request (self->video_output, 320, 240);
	gtk_box_pack_start (GTK_BOX (self->pane), self->video_output, TRUE, TRUE, 0);
	gtk_widget_show (self->video_output);
	
	// signals
	button = GTK_WIDGET (gtk_builder_get_object (builder, "button_play"));
	gtk_signal_connect(GTK_OBJECT (self->hide_playlist_button), "clicked", GTK_SIGNAL_FUNC (cb_hide_playlist), self);
	gtk_signal_connect(GTK_OBJECT (self->window), "destroy", GTK_SIGNAL_FUNC (on_main_window_destroy), self);
	self->play_button = GTK_WIDGET (gtk_builder_get_object (builder, "button_play"));
	gtk_signal_connect(GTK_OBJECT (self->play_button), "clicked", GTK_SIGNAL_FUNC (cb_player_play_pause), self);
	
	button = GTK_WIDGET (gtk_builder_get_object (builder, "button_stop"));
	gtk_signal_connect(GTK_OBJECT (button), "clicked", GTK_SIGNAL_FUNC (cb_player_stop), self);
	button = GTK_WIDGET (gtk_builder_get_object (builder, "button_next"));
	gtk_signal_connect(GTK_OBJECT (button), "clicked", GTK_SIGNAL_FUNC (cb_player_next), self);
	button = GTK_WIDGET (gtk_builder_get_object (builder, "button_prev"));
	gtk_signal_connect(GTK_OBJECT (button), "clicked", GTK_SIGNAL_FUNC (cb_player_previous), self);
	gtk_signal_connect(GTK_OBJECT (self->scale_seek), "change-value", GTK_SIGNAL_FUNC (cb_player_seek), self);
	
	gtk_signal_connect(GTK_OBJECT(self->playlist), "row-activated", GTK_SIGNAL_FUNC(cb_movie_select), self);
	
	gtk_widget_set_sensitive(self->play_button, FALSE);
	gtk_widget_show(GTK_WIDGET(self->window));
	player_init(self);
	gtk_signal_connect(GTK_OBJECT (self->video_output), "motion-notify-event", GTK_SIGNAL_FUNC (cb_mouse_move), self);
	gtk_adjustment_set_lower(self->adj_seek, 0);
	gtk_adjustment_set_upper(self->adj_seek, 0);
	player_show_controls(self);
	playlist_init(self);
	show_playlist(self);
	self->pl_visible = TRUE;
	self->playing = FALSE;
	g_timeout_add (500, (GSourceFunc) player_show_time, self);
	load_playlist_from_file(self, playlist_path);
	
	gtk_main ();

	return 0;
}
