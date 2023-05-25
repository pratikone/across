#include	"all.h"


static int negmodulusis( int osztando, int oszto ) {
	if( oszto <= 0 )
		error( "negmodulusis-ben oszto <= 0!" );
	if( osztando >= 0 )
		return osztando % oszto;
	// osztando most mar negativ!
	int mod = oszto-((-osztando)%oszto);
	#ifdef TEST
		if( mod > oszto || mod <= 0 )
			error( "negmodulusis-ben mod > oszto || mod <= 0!" );
	#endif
	if( mod >= oszto )
		mod = 0;
	return mod;
}

ratesz::ratesz( int ysizepar, pic8* ptextpar, int textxalapsizepar ) {
	ptext = ptextpar;
	textxalapsize = textxalapsizepar;
	ysize = ysizepar;
	sorhatok = NULL;
	y0 = 0;
	firsthivas = 1;

	{ sorhatok = NEW(ptrhat*) ptrhat[ysize]; }
	if( !sorhatok )
		uzenet( "memory (sorhatok) ratesz::ratesz-ben!" );
	for( int i = 0; i < ysize; i++ )
		sorhatok[i] = NULL;

	{ firsttomb = NEW(int*) int[ysize]; }
	if( !firsttomb )
		uzenet( "memory (firsttomb) ratesz::ratesz-ben!" );
}

ratesz::~ratesz( void ) {
	if( sorhatok )
		DELETE sorhatok; }
	if( firsttomb )
		DELETE firsttomb; }
}

void ratesz::reset( void ) {
	firsthivas = 1;
}

void ratesz::beallitsort( int y, magassag* pm ) {
	//if( y < 0 )
	//	error( "y < 0 ratesz::beallitsort-ban!" );
	// torlendo:
	//if( y == 333 )
	//	semmi();
	unsigned yindex = negmodulusis( y , ysize );
	firsttomb[yindex] = 1;
	sorhatok[yindex] = pm->getoszlopfirsthat( y );
}

void ratesz::teddra( int x, int y, pic8* phatter, magassag* pm ) {
	// Egy kis ellenorzes:
	//if( x < 0 || y < 0 )
	//	error( "ratesz::teddra-ban x < 0 || y < 0!" );
	if( ysize != phatter->getysize() )
		error( "ratesz::teddra-ban ysize != phatter->getysize()!" );
	if( Displaysizex != phatter->getxsize() )
		error( "ratesz::teddra-ban Displaysizex != phatter->getxsize()!" );
	if( Displaysizey != phatter->getysize() )
		error( "ratesz::teddra-ban Displaysizey != phatter->getysize()!" );
	// No ervenyes sorok beallitasa uj y fuggvenyeben:
	if( firsthivas ) {
		firsthivas = 0;
		y0 = y - 1000;
		if( y0 < 0 )
			y0 = y + 1000;
	}
	// Beallitjuk sorokat:
	if( y < y0 ) {
		for( int futy = y; futy < y0 && futy < y + ysize; futy++ ) {
			beallitsort( futy, pm );
		}
		y0 = y;
	}
	if( y > y0 ) {
		if( y >= y0 + ysize ) {
			// Egesz ujrairodik:
			for( int futy = y; futy < y+ysize; futy++ ) {
				beallitsort( futy, pm );
			}
		}
		else {
			// Csak reszlet irodik ujra:
			for( int futy = y0+ysize; futy < y+ysize; futy++ ) {
				beallitsort( futy, pm );
			}
		}
		y0 = y;
	}
	// Kidrawoljuk sorokat:
	int foldkezdx = negmodulusis( x, textxalapsize );
	#ifdef TEST
		if( foldkezdx+Displaysizex > ptext->getxsize() )
			error( "ratesz::teddra-ban foldkezdx+Displaysizex > ptext->getxsize()" );
	#endif
	int xutani = x+phatter->getxsize();
	//int stopsor = 300;
	int kellhangsor = y+201;
	hang();
	for( int futy = y; futy < y+ysize; futy++ ) {
		if( y == kellhangsor )
			hang();

		//if( futy - y == stopsor )
		//	semmi();

		// Beallitjuk hatter es text sorokra mutato mutatokat:
		int yindex = negmodulusis( futy, ysize );
		unsigned char* phattersor = phatter->getptr( futy-y );

		int yfold = negmodulusis( futy, ptext->getysize() );
		unsigned char* pfoldsor = ptext->getptr( yfold );
		// Kov sor utan pfoldsor karaktersorra fog mutatni, amely display
		// 0. soraval fog kezdodni:
		pfoldsor += foldkezdx;

		// Egy sor elintezese:
		hat* phat = sorhatok[yindex];
		int first = firsttomb[yindex];
		#ifdef TEST
			if( !first && !phat )
				error( "ratesz::teddra-ban !first && !phat!" );
		#endif
		if( first && !phat ) {
			// Ebben a sorban nincs elatmenet, tehat tisztan fold jon ide:
			memcpy( phattersor, pfoldsor, Displaysizex );
			continue;
		}

		// Innentol phat != NULL
		// Megnezzuk, hogy No leptunk-e tul vissza, mert akkor ujra kell
		// kitolteni sort:
		if( !first && x < phat->y ) {
			beallitsort( futy, pm );
			phat = sorhatok[yindex];
			first = firsttomb[yindex];
		}
		// Most biztosan elol all phat:
		#ifdef TEST
			if( !phat || (!first && x < phat->y) )
				error( "ratesz::teddra-ban !phat || (!first && phat->y < x)!" );
		#endif
		int futx = x;
		hat* phatkov = phat->pkov;

		// Atlepjuk azokat a hat-okat, amelyek tul koran vannak:
		while( phatkov && phatkov->y < x ) {
			phat = sorhatok[yindex] = phatkov;
			phatkov = phatkov->pkov;
			first = firsttomb[yindex] = 0;
		}
		// Visszalepunk, ha kell:
		if( x < phat->y ) {
			#ifdef TEST
				if( !first )
					error( "ratesz::teddra-ban !first, pedig x < phat->y!" );
			#endif
			phatkov = phat;
			phat = NULL;
		}
		// Innentol phat lehet NULL:
		while( 1 ) {
			// Most phat es phatkov kozrefogjak futx-et (phat lehet NULL):
			int fold = 1;
			if( first ) {
				if( phat && phat->ffirst && phat->y <= futx )
					fold = 0;
			}
			else {
				fold = !phat->ffirst;
			}
			if( !phatkov || phatkov->y >= xutani ) {
				// Sor vegeig kitolt:
				if( fold )
					memcpy( phattersor+(futx-x), pfoldsor+(futx-x), xutani-futx );
				break;
			}
			// Kitolt phatkov->y -ig:
			if( fold )
				memcpy( phattersor+(futx-x), pfoldsor+(futx-x), phatkov->y-futx );
			futx = phatkov->y;
			phat = phatkov;
			phatkov = phatkov->pkov;
		}
	}
	hang();
}

