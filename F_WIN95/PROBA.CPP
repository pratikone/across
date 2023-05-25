#include	"w_all.h"

void w_initvaltozok( void ) {
}

//static double Elteltelozo = 0.0;

// Idohuzasra:
void semmi( void ) {
}

void mv_main( void ) {
	mv_startstopper();
	while( 1 ) {
		mv_check();

		char tmp[10];
		long l = mv_stopperido()*1000.0/182.0;
		ltoa( l, tmp, 10 );
		text( 100, tmp );

		/*char tmp[20];
		double eltelt = mv_getsec();
		if( Elteltelozo == 0.0 )
			Elteltelozo = eltelt;
		sprintf( tmp, "%f", (float)(eltelt-Elteltelozo) );
		strcat( tmp, "          " );
		text( 100, tmp );
		Elteltelozo = eltelt;

		for( long l = 0; l < 100000l; )
			l++;
		*/

		//if( szam == 100000l )
		//	error( "Szazezzer!" );

		if( mk_kbhit() ) {
			char text[10];
			int c = mk_getextchar();
			if( c == 'q' )
				mv_exit();
			if( c == 'd' )
				error( "d lenyomva!" );
			itoa( c, text, 10 );
			text( 20, text );
			text[0] = (char)c;
			text[1] = 0;
			text( 40, text );
		}

		for( int i = 0; i < 100; i++ ) {
			if( mk_getstate( MK_ESC ) ) {
				text( 60, "ESC               " );
			}
			if( mk_getstate( MK_ENTER ) ) {
				text( 60, "ENTER               " );
            //ddinit();
			   // Betoltunk valamit back bufferbe:
            unsigned char buff1[640];
            unsigned char buff2[640];
            for( int i = 0; i < 640; i++ ) {
            	if( i%10 >= 5 )
               	buff1[i] = 0;
					else
                  buff1[i] = 1;
					if( i%20 >= 10 )
               	buff2[i] = 0;
					else
                  buff2[i] = 1;
            }

			   for( long l = 0; l < 640; l++ ) {
            	int paros = 0;
            	if( l%2 )
                 	paros = 1;
	           	mv_check();
        	      int xs,ys;
				   unsigned char** ppc = lockbackbuffer( &xs, &ys );
				   for( int y = 0; y < ys; y++ ) {
					  	unsigned char* sor = ppc[y];
               	if( paros )
							//memcpy( sor+1, buff1, 639 );
                     memcpy( sor, buff1, 640 );
	               else
                     //memcpy( sor+1, buff2, 639 );
                     memcpy( sor, buff2, 640 );
		  		   }
		     		unsigned char* sor = ppc[180];
               memset( sor, 50, l );
		  		   unlockbackbuffer();
            }
			   //ddfinish();
			   error( "Befejezte dd probat!" );
			}
			if( mk_getstate( MK_UP ) ) {
				text( 60, "UP               " );
			}
			if( mk_getstate( MK_DOWN ) ) {
				text( 60, "DOWN               " );
			}
			if( mk_getstate( MK_RIGHT ) ) {
				text( 60, "RIGHT               " );
			}
			if( mk_getstate( MK_LEFT ) ) {
				text( 60, "LEFT               " );
			}
         if( mk_getstate( MK_PGUP ) ) {
				text( 60, "PGUP               " );
			}
			if( mk_getstate( MK_PGDOWN ) ) {
				text( 60, "PGDOWN               " );
			}                           
         if( mk_getstate( MK_BACKSPACE ) ) {
				text( 60, "BACKSPACE               " );
			}
		}
	}
}



