#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gnome.h>
#include <ctype.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "enigma.h"

extern  GtkWidget *entry_w1;
extern  GtkWidget *entry_w2;
extern  GtkWidget *entry_w3;
extern  GtkWidget *entry_r1;
extern  GtkWidget *entry_s1;
extern  GtkWidget *entry_r2;
extern  GtkWidget *entry_s2;
extern  GtkWidget *entry_r3;
extern  GtkWidget *entry_s3;
extern  GtkWidget *entry_stecker;
extern  GtkWidget *entry_in;
extern  GtkWidget *entry_out;
extern  GtkWidget *checkbutton_animation;
extern  GtkWidget *checkbutton_e_file;
extern  GtkWidget *checkbutton_a_file;
extern  GtkWidget *button_e_file;
extern  GtkWidget *button_a_file;

gchar *in_name = NULL;
gchar *out_name = NULL;

GtkWidget *file_selector = NULL;

gboolean
on_MainWindow_destroy_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  if( in_name != NULL )
    g_free( in_name );

  if( out_name != NULL )
    g_free( out_name );

  gtk_main_quit();

  return FALSE;
}


void
on_entry_in_activate                   (GtkEditable     *editable,
                                        gpointer         user_data)
{
  on_button_calc_clicked( NULL, NULL );
}


void
on_button_calc_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  Params p;
  gchar *string;
  GtkWidget *widget;
  int i, k;
  int alpha[26];
  gchar *in, *out;
  FILE *fin, *fout;
  gulong size;
 
  /* walzen */
  string =  gtk_entry_get_text( GTK_ENTRY( entry_w1 ) );
  if( (isdigit( string[0] ) == 0) || 
      (string[0]-'0' < 1) || 
      (string[0]-'0' > 5) )
    {
      widget = create_messagebox_walzen();
      gtk_widget_show( widget );

      return;
    }
  else
    p.order[2] = string[0] -'0';
      
  string =  gtk_entry_get_text( GTK_ENTRY( entry_w2 ) );
  if( (isdigit( string[0] ) == 0) || 
      (string[0]-'0' < 1) || 
      (string[0]-'0' > 5) ||
      (string[0]-'0' == p.order[2]))
    {
      widget = create_messagebox_walzen();
      gtk_widget_show( widget );

      return;
    }
  else
      p.order[1] = string[0] -'0';

  string =  gtk_entry_get_text( GTK_ENTRY( entry_w3 ) );
  if( (isdigit( string[0] ) == 0) || 
      (string[0]-'0' < 1) || 
      (string[0]-'0' > 5) ||
      (string[0]-'0' == p.order[2]) ||
      (string[0]-'0' == p.order[1]) )
    {
      widget = create_messagebox_walzen();
      gtk_widget_show( widget );

      return;
    }
  else
      p.order[0] = string[0] -'0';

  /* Ringe */
  string =  gtk_entry_get_text( GTK_ENTRY( entry_r1 ) );
  if( (isalpha( string[0] ) == 0) )
    {
      widget = create_messagebox_ring();
      gtk_widget_show( widget );

      return;
    }
  else
    p.rings[2] = toupper(string[0]);

  string =  gtk_entry_get_text( GTK_ENTRY( entry_r2 ) );
  if( isalpha( string[0] ) == 0)
    {
      widget = create_messagebox_ring();
      gtk_widget_show( widget );

      return;
    }
  else
    p.rings[1] = toupper(string[0]);

  string =  gtk_entry_get_text( GTK_ENTRY( entry_r3 ) );
  if( isalpha( string[0] ) == 0 )
    {
      widget = create_messagebox_ring();
      gtk_widget_show( widget );

      return;
    }
  else
    p.rings[0] = toupper(string[0]);
  
  /* Start */
  string =  gtk_entry_get_text( GTK_ENTRY( entry_s1 ) );
  if( (isalpha( string[0] ) == 0) )
    {
      widget = create_messagebox_start();
      gtk_widget_show( widget );

      return;
    }
  else
    p.pos[2] = toupper(string[0]);

  string =  gtk_entry_get_text( GTK_ENTRY( entry_s2 ) );
  if( (isalpha( string[0] ) == 0) )
    {
      widget = create_messagebox_start();
      gtk_widget_show( widget );

      return;
    }
  else
    p.pos[1] = toupper(string[0]);

  string =  gtk_entry_get_text( GTK_ENTRY( entry_s3 ) );
  if( (isalpha( string[0] ) == 0) )
    {
      widget = create_messagebox_start();
      gtk_widget_show( widget );

      return;
    }
  else
    p.pos[0] = toupper(string[0]);

  /* Stecker */
  string =  gtk_entry_get_text( GTK_ENTRY( entry_stecker ) );
  if( ((strlen( string ) / 2) * 2) != strlen( string ) )
    {
      widget = create_messagebox_stecker();
      gtk_widget_show( widget );

      return;
    }

  for( i = 0; i<strlen( string ); i++ )
    {
      if( isalpha( string[i] ) == 0 )
	{
	  widget = create_messagebox_stecker();
	  gtk_widget_show( widget );
	  
	  return;
	}
      for( k = 0; k<i; k++ )
	if( alpha[k] == string[i] )
	  {
	    widget = create_messagebox_stecker();
	    gtk_widget_show( widget );
	    
	    return;
	  }
      alpha[i] = string[i];
    }
  strncpy( p.plug, string, sizeof( p.plug ) );
  g_strup( (gchar*) p.plug );

  if( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( checkbutton_e_file ) ) )
    {
      if( (in_name == NULL) || ( (fin=fopen( in_name, "rt" )) == NULL) )
	{
	  GtkWidget *widget;

	  widget = gnome_message_box_new(  _("Fehler beim Lesen vom File"), 
					  GNOME_MESSAGE_BOX_ERROR, 
					  GNOME_STOCK_BUTTON_OK,
					  NULL );
	  gtk_widget_show( widget );
	  return;
	}
      fseek( fin, 0, SEEK_END );
      size = ftell( fin );
      fseek( fin, 0, SEEK_SET );
      in = (gchar*) g_malloc( size + 1 );
      fread( in, size, 1, fin );
      in[size] = '\0';

      fclose( fin );
    }
  else
    {
      in = gtk_entry_get_text( GTK_ENTRY(entry_in) );
      if( in == NULL )
	return;

      if( strcmp( in, "" ) == 0 )
	return;
    }
  g_strup( in );

  out = (gchar*) g_malloc( strlen(in) );
  
  for( i=0; i<strlen(in); i++ )
    {
      if( isalpha( in[i] ) == 0 )
	{
	  out[i] = in[i];
	  continue;
	}

      out[i] = scramble( in[i], &p );
    }
  out[i] = '\0';

  if( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( checkbutton_e_file ) ) )
    {
      g_free( in );
    }

  if( gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( checkbutton_a_file ) ) )
    {
      if((out_name == NULL) || ((fout=fopen( out_name, "wt" )) == NULL) )
	{
	  GtkWidget *widget;

	  widget = gnome_message_box_new( _("Fehler beim Schreiben auf File"),
					  GNOME_MESSAGE_BOX_ERROR, 
					  GNOME_STOCK_BUTTON_OK,
					  NULL );
	  gtk_widget_show( widget );
	  g_free( out );
	  return;
	}
      fwrite( out, strlen( out ), 1, fout );
      fclose( fout );
      widget = gnome_message_box_new( _("Die Verschlüsselung, oder Entschlüsselung\n"
					"wurde erfolgreichdurchgeführt."),
				      GNOME_MESSAGE_BOX_INFO,
				      GNOME_STOCK_BUTTON_OK,
				      NULL );
      gtk_widget_show( widget );

    }
  else
    {
      gtk_entry_set_text( GTK_ENTRY( entry_out ), out );
    }
  g_free( out );
}


void
on_checkbutton_e_file_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if( GTK_WIDGET_IS_SENSITIVE( button_e_file ) == TRUE)
    {
      gtk_widget_set_sensitive( button_e_file, FALSE );
      gtk_entry_set_editable( GTK_ENTRY( entry_in ), TRUE);
    }
  else
    {
      gtk_widget_set_sensitive( button_e_file, TRUE );
      gtk_entry_set_editable( GTK_ENTRY( entry_in ), FALSE );
    }
}

void 
on_in_filename( GtkFileSelection *selector, 
		gpointer user_data )
{
  gchar *string;

  if( in_name != NULL )
    g_free( in_name );

  string = gtk_file_selection_get_filename( GTK_FILE_SELECTION( file_selector ) );
  in_name = g_strdup( string );
  g_print("%s\n", in_name);

  string = g_strconcat("file://",in_name,NULL);
  gtk_entry_set_text( GTK_ENTRY( entry_in ), string );
  g_free( string );

}

void 
on_out_filename( GtkFileSelection *selector, 
		 gpointer user_data )
{
  gchar *string;

  if( out_name != NULL )
    g_free( out_name );

  string = gtk_file_selection_get_filename( GTK_FILE_SELECTION( file_selector ) );
  out_name = g_strdup( string );
  g_print("%s\n", out_name);

  string = g_strconcat("file://",out_name,NULL);
  gtk_entry_set_text( GTK_ENTRY( entry_out ), string );
  g_free( string );

}

static void
destroy_file_select	(GtkWidget *selector)
{
    gtk_widget_destroy( selector );
    file_selector = NULL;
}

void
on_button_e_file_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
  if( file_selector != NULL )
    gtk_widget_destroy( file_selector );

  file_selector = gtk_file_selection_new(_("Öffne Datei"));
  gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION(file_selector)->ok_button),
                            "clicked", GTK_SIGNAL_FUNC (on_in_filename), NULL);
                            
    /* Ensure that the dialog box is destroyed when the user clicks a button. */
    
    gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION(file_selector)->ok_button),
                                           "clicked", GTK_SIGNAL_FUNC (destroy_file_select),
                                           (gpointer) file_selector);

    gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION(file_selector)->cancel_button),
                                           "clicked", GTK_SIGNAL_FUNC (destroy_file_select),
                                           (gpointer) file_selector);
    
    /* Display that dialog */
    
    gtk_widget_show (file_selector);

}


void
on_button_a_file_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
  if( file_selector != NULL )
    gtk_widget_destroy( file_selector );

  file_selector = gtk_file_selection_new(_("Speichern unter"));
  gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION(file_selector)->ok_button),
                            "clicked", GTK_SIGNAL_FUNC (on_out_filename), NULL);
                            
    /* Ensure that the dialog box is destroyed when the user clicks a button. */
    
    gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION(file_selector)->ok_button),
                                           "clicked", GTK_SIGNAL_FUNC (destroy_file_select),
                                           (gpointer) file_selector);

    gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION(file_selector)->cancel_button),
                                           "clicked", GTK_SIGNAL_FUNC (destroy_file_select),
                                           (gpointer) file_selector);
    
    /* Display that dialog */
    
    gtk_widget_show (file_selector);

}

void
on__ffnen1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( checkbutton_e_file ), TRUE );
  on_button_e_file_clicked( NULL, NULL );
}


void
on_speichern1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( checkbutton_a_file ), TRUE );

  if( out_name == NULL )
    on_button_a_file_clicked( NULL, NULL );
}


void
on_speichern_unter1_activate           (GtkMenuItem     *menuitem,
                                       gpointer         user_data)
{
  gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( checkbutton_a_file ), TRUE );
  on_button_a_file_clicked( NULL, NULL );
}


void
on_beenden1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_info1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkWidget *about;

  about = create_about1();
  gtk_widget_show( about );
}


void
on_checkbutton_a_file_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if( GTK_WIDGET_IS_SENSITIVE( button_a_file ) == TRUE)
    gtk_widget_set_sensitive( button_a_file, FALSE );
  else
    gtk_widget_set_sensitive( button_a_file, TRUE );
}

