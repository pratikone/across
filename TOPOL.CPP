#include	"all.h"

// Ertekeik kommentkent kint is vannak:

#define BELYEGSZORZO (3247.764325643)

int Maxpont_sarvari =  10;
int Maxkerek_sarvari = 8;
int Maxgyuru_sarvari = 3;

// KEREK KEREK KEREK KEREK KEREK KEREK KEREK KEREK KEREK
// KEREK KEREK KEREK KEREK KEREK KEREK KEREK KEREK KEREK
// KEREK KEREK KEREK KEREK KEREK KEREK KEREK KEREK KEREK
// KEREK KEREK KEREK KEREK KEREK KEREK KEREK KEREK KEREK

kerek::kerek( double x, double y, int tipusp ) {
	r.x = x;
	r.y = y;
	tipus = tipusp;
}

static const double Hal_eltol = -0.15 + 0.3 * (0.07/0.37);

void kerek::kidrawol( void ) {
	int szegmensszam = 16;
	double sugar = 0.4;

	double elfordulas = 2.0*K_pi/szegmensszam;
	for( int i = 0; i < szegmensszam; i++ ) {
		double alfa1 = i*elfordulas;
		double alfa2 = (i+1)*elfordulas;
		vector2 r1( sugar*sin( alfa1 ), sugar*cos( alfa1 ) );
		vector2 r2( sugar*sin( alfa2 ), sugar*cos( alfa2 ) );
		LINEdraw( r+r1, r+r2, 0 );
	}

	// Kis jel kor kozepere:
	double meret = 0.02;
	LINEdraw( r+vector2( -meret, -meret ),  r+vector2( meret, meret ), 0 );
	LINEdraw( r+vector2( meret, -meret ),  r+vector2( -meret, meret ), 0 );

	// Beleirjuk betut korbe:
	if( tipus == T_CEL ) {
		LINEdraw( r+vector2( -0.15, 0.3 ),  r+vector2( -0.15, -0.3 ), 0 );
		LINEdraw( r+vector2( -0.15, 0.3 ),  r+vector2( 0.15, 0.3 ), 0 );
		LINEdraw( r+vector2( -0.15, -0.3 ), r+vector2( 0.15, -0.3 ), 0 );
		LINEdraw( r+vector2( -0.15, 0.0 ),  r+vector2( 0.1, 0.0 ), 0 );
		return;
	}
	if( tipus == T_KAJA ) {
		LINEdraw( r+vector2( -0.15, 0.3 ),  r+vector2( -0.15, -0.3 ), 0 );
		LINEdraw( r+vector2( -0.15, -0.3 ), r+vector2( 0.15, -0.3 ), 0 );
		LINEdraw( r+vector2( -0.15, 0.0 ),  r+vector2( 0.1, 0.0 ), 0 );
		return;
	}
	if( tipus == T_KEZDO ) {
		LINEdraw( r+vector2( 0.15, 0.3 ),  r+vector2( 0.15, 0.0 ), 0 );
		LINEdraw( r+vector2( -0.15, -0.3 ),  r+vector2( -0.15, 0.0 ), 0 );

		LINEdraw( r+vector2( -0.15, 0.3 ),  r+vector2( 0.15, 0.3 ), 0 );
		LINEdraw( r+vector2( -0.15, -0.3 ), r+vector2( 0.15, -0.3 ), 0 );
		LINEdraw( r+vector2( -0.15, 0.0 ),  r+vector2( 0.15, 0.0 ), 0 );
		return;
	}
	if( tipus == T_HALALOS ) {
		LINEdraw( r+vector2( -0.15, 0.3 ),  r+vector2( -0.15, -0.3 ), 0 );
		LINEdraw( r+vector2( -0.15, 0.07 ),  r+vector2( 0.15, -0.3 ), 0 );
		LINEdraw( r+vector2( Hal_eltol, 0.0 ),  r+vector2( 0.15, 0.3 ), 0 );
		return;
	}
	error( "iugiffiif" );
}

kerek::kerek( FILE* h ) {
	if( fread( &r.x, 1, sizeof( r.x ), h ) != sizeof( r.x ) )
		error( "No olvas file-bol kerek::kerek-ben!" );
	if( fread( &r.y, 1, sizeof( r.y ), h ) != sizeof( r.y ) )
		error( "No olvas file-bol kerek::kerek-ben!" );
	if( fread( &tipus, 1, sizeof( tipus ), h ) != sizeof( tipus ) )
		error( "No olvas file-bol kerek::kerek-ben!" );
}

void kerek::save( FILE* h ) {
	if( fwrite( &r.x, 1, sizeof( r.x ), h ) != sizeof( r.x ) )
		error( "No ir file-ba kerek::save-ben!" );
	if( fwrite( &r.y, 1, sizeof( r.y ), h ) != sizeof( r.y ) )
		error( "No ir file-ba kerek::save-ben!" );
	if( fwrite( &tipus, 1, sizeof( tipus ), h ) != sizeof( tipus ) )
		error( "No ir file-ba kerek::save-ben!" );
}

double kerek::belyegszamitas( void ) {
	double belyeg = 0;
	belyeg += r.x;
	belyeg += r.y;
	belyeg += tipus;
	return belyeg;
}

// TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL
// TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL
// TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL
// TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL TOPOL

topol::topol( void ) {
	errors = 0;
	lezart = 0;
	for( int i = 0; i < MAXGYURU; i++ )
		ptomb[i] = NULL;
	for( i = 0; i < MAXKEREK; i++ )
		kerektomb[i] = NULL;

	ptomb[0] = new gyuru;
	kerektomb[0] = new kerek( -2, 0.5, T_CEL );
	kerektomb[1] = new kerek( 2, 0.5, T_KEZDO );
	strcpy( levelname, "Unnamed" );
}

topol::~topol( void ) {
	for( int i = 0; i < MAXGYURU; i++ ) {
		if( ptomb[i] )
			delete ptomb[i];
		ptomb[i] = NULL;
	}
}

gyuru* topol::legkozpont( double x, double y, int* pk, double* ptav ) {
	if( ptav )
		*ptav = 1000000000.0;

	double tav = 1000000.0;
	gyuru* pgy = NULL;
	for( int i = 0; i < MAXGYURU; i++ ) {
		if( ptomb[i] ) {
			int ujk;
			double ujtav = ptomb[i]->legkozpont( x, y, &ujk );
			if( ujtav < tav ) {
				tav = ujtav;
				*pk = ujk;
				pgy = ptomb[i];
			}
		}
	}
	if( tav > display_double_kozeltav() ) {
		*pk = 0;
		return NULL;
	}
	if( ptav )
	*ptav = tav;
	return pgy;
}

kerek* topol::legkozkerek( double x, double y, double* ptav ) {
	if( ptav )
		*ptav = 1000000000.0;

	double tav = 1000000.0;
	kerek* pker = NULL;
	vector2 r( x, y );
	for( int i = 0; i < MAXKEREK; i++ ) {
		if( kerektomb[i] ) {
			double ujtav = abs( kerektomb[i]->r - r );
			if( ujtav < tav ) {
				tav = ujtav;
				pker = kerektomb[i];
			}
		}
	}
	if( tav > display_double_kozeltav() ) {
		return NULL;
	}
	if( ptav )
		*ptav = tav;
	return pker;
}

void topol::kidrawol( void ) {
	for( int i = 0; i < MAXGYURU; i++ ) {
		if( ptomb[i] ) {
			ptomb[i]->osszdraw();
		}
	}
	for( i = 0; i < MAXKEREK; i++ ) {
		if( kerektomb[i] ) {
			kerektomb[i]->kidrawol();
		}
	}
}

// Igaz, ha bent van:
int topol::levegoben( gyuru* pgy, vector2* pr ) {
	if( (pgy && pr) || (!pgy && !pr) )
		error( "topol::levegoben-ben paramok No stimmelnek!" );

	vector2 r;
	if( pgy )
		r = pgy->ponttomb[0];  //pgy->sulypont();
	else
		r = *pr;
	vector2 v = vector2( 27654.475374565578, 37850.5364775 );
	// Vegigmegyunk az egeszen:
	int metszes = 0;
	for( int i = 0; i < MAXGYURU; i++ ) {
		if( ptomb[i] && ptomb[i] != pgy )
			metszes += ptomb[i]->metszesszam( r, v );
	}
	if( metszes%2 )
		return 1;
	else
		return 0;
}

topol::topol( char* filename, int kulso ) {
	errors = 0;
	lezart = 0;

	FILE* h = NULL;
	if( kulso )
		h = fopen( filename, "rb" );
	else
		h = qopen( filename, "rb" );
	if( !h )
		error( "topol::topol-ban No tudta nyitni file-t!: ", filename );

	for( int i = 0; i < MAXGYURU; i++ )
			ptomb[i] = NULL;
	for( i = 0; i < MAXKEREK; i++ )
		kerektomb[i] = NULL;

	// Verzio beolvasasa:
	char tmp[10] = "AAAAA";
	if( fread( tmp, 1, 5, h ) != 5 )
		error( "No tudott olvasni txt file-bol!" );
	if( strncmp( tmp, "POT", 3 ) != 0 )
		error( "topol::topol( char* filename )-ben No stimmel eleje!" );

	unsigned long belyeg_l = 0;
	double belyeg = 0;
	int verzio = tmp[4] - '0' + 10*(tmp[3] - '0');

	if( verzio > 6 )
		uzenet( "Level file's version is too new!: ", filename );

	if( verzio < 6 )
		uzenet( "Corrupt level file!: ", filename );

	/*if( verzio == 1 ) {
		// Ezt az agat ki kell majd torolni veglegesbol, mivel
		// 1-es verzio No ervenyes:

		// Belyeg:
		if( fread( &belyeg, 1, 4, h ) != 4 )
			error( "No tudott olvasni lev file-bol!" );

		// Beolvassuk hogy errors-e:
		double errorertek;
		if( fread( &errorertek, 1, sizeof( errorertek ), h ) != sizeof( errorertek ) )
			error( "No tudott olvasni lev file-bol!" );
		if( errorertek + double(belyeg) < 9786.0 ||
			errorertek + double(belyeg) > 36546.0 )
			error( "topol::topol-ban errorertek kivul esik tartomanyon!" );
		if( errorertek + double(belyeg) > 20000.0 )
			errors = 1;

		// Beolvassuk hogy lezart-e:
		double lezartertek;
		if( fread( &lezartertek, 1, sizeof( lezartertek ), h ) != sizeof( lezartertek ) )
			error( "No tudott olvasni lev file-bol!" );
		if( lezartertek + double(belyeg) < 9875.0 ||
			lezartertek + double(belyeg) > 32345.0 )
			error( "topol::topol-ban lezartertek kivul esik tartomanyon!" );
		if( lezartertek + double(belyeg) > 20000.0 )
			lezart = 1;

	}
	if( verzio == 2 ) {
		// Ezt az agat ki kell majd torolni veglegesbol, mivel
		// 1-es verzio No ervenyes:

		// Belyeg:
		long belyeg_l;
		if( fread( &belyeg_l, 1, 4, h ) != 4 )
			error( "No tudott olvasni lev file-bol!" );

		// Beolvassuk hogy shareware-e:
		double sarvarertek;
		if( fread( &sarvarertek, 1, sizeof( sarvarertek ), h ) != sizeof( sarvarertek ) )
			error( "No tudott olvasni lev file-bol!" );
		if( sarvarertek + double(belyeg_l) < 9786.0 ||
			sarvarertek + double(belyeg_l) > 36546.0 )
			error( "topol::topol-ban sarvarertek kivul esik tartomanyon!" );

		// Ha igaz, akkor ezt a level file-t regisztraltbol-bol mentettek:
		// Csak en regisztraltbol is sarvar-nak mentek:
		#ifdef SARVARI
			if( sarvarertek + double(belyeg_l) <= 20000.0 )
				uzenet( "Sorry, this level file can only be played with the registered version!:\n",
													filename );
		#endif

		// Beolvassuk hogy errors-e:
		double errorertek;
		if( fread( &errorertek, 1, sizeof( errorertek ), h ) != sizeof( errorertek ) )
			error( "No tudott olvasni lev file-bol!" );
		if( errorertek + double(belyeg_l) < 9786.0 ||
			errorertek + double(belyeg_l) > 36546.0 )
			error( "topol::topol-ban errorertek kivul esik tartomanyon!" );
		if( errorertek + double(belyeg_l) > 20000.0 )
			errors = 1;

		// Beolvassuk hogy lezart-e:
		double lezartertek;
		if( fread( &lezartertek, 1, sizeof( lezartertek ), h ) != sizeof( lezartertek ) )
			error( "No tudott olvasni lev file-bol!" );
		if( lezartertek + double(belyeg_l) < 9875.0 ||
			lezartertek + double(belyeg_l) > 32345.0 )
			error( "topol::topol-ban lezartertek kivul esik tartomanyon!" );
		if( lezartertek + double(belyeg_l) > 20000.0 )
			lezart = 1;
	}
	if( verzio == 3 ) {
		// Belyeg:
		if( fread( &belyeg, 1, 4, h ) != 4 )
			error( "No tudott olvasni lev file-bol!" );

		// Beolvassuk hogy shareware-e:
		double sarvarertek;
		if( fread( &sarvarertek, 1, sizeof( sarvarertek ), h ) != sizeof( sarvarertek ) )
			error( "No tudott olvasni lev file-bol!" );

		// Beolvassuk hogy errors-e:
		double errorertek;
		if( fread( &errorertek, 1, sizeof( errorertek ), h ) != sizeof( errorertek ) )
			error( "No tudott olvasni lev file-bol!" );
		errors = 0;

		// Beolvassuk hogy lezart-e:
		double lezartertek;
		if( fread( &lezartertek, 1, sizeof( lezartertek ), h ) != sizeof( lezartertek ) )
			error( "No tudott olvasni lev file-bol!" );
		lezart = 0;
	}
	if( verzio == 4 ) {
		// Belyeg:
		if( fread( &belyeg_l, 1, 4, h ) != 4 )
			error( "No tudott olvasni lev file-bol!" );
		if( fread( &belyeg, 1, sizeof( belyeg ), h ) != sizeof( double ) )
			error( "No tudott olvasni lev file-bol double belyeget!" );

		double sarvarertek;
		if( fread( &sarvarertek, 1, sizeof( sarvarertek ), h ) != sizeof( sarvarertek ) )
			error( "No tudott olvasni lev file-bol!" );

		double errorertek;
		if( fread( &errorertek, 1, sizeof( errorertek ), h ) != sizeof( errorertek ) )
			error( "No tudott olvasni lev file-bol!" );

		double lezartertek;
		if( fread( &lezartertek, 1, sizeof( lezartertek ), h ) != sizeof( lezartertek ) )
			error( "No tudott olvasni lev file-bol!" );
	}*/
	if( verzio == 6 /*|| verzio == 5*/ ) {
		// Belyeg:
		if( fread( &belyeg_l, 1, 4, h ) != 4 )
			error( "No tudott olvasni lev file-bol!" );
		if( fread( &belyeg, 1, sizeof( belyeg ), h ) != sizeof( double ) )
			error( "No tudott olvasni lev file-bol double belyeget!" );

		// Beolvassuk hogy shareware-e:
		double sarvarertek;
		if( fread( &sarvarertek, 1, sizeof( sarvarertek ), h ) != sizeof( sarvarertek ) )
			error( "No tudott olvasni lev file-bol!" );
		if( sarvarertek + belyeg < 9786.0 ||
			sarvarertek + belyeg > 36546.0 )
			error( "topol::topol-ban sarvarertek kivul esik tartomanyon!" );

		// Ha igaz, akkor ezt a level file-t regisztraltbol-bol mentettek:
		// Csak en regisztraltbol is sarvar-nak mentek:
		#ifdef SARVARI
			if( sarvarertek + belyeg <= 20000.0 )
				uzenet( "Sorry, this level file can only be loaded into the registered version!:\n",
													filename );
		#endif

		// Beolvassuk hogy errors-e:
		double errorertek;
		if( fread( &errorertek, 1, sizeof( errorertek ), h ) != sizeof( errorertek ) )
			error( "No tudott olvasni lev file-bol!" );
		if( errorertek + belyeg < 9786.0 ||
			errorertek + belyeg > 36546.0 )
			error( "topol::topol-ban errorertek kivul esik tartomanyon!" );
		if( errorertek + belyeg > 20000.0 )
			errors = 1;

		// Beolvassuk hogy lezart-e:
		double lezartertek;
		if( fread( &lezartertek, 1, sizeof( lezartertek ), h ) != sizeof( lezartertek ) )
			error( "No tudott olvasni lev file-bol!" );
		if( lezartertek + belyeg < 9875.0 ||
			lezartertek + belyeg > 32345.0 )
			error( "topol::topol-ban lezartertek kivul esik tartomanyon!" );
		if( lezartertek + belyeg > 20000.0 )
			lezart = 1;

		//lezart = 0; // Ha ez nincs kikomentezve, unlockol minden file-t
	}

	// Beolvassuk level nameet:
	fread( levelname, 1, LEVELNAMEHOSSZ+1, h );
	levelname[LEVELNAMEHOSSZ] = 0; // Ez csak biztonsagert

	// Gyuruk szamat 100 byte utan kezdjuk:
	if( kulso )
		fseek( h, 100, SEEK_SET );
	else
		qseek( h, 100, SEEK_SET );
	double gyuruolvszam = 0;
	if( fread( &gyuruolvszam, 1, sizeof( gyuruolvszam ), h ) != sizeof( gyuruolvszam ) )
		error( "topol::topol-ban No tudta olvasni lebego gyuruszamot!" );

	int gyuruszam = gyuruolvszam;
	if( gyuruszam > MAXGYURU )
		error( "topol::topol-ban gyuruszam > MAXGYURU!: " );
	if( gyuruszam <= 0 )
		error( "topol::topol-ban gyuruszam <= 0!" );

	for( i = 0; i < gyuruszam; i++ ) {
		// Beolvassa sok gyurut:
		ptomb[i] = new gyuru( h );
	}

	// Gyuruk szama:
	double kerekolvszam = 0;
	if( fread( &kerekolvszam, 1, sizeof( kerekolvszam ), h ) != sizeof( kerekolvszam ) )
		error( "topol::topol-ban No tudta olvasni lebego kerekszamot!" );

	int kerekszam = kerekolvszam;
	if( kerekszam > MAXKEREK )
		error( "topol::topol-ban kerekszam > MAXKEREK!: " );
	if( kerekszam <= 0 )
		error( "topol::topol-ban kerekszam <= 0!" );

	for( i = 0; i < kerekszam; i++ ) {
		// Beolvassa sok gyurut:
		kerektomb[i] = new kerek( h );
	}

	if( kulso )
		fclose( h );
	else
		qclose( h );

	// Most megnezzuk, hogy belyeg stimmel-e:
	if( verzio == 6 ) {
		double szamitottbelyeg = belyegszamitas();
		if( fabs( szamitottbelyeg - belyeg ) > 0.01 )
			uzenet( "Corrupt .LEV file!: ", filename );
	}
}

static long longosit( double belyeg ) {
	srand( clock() );
	unsigned long veletlen = s_random(6542);
	veletlen *= s_random(7042);
	veletlen += s_random(4542);
	veletlen *= s_random(3042);
	veletlen *= s_random(3742);
	veletlen += s_random(9187);

	belyeg = sin( belyeg );
	belyeg *= (belyeg+1.0001)*40000;
	unsigned long l = belyeg;

	unsigned long belyeg_l = ( l & 0x0000ffff ) |
							 ( veletlen & 0xffff0000 );
	return belyeg_l;
}

void topol::save( char* filename ) {
	// Mivel ezzel meg valtoztathatunk polygonokon
	// (polygon korbejaras megvaltoztatas, egymason fekvo pontok
	// megszuntetese, tul hegyes szogek eltolasa), ezert ezt elore tesszuk:
	errors = check_topology( 0 );
	if( errors && lezart ) {
		dialog( "You cannot save this file as a locked file because there are",
				"some topology errors in the design and you could not play on it!" );
		return;
	}


	FILE* h = fopen( filename, "wb" );
	if( !h )
		error( "topol::save-ben No tudta nyitni file-t!: ", filename );

	fwrite( "POT06", 1, 5, h );

	// Kiszamitjuk es kiirjuk ketfele belyeget:
	double belyeg = belyegszamitas();
	unsigned long belyeg_l = longosit( belyeg );

	fwrite( &belyeg_l, 1, 4, h );
	fwrite( &belyeg, 1, sizeof( belyeg ), h );

	// Kiirjuk hogy sarvari-e:
	// Ha sarvarertek + double(belyeg_l) > 20000.0, akkor sarvari:
	#ifdef SARVARI
		double sarvarertek = 20961.0 + s_random( 4982 ) - belyeg;
	#else
		double sarvarertek = 11877.0 + s_random( 5871 ) - belyeg;
	#endif

	// Ezt a sort vegleges verzioban ki kell kommentezni:
	// Regisztralt verzioju file-t is shareware-nek tunteti fol:
	//sarvarertek = 20961.0 + s_random( 4982 ) - belyeg;

	fwrite( &sarvarertek, 1, sizeof( sarvarertek ), h );

	// Kiirjuk hogy errors-e:
	// Ha errorertek + double(belyeg_l) > 20000.0, akkor errors:
	double errorertek = 11877.0 + s_random( 5871 ) - belyeg;
	if( errors ) // ezt mar elejen meghataroztuk
		errorertek = 20961.0 + s_random( 4982 ) - belyeg;
	fwrite( &errorertek, 1, sizeof( errorertek ), h );

	// Kiirjuk hogy lezart-e:
	double lezartertek = 12112.0 + s_random( 6102 ) - belyeg;
	if( lezart )
		lezartertek = 23090.0 + s_random( 6310 ) - belyeg;
	fwrite( &lezartertek, 1, sizeof( errorertek ), h );

	// Kiirjuk level nameet:
	fwrite( levelname, 1, LEVELNAMEHOSSZ+1, h );

	// Kimentjuk gyuruket elejetol kezdve 100 byte utan:
	fseek( h, 100, SEEK_SET );
	int gyuruszam = 0;
	for( int i = 0; i < MAXGYURU; i++ ) {
		if( ptomb[i] )
			gyuruszam++;
	}
	double gyuruirtszam = gyuruszam+0.4643643;
	fwrite( &gyuruirtszam, 1, sizeof( gyuruirtszam ), h );
	for( i = 0; i < MAXGYURU; i++ ) {
		if( ptomb[i] )
			ptomb[i]->save( h, this );
	}
	// Kimentjuk kerekeket:
	int kerekszam = 0;
	for( i = 0; i < MAXKEREK; i++ ) {
		if( kerektomb[i] )
			kerekszam++;
	}
	double kerekirtszam = kerekszam+0.4643643;
	fwrite( &kerekirtszam, 1, sizeof( kerekirtszam ), h );
	for( i = 0; i < MAXKEREK; i++ ) {
		if( kerektomb[i] )
			kerektomb[i]->save( h );
	}

	fclose( h );

	if( errors )
		dialog( "Though the level file was successfully saved, there are some errors in the design.",
				"You cannot play on this level until you correct these problems. To see what the",
				"problems are, please push the Check Topology button in the editor." );
}

void topol::getminmax( double* px1, double* py1, double* px2, double* py2,
											int kerekkel ) {
	*px1 = 100000000000.0;
	*py1 = 100000000000.0;
	*px2 = -100000000000.0;
	*py2 = -100000000000.0;
	for( int i = 0; i < MAXGYURU; i++ ) {
		if( ptomb[i] ) {
			ptomb[i]->getminmax( px1, py1, px2, py2 );
		}
	}
	if( kerekkel ) {
		for( i = 0; i < MAXKEREK; i++ ) {
			if( kerektomb[i] ) {
				if( *px1 > kerektomb[i]->r.x )
					*px1 = kerektomb[i]->r.x;
				if( *px2 < kerektomb[i]->r.x )
					*px2 = kerektomb[i]->r.x;
				if( *py1 > kerektomb[i]->r.y )
					*py1 = kerektomb[i]->r.y;
				if( *py2 < kerektomb[i]->r.y )
					*py2 = kerektomb[i]->r.y;
			}
		}
	}
}

double topol::belyegszamitas( void ) {
	double belyeg = 0.0;
	for( int i = 0; i < MAXGYURU; i++ ) {
		if( ptomb[i] ) {
			belyeg += ptomb[i]->belyegszamitas();
		}
	}
	for( i = 0; i < MAXKEREK; i++ ) {
		if( kerektomb[i] ) {
			belyeg += kerektomb[i]->belyegszamitas();
		}
	}
	return BELYEGSZORZO*belyeg;
}

// Ha Sarvari, megvizsgalja hogy palya megfelel-e sarvari verzionak:
// Ha No, akkor kilep corrupt erroruzenettel:
void topol::checklevelforsarvari( void ) {
	//Maxpont_sarvari
	//Maxkerek_sarvari
	//Maxgyuru_sarvari
	#ifdef SARVARI
		int gyuruszam = 0;
		for( int i = 0; i < MAXGYURU; i++ ) {
			if( ptomb[i] ) {
				gyuruszam++;
				if( ptomb[i]->pontszam > Maxpont_sarvari )
					error( "checklevelforsarvari-ban tul sok pont!" );
			}
		}
		if( gyuruszam > Maxgyuru_sarvari )
			error( "checklevelforsarvari-ban tul sok gyuru!" );

		int kerekszam = 0;
		for( i = 0; i < MAXKEREK; i++ ) {
			if( kerektomb[i] ) {
				kerekszam++;
			}
		}
		if( kerekszam > Maxkerek_sarvari )
			error( "checklevelforsarvari-ban tul sok kerek!" );
	#endif
}














// Barmilyen error eseten 1-el ter vissza:
unsigned long getbelyeg( char* name ) {
	FILE* h = fopen( name, "rb" );
	if( !h )
		return 1;
	unsigned long l = 0;
	if( fread( &l, 1, 1, h ) != 1 )
		return 1;
	if( fread( &l, 1, 4, h ) != 4 )
		return 1;
	if( fread( &l, 1, 4, h ) != 4 )
		return 1;
	fclose( h );
	return l;
}

// Igazzal ter vissza, ha megvan name;
int getbelyeghezname( unsigned long belyeg, char* name ) {
	find_t ffblk;
	int done = _dos_findfirst( "*.lev", _A_NORMAL, &ffblk );
	while( !done ) {
		if( getbelyeg( ffblk.name ) == belyeg ) {
			// Megvan level file namee:
			strcpy( name, ffblk.name );
			return 1;
		}

		done = _dos_findnext( &ffblk );
	}
	return 0;
}

// Csak external file-hoz:
// Ha NULL, akkor valami baj volt vele:
char* getlevelname( char* name ) {
	static char levelnamebuffer[LEVELNAMEHOSSZ+4] = "";

	FILE* h = fopen( name, "rb" );
	if( !h )
		return NULL;

	if( fseek( h, 41, SEEK_CUR ) != 0 ) {
		fclose( h );
		return NULL;
	}

	if( fread( levelnamebuffer, 1, LEVELNAMEHOSSZ+1, h ) != LEVELNAMEHOSSZ+1 ) {
		fclose( h );
		return NULL;
	}
	levelnamebuffer[LEVELNAMEHOSSZ] = 0; // Ez csak biztonsagert

	fclose( h );
	return levelnamebuffer;
}

/*unsigned long double2belyeg( double* px ) {
	double y = *px;
	if( y < 0 )
		y = 0 - y;
	y *= 1000.0;
	if( y > double(0x6fffffff) )
		error( "double2belyeg-ben y > 0x6fffffff!" );

	unsigned long l = y;
	return l;
}

unsigned long double2belyeg( long l ) {
	double x = l;
	return double2belyeg( &x );
} */



