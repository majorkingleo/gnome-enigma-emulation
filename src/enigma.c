/* modified 24-Jan-99 
   attention: Walzen order is correct now
              the programm reads from stdin and puts the text to stdout 
	      errors and help streen are printed to stderr 
*/

/* modified 16-Jan-99 :
   attention : Walzen are in reverse order
               plug array increased
               rotor movement changed */

/* enigma simulation and bombe, harald schmidl, april 1998
  the encoding scheme uses code from Fauzan Mirza's
  3 rotor German Enigma simulation
  Written by  */

#include "enigma.h"

#include <config.h>
#include <gnome.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int loud = 1;

/* Rotor wirings */
char rotor[5][26]={
        /* Input "ABCDEFGHIJKLMNOPQRSTUVWXYZ" */
        /* 1: */ "EKMFLGDQVZNTOWYHXUSPAIBRCJ",
        /* 2: */ "AJDKSIRUXBLHWTMCQGZNPYFVOE",
        /* 3: */ "BDFHJLCPRTXVZNYEIWGAKMUSQO",
        /* 4: */ "ESOVPZJAYQUIRHXLNFTGKDCMWB",
        /* 5: */ "VZBRGITYUPSDNHLXAWMJQOFECK" };
char ref[26]="YRUHQSLDPXNGOKMIEBFZCWVJAT";

char notch[5]="QEVJZ";

/*take a char and return its encoded version according to the 
  encryption params, update params, i.e. advance wheels
  this part uses Fauzan Mirza's code*/
char scramble(char c, Params *p)
{
  int i, j;
  static int start=1;
  static int flag_notch0, flag_notch1 = 0;
  static int flag_notch0_act, flag_notch1_act = 0;
  

                c=toupper(c);
                if (!isalpha(c))
                        return -1;
                /* Attention : stepping of rotors is done before cyphering
                   Check if Grundstellung is already in notch position  */
                if(start) {
                   if (p->pos[0]==notch[p->order[0]-1]) flag_notch0=1;
                   if (p->pos[1]==notch[p->order[1]-1]) flag_notch1=1;
                   start =0;
                }

                flag_notch0_act = flag_notch0;
                flag_notch1_act = flag_notch1;

                /* Step up first rotor always  */
                p->pos[0]++;
                if (p->pos[0]>'Z')
                        p->pos[0] -= 26;
                /* Check if first rotor has reached notch pos */
                if (p->pos[0]==notch[p->order[0]-1]) flag_notch0=1;

                /* Check if first rotor reached notch last time */
                if (flag_notch0_act) {
                        /* rotate second rotor */
                        p->pos[1]++;
                        if (p->pos[1]>'Z')
                                p->pos[1] -= 26;
                        /* Check if second rotor has reached notch pos */
                        if (p->pos[1]==notch[p->order[1]-1])
flag_notch1=1;
                        flag_notch0=0;
                }
                /* Check if second rotor reached notch last time */
                if (flag_notch1_act) {
                        /* rotate third rotor and second rotor */
                        p->pos[2]++;
                        if (p->pos[2]>'Z')
                                p->pos[2] -= 26;
                        p->pos[1]++;
                        if (p->pos[1]>'Z')
                                p->pos[1] -= 26;
                        flag_notch1=0;
                }

		if( loud == 0)
		  fprintf(stderr, _("Walzen Position : %c %c %c\n"),p->pos[2],p->pos[1],p->pos[0]);

                /*  Swap pairs of letters on the plugboard */
                for (i=0; p->plug[i]; i+=2)
                {
                        if (c==p->plug[i])
                                c=p->plug[i+1];
                        else if (c==p->plug[i+1])
                                c=p->plug[i];
                }

                /*  Rotors (forward) */
                for (i=0; i<3; i++)
                {
                        c += p->pos[i]-'A';
                        if (c>'Z')
                                c -= 26;

                        c -= p->rings[i]-'A';
                        if (c<'A')
                                c += 26;

                        c=rotor[p->order[i]-1][c-'A'];

                        c += p->rings[i]-'A';
                        if (c>'Z')
                                c -= 26;

                        c -= p->pos[i]-'A';
                        if (c<'A')
                                c += 26;
                }

                /*  Reflecting rotor */
                c=ref[c-'A'];

                /*  Rotors (reverse) */
                for (i=3; i; i--)
                {
                        c += p->pos[i-1]-'A';
                        if (c>'Z')
                                c -= 26;

                        c -= p->rings[i-1]-'A';
                        if (c<'A')
                                c += 26;

                        for (j=0; j<26; j++)
                                if (rotor[p->order[i-1]-1][j]==c)
                                        break;
                        c=j+'A';

                        c += p->rings[i-1]-'A';
                        if (c>'Z')
                                c -= 26;

                        c -= p->pos[i-1]-'A';
                        if (c<'A')
                                c += 26;
                }
                
                /*  Plugboard */
                for (i=0; p->plug[i]; i+=2)
                {
                        if (c==p->plug[i])
                                c=p->plug[i+1];
                        else if (c==p->plug[i+1])
                                c=p->plug[i];
                }

                /* printf("end scramble :%c\n",c); */
  return c;
}

