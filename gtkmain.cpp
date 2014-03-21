#include <gtk/gtk.h>
#include "rcpparse.h"


#define CONFIG						\
  (ANALOG_0_ENABLED					\
  |ANALOG_1_ENABLED					\
  |ANALOG_2_ENABLED					\
  |FREQ_1_ENABLED					\
  |ACCEL_0_ENABLED					\
  |ACCEL_1_ENABLED					\
  |ACCEL_2_ENABLED 					\
  |ACCEL_3_ENABLED					\
  |GPS_LAT_ENABLED					\
  |GPS_LON_ENABLED					\
  |GPS_SPEED_ENABLED					\
  |GPS_TIME_ENABLED					\
  |GPS_SATELITE_ENABLED)


class GuiParser : public RCPParser 
{
public:
  GtkLabel *labels[MAX_CHANNELS];
  GuiParser(ChannelConfig c) : RCPParser(c) {};

private:
  void processSample(float s, ChannelId n)
  {
    char *text = g_strdup_printf("%.2f", s);

    gtk_label_set_text(labels[n], text);
    g_free(text);
  };
};


gboolean idle_func(gpointer parser)
{
  int c;

  if ((c = fgetc(stdin)) != -1) {
    ((GuiParser *) parser)->handleChar(c);
  }

  return TRUE;
}


int
main (int argc, char **argv)
{
  gtk_init(&argc, &argv);

  GtkWidget 
    *w = gtk_window_new(GTK_WINDOW_TOPLEVEL),
    *t = GTK_WIDGET(
        g_object_new(GTK_TYPE_TABLE,
		     "row-spacing", 10,
		     NULL));

  GuiParser p(CONFIG);

  for (int i = 0; i < MAX_CHANNELS; i++) {
    char * text;
    GtkLabel *l = 
      GTK_LABEL(g_object_new(GTK_TYPE_LABEL,
			     "label", "0",
			     "justify", GTK_JUSTIFY_LEFT,
			     NULL));
    p.labels[i] = l;

    gtk_container_add_with_properties(GTK_CONTAINER(t),
				      gtk_label_new((text = 
						     g_strdup_printf("%02d", i))),
				      "top-attach", i,
				      "left-attach", 0,
				      "bottom-attach", i + 1,
				      "right-attach", 1,
				      "x-options", 0,
				      "y-options", 0,
				      NULL);

    g_free(text);

    gtk_container_add_with_properties(GTK_CONTAINER(t),
				      GTK_WIDGET(l),
				      "top-attach", i,
				      "left-attach", 1,
				      "bottom-attach", i + 1,
				      "right-attach", 2,
				      "x-options", GTK_EXPAND | GTK_FILL,
				      "y-options", 0,
				      NULL);

  }

  gtk_container_add(GTK_CONTAINER(w), t);
  gtk_widget_show_all(w);
  g_idle_add(idle_func, &p);
  g_signal_connect(w, "destroy", gtk_main_quit, NULL);

  gtk_main();

  return 0;
}
