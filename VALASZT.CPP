#include	"all.h"

void valaszt::kidraw( int ffirst, ddpal* ppal ) {
	blt8( Korny->picbuffervalaszt, hatter );
	for( int i = 0; i < egykepen && i < szam-ffirst; i++ ) {
		if( tomoren )
			pabc->write( Korny->picbuffervalaszt, x0, y0+i*dy, Korny->rubrikak[ffirst+i], -2 );
		else
			pabc->write( Korny->picbuffervalaszt, x0, y0+i*dy, Korny->rubrikak[ffirst+i] );
	}
	// Jel kidrawolasa:
	char tmp[2] = "A";
	tmp[0] = '@';
	int szelesseg = pabc->len( tmp );                  // x0-37
	pabc->write( Korny->picbuffervalaszt, (x0-szelesseg)/2, y0+(kur-ffirst)*dy+0, tmp );
	lassufizre( Korny->picbuffervalaszt, ppal );
}

valaszt::valaszt( void ) {
	tomoren = 0;
}


// Visszaadja valasztott szamat, ESC eseten -1-t:
int valaszt::valassz( ddpal* ppal, int tobbdraw ) {
	if( szam < 0 || szam > MAXRUBRIKASZAM )
		error( "valassz-ban szam No stimmel!" );

	int ffirst = kur-egykepen/2;
	int maxffirst = szam-egykepen;
	if( ffirst > maxffirst )
		ffirst = maxffirst;
	if( ffirst < 0 )
		ffirst = 0;
	// ffirst be van allitva!
	kidraw( ffirst, ppal );
	if( tobbdraw )
		kidraw( ffirst, ppal );
	while( 1 ) {
		mk_emptychar();
		int c = mk_getextchar();

		//FILE* h = fopen( "billkod.txt", "at" );
		//fprintf( h, "%d\n", c );
		//fclose( h );

		/*if( c == '1' )
			Korny->picbuffervalaszt->save( "menu1.pcx", ppal->tomb );
		if( c == '2' )
			Korny->picbuffervalaszt->save( "menu2.pcx", ppal->tomb );
		if( c == '3' )
			Korny->picbuffervalaszt->save( "menu3.pcx", ppal->tomb );
		if( c == '4' )
			Korny->picbuffervalaszt->save( "menu4.pcx", ppal->tomb );
		if( c == '5' )
			Korny->picbuffervalaszt->save( "menu5.pcx", ppal->tomb );
		if( c == '6' )
			Korny->picbuffervalaszt->save( "menu6.pcx", ppal->tomb );
		if( c == '7' )
			Korny->picbuffervalaszt->save( "menu7.pcx", ppal->tomb );
		if( c == '8' )
			Korny->picbuffervalaszt->save( "menu8.pcx", ppal->tomb );*/


		if( c == MK_ESC && escelheto )
			return -1;
		if( c == MK_ENTER )
			return kur;
		if( c == MK_UP ) {
			if( kur > 0 ) {
				kur--;
				if( kur < ffirst )
					ffirst--;
				kidraw( ffirst, ppal );
			}
		}
		if( c == MK_PGUP ) {
			if( kur > 0 ) {
				kur -= egykepen;
				if( kur < 0 )
					kur = 0;
				if( kur < ffirst )
					ffirst = kur;
				kidraw( ffirst, ppal );
			}
		}
		if( c == MK_DOWN ) {
			if( kur < szam-1 ) {
				kur++;
				if( kur > ffirst+egykepen-1 )
					ffirst++;
				kidraw( ffirst, ppal );
			}
		}
		if( c == MK_PGDOWN ) {
			if( kur < szam-1 ) {
				kur += egykepen;
				if( kur >= szam )
					kur = szam-1;
				if( kur > ffirst+egykepen-1 )
					ffirst = kur-egykepen+1;
				kidraw( ffirst, ppal );
			}
		}
	}
}

