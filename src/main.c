/*
 * Initial main.c file generated by Glade. Edit as required.
 * Glade will not overwrite this file.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gnome.h>
#include <ctype.h>
#include <X11/Xlib.h>

#include "interface.h"
#include "support.h"
#include "enigma.h"
#include <locale.h>

extern int loud;

void 
print_usage() 
{ 
  fprintf( stderr,
	   "genigma " 
	   VERSION
	   "\n%s",
	   _( "Verwendung: genigma [-h] [-help] [--loud] [-l] WALZEN RINGE STARTPOS STECKER\n"
	   "Wollen sie das grafische Programm starten, geben Sie keine Argumente an.\n"
	   ));
}

void 
print_help() 
{
  fprintf(stderr,_(
	  " -h --help      Zeige diesen Bildschirm.\n" 
	  " -l --loud      Schreibe die Walzenstellung nach jedem Zeichen heraus.\n" 
	  " WALZEN         Die Reihenfolge der Walzen. 5 stehen zur Verfügung\n" 
	  "                und 3 werden verwendet.\n" 
	  " RINGE          Die Position der 3 Ringe.\n" 
	  " STARTPOS       Die Startposition der 3 Walzen.\n" 
	  " STECKER        Der Stecker. Maximal 26 Zeichen\n\n" 
	  " Beispiel:\n" 
	  "   $genigma 215 adg bsa hasdfg\n" 
	  " Jetzt verschlüsselt genigma den Text von stdin und gibt auf stdout aus.\n" 
	  " Achtung: Genigma verschlüsselt nur Buchstaben von A bis Z. Alle anderen\n" 
	  " Zeichen werden nicht verschlüsselt sondern direkt zum stdout geschickt.\n" 
	 ) );
}


int
main (int argc, char *argv[])
{
  GtkWidget *MainWindow;
  gboolean use_gui;
  gboolean p1, p2, p3;
  Params p;
  int i, k;
  char c, o;
  int alpha[26];

  setlocale (LC_ALL, "");
#ifdef ENABLE_NLS
  bindtextdomain (PACKAGE, PACKAGE_LOCALE_DIR);
  textdomain (PACKAGE);
#endif

  use_gui = TRUE;
  p1 = p2 = p3 = FALSE;

  if( argc == 2 )
    {
      print_usage();

      if( (strcmp( argv[1], "-h") == 0 ) || (strcmp( argv[1], "--help" ) == 0 ) )
	print_help();

      return 1;
    }

  if( argc > 2 )
    {
      i = 0;
      use_gui = FALSE;

      for( k=1; k < argc; k++ )
	{
	  if( (strcmp(argv[k],"-h") == 0) || (strcmp(argv[k], "--help") == 0) )
	    {
	      print_usage();
	      print_help();
	      return 1;
	    }
	  if( (strcmp(argv[k],"-l") == 0) || (strcmp(argv[k], "--loud") == 0) )
	    {
	      loud = 0;
	      if (k < 4)
		i = k;
	    }
	  else if (*argv[k] == '-') {
	      print_usage();
	      return 1;
	  }
	}

      if( argc > 3 + i )
	{
	  if( (strlen( argv[1+i] ) >= 3 ) && (strlen( argv[2+i] ) >= 3 ) && (strlen( argv[3+i] ) >= 3 ) )
	    {
	      p.order[2] = argv[1+i][0] - '0';
	      p.order[1] = argv[1+i][1] - '0';
	      p.order[0] = argv[1+i][2] - '0';

	      if( (p.order[0] == p.order[1] ) ||
		  (p.order[0] == p.order[2] ) ||
		  (p.order[2] == p.order[1] ) )
		{
		  print_usage();
		  print_help();
		  return 1;
		}

	      if( (p.order[0] < 1) || ( p.order[0] > 5 ) ||
		  (p.order[1] < 1) || ( p.order[1] > 5 ) ||
		  (p.order[2] < 1) || ( p.order[2] > 5 ) )
		{
		  print_usage();
		  print_help();
		  return 1;
		}
	      p1 = TRUE;
	      
	      p.rings[2] = toupper( argv[2+i][0] );
	      p.rings[1] = toupper( argv[2+i][1] );
	      p.rings[0] = toupper( argv[2+i][2] );

	      for( k = 0; k < 3; k++ )
		if( isalpha( p.rings[k] ) == 0 )
		{
		  print_usage();
		  print_help();
		  return 1;
		}
	      p2 = TRUE;	      

	      p.pos[2] = toupper( argv[3+i][0] );
	      p.pos[1] = toupper( argv[3+i][1] );
	      p.pos[0] = toupper( argv[3+i][2] );

	      for( k = 0; k < 3; k++ )
		if( isalpha( p.pos[k] ) == 0 )
		{
		  print_usage();
		  print_help();
		  return 1;
		}
	      p3 = TRUE;
	    }      
	  else 
	    {
	      print_usage();
	      print_help();
	      return 1;
	    }

	  if(argc > 4 +i && *argv[4+i] != '-')
	    {
	      strncpy( p.plug, argv[4+i], sizeof( p.plug ) );
	      g_strup( (gchar*) p.plug );

	      if( ((strlen( p.plug ) / 2) *2) != strlen( p.plug ) )
		{
		  print_usage();
		  print_help();
		  return 1;
		}

	      for( i = 0; i<strlen( p.plug ); i++ )
		{
		  if( isalpha( p.plug[i] ) == 0 )
		    {
		      print_usage();
		      print_help();
		      return 1;
		    }
		  for( k=0; k<i; k++ )
		    if( alpha[k] == p.plug[i] )
		      {
			print_usage();
			print_help();
			return 1;
		      }
		  alpha[i] = p.plug[i];
		}
	    }
	  else
	    {
	      strcpy( p.plug, "" );
	    }
	}
    }
  
  if( use_gui == TRUE )
    {
      Display *dis;

      if( (dis = XOpenDisplay(NULL)) == NULL )
	{
	  print_usage();
	  if( argc > 1 )
	    if( (strcmp( argv[1], "-h" ) == 0) || (strcmp( argv[1], "--help" ) == 0) )
	      print_help();
	  return 0;
	}
      else
	{
	  XCloseDisplay( dis );
	}
      /*
       * The following code was added by Glade to create one of each component
       * (except popup menus), just so that you see something after building
       * the project. Delete any components that you don't want shown initially.
       */
      gnome_init ("genigma", VERSION, argc, argv);

      MainWindow = create_MainWindow ();
      gtk_widget_show (MainWindow);

      gtk_main ();
      return 0;
    }
  else
    {
      if( (p1 != TRUE) || 
	  (p2 != TRUE) ||
	  (p3 != TRUE) )
	{
	  print_usage();
	  return 1;
	}

        fprintf(stderr, 
	  _("Walzen %d %d %d Start %c %c %c Ringe %c %c %c Stecker \"%s\"\n"),
	  p.order[2], p.order[1], p.order[0], 
	  p.pos[2], p.pos[1], p.pos[0],
	  p.rings[02], p.rings[1], p.rings[0], p.plug);


	while( (i = getc( stdin )) !=EOF )
	  {
	    c = (char) toupper( i );
	    if( (c == 'A') ||
		(c == 'B') ||
		(c == 'C') ||
		(c == 'D') ||
		(c == 'E') ||
		(c == 'F') ||
		(c == 'G') ||
		(c == 'H') ||
		(c == 'I') ||
		(c == 'J') ||
		(c == 'K') ||
		(c == 'L') ||
		(c == 'M') ||
		(c == 'N') ||
		(c == 'O') ||
		(c == 'P') ||
		(c == 'Q') ||
		(c == 'R') ||
		(c == 'S') ||
		(c == 'T') ||
		(c == 'U') ||
		(c == 'V') ||
		(c == 'W') ||
		(c == 'X') ||
		(c == 'Y') ||
		(c == 'Z') )
	      o = scramble( c, &p );
	    else
	      o = c;
	    putc( o, stdout );
	  }
      return 0;
    }

  return 1;
}

