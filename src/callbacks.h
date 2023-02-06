#include <gnome.h>


gboolean
on_MainWindow_destroy_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_entry_in_activate                   (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_button_calc_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_checkbutton_e_file_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_button_e_file_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_a_file_clicked               (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_MainWindow_destroy_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on__ffnen1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_speichern1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_speichern_unter1_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_beenden1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_info1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_checkbutton_a_file_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data);
