#include	"all.h"

#ifdef WATCOM
	static int Szakaszmaxszam = 5130;
#else
	static int Szakaszmaxszam = 1040;
#endif

// loadszakasz beleirja ptop->lezart-at ha topologia file-t olvas be:
// Ezt azutan afterplayext fuggveny hasznalja ki:
int Ucsoloadszakasz_lezart = 0;

/*szakaszok::szakaszok( void ) {
	tomb = NULL;
	maxszam = szam = 0;
	tertomb = NULL;
	xdim = 1;
	ydim = 1;
	cellameret = 1.0;
	origo = vector2( 0, 0 );
	nextnode = NULL;
	pfirstcsomag = NULL;
	csomagbanbetelt = 0;


	{ tomb = NEW(szakasz*) szakasz[Szakaszmaxszam]; }
	if( !tomb ) {
		uzenet( "memory szakaszoknak!" );
		return;
	}
	maxszam = Szakaszmaxszam;
	for( int i = 0; i < maxszam; i++ )
		memset( &tomb[i], 0, sizeof( szakasz ) );
} */

szakaszok::szakaszok( FILE* h, int targyas ) {
	// Elejen ugyanaz, mint void konstruktor:
	tomb = NULL;
	maxszam = szam = 0;
	tertomb = NULL;
	xdim = 1;
	ydim = 1;
	cellameret = 1.0;
	origo = vector2( 0, 0 );
	nextnode = NULL;
	pfirstcsomag = NULL;
	csomagbanbetelt = 0;

	#ifndef WATCOM
		// Fel lehet emelni 64000-re:
		if( long(Szakaszmaxszam) * sizeof( szakasz ) > 64000l )
			error( "szakaszok::szakaszok-ban long(Szakaszmaxszam) * sizeof( szakasz ) > 64000l!" );
	#endif

	{ tomb = NEW(szakasz*) szakasz[Szakaszmaxszam]; }
	if( !tomb ) {
		uzenet( "memory szakaszoknak!" );
		return;
	}
	maxszam = Szakaszmaxszam;
	for( int i = 0; i < maxszam; i++ )
		memset( &tomb[i], 0, sizeof( szakasz ) );

	char sor[200];
	// Verzio beolvasasa:
	if( fgets( sor, 190, h ) == NULL ) {
		error( "No tudott olvasni txt file-bol!" );
		return;
	}
	int verzio = 0;
	char verstring[200];
	if( sscanf( sor, "%s %d\n", verstring, &verzio ) != 2 ) {
		error( "No tudta feldolgozni sort!: ", sor );
		return;
	}
	if( verzio < 3 || verzio > 5 ) {
		error( "Verzio No stimmel!: ", sor );
		return;
	}
	if( strcmp( verstring, "Verzio:" ) != 0 ) {
		error( "No tudta beolvasni verziot!: ", sor );
		return;
	}

	// Elemszam beolvasasa:
	if( fgets( sor, 190, h ) == NULL ) {
		error( "No tudott olvasni txt file-bol!" );
		return;
	}
	if( sscanf( sor, "%d\n", &szam ) != 1 ) {
		error( "No tudta feldolgozni sort!: ", sor );
		return;
	}

	if( szam >= Szakaszmaxszam )
		error( "Nincs eleg hely szakaszoknak!" );

	for( i = 0; i < szam; i++ ) {
		if( fgets( sor, 190, h ) == NULL ) {
			error( "No tudott olvasni txt file-bol!" );
			return;
		}

		long lx = 0;
		long ly = 0;
		long ldx = 0;
		long ldy = 0;
		if( verzio <= 1 )
			error( "Verzio kisebb 2!" );
		int balfold = 1;
		if( verzio < 5 ) {
			if( sscanf( sor, "%d %ld %ld %ld %ld", &balfold,
					&lx, &ly, &ldx, &ldy ) != 5 ) {
				error( "No tudta feldolgozni sort!: ", sor );
				return;
			}
		}
		else {
			if( sscanf( sor, "%ld %ld %ld %ld",
					&lx, &ly, &ldx, &ldy ) != 4 ) {
				error( "No tudta feldolgozni sort!: ", sor );
				return;
			}
		}
		double szorzo = 1.0/100.0;
		if( verzio > 2 )
			szorzo = 1.0/100000.0;
		if( verzio <= 3 ) {
			tomb[i].vv.r.x = lx*szorzo;
			tomb[i].vv.r.y = ly*szorzo;
			tomb[i].vv.v.x = ldx*szorzo;
			tomb[i].vv.v.y = ldy*szorzo;
		}
		else {
			tomb[i].vv.r.x = lx*szorzo;
			tomb[i].vv.r.y = ly*szorzo;
			tomb[i].vv.v.x = ldx*szorzo - tomb[i].vv.r.x;
			tomb[i].vv.v.y = ldy*szorzo - tomb[i].vv.r.y;
		}
		if( !balfold ) {
			tomb[i].vv.r = tomb[i].vv.r + tomb[i].vv.v;
			tomb[i].vv.v = vector2null - tomb[i].vv.v;
		}

		// Targy beolvasasa:
		if( targyas ) {
			{ tomb[i].ptargy = NEW(targy*) targy; }
			loadtargy( h, tomb[i].ptargy );
		}
	}
}

szakaszok::szakaszok( topol* ptop, int targyas ) {
	// Elejen ugyanaz, mint void konstruktor:
	tomb = NULL;
	maxszam = szam = 0;
	tertomb = NULL;
	xdim = 1;
	ydim = 1;
	cellameret = 1.0;
	origo = vector2( 0, 0 );
	nextnode = NULL;
	pfirstcsomag = NULL;
	csomagbanbetelt = 0;

	{ tomb = NEW(szakasz*) szakasz[Szakaszmaxszam]; }
	if( !tomb ) {
		uzenet( "memory szakaszoknak!" );
		return;
	}
	maxszam = Szakaszmaxszam;
	for( int i = 0; i < maxszam; i++ )
		memset( &tomb[i], 0, sizeof( szakasz ) );

	if( targyas ) {
		for( i = 0; i < MAXKEREK; i++ ) {
			kerek* pker = ptop->kerektomb[i];
			if( !pker )
				continue;
			if( szam >= Szakaszmaxszam )
				error( "szakaszok::szakaszok-ban szam >= Szakaszmaxszam (ker)!" );
			tomb[szam].vv.r = pker->r;
			tomb[szam].vv.r.y = -tomb[szam].vv.r.y; // fejjel lefele
			tomb[szam].vv.v = vector2( 0.0, 0.0 );

			{ tomb[szam].ptargy = NEW(targy*) targy; }
			if( !tomb[szam].ptargy )
				error( "Nincs eleg memoria szakaszok::szakaszok-ban ptargynak!" );

			if( pker->tipus < T_CEL || pker->tipus > T_KEZDO )
				error( "szakaszok::szakaszok-ban pker->tipus < T_CEL || pker->tipus > T_KEZDO!" );

			tomb[szam].ptargy->mozgo = 0;
			tomb[szam].ptargy->szam = pker->tipus-T_CEL;
			tomb[szam].ptargy->sebesseg = 0.0;
			tomb[szam].ptargy->fazis = 0.0;
			tomb[szam].ptargy->odavissza = 0;
			tomb[szam].ptargy->aktiv = 0;

			szam++;
		}
	}
	else {
		// Normal szakaszok beolvasasa:
		for( i = 0; i < MAXGYURU; i++ ) {
			gyuru* pgy = ptop->ptomb[i];
			if( !pgy )
				continue;
			for( int j = 0; j < pgy->pontszam; j++ ) {
				if( szam >= Szakaszmaxszam )
					error( "szakaszok::szakaszok-ban z >= Szakaszmaxszam!" );

				vector2 r1, r2;
				if( j < pgy->pontszam - 1 ) {
					r1 = pgy->ponttomb[j];
					r2 = pgy->ponttomb[j+1];
				}
				else {
					r1 = pgy->ponttomb[j];
					r2 = pgy->ponttomb[0];
				}
				tomb[szam].vv.r = r1;
				tomb[szam].vv.v = r2-r1;

				tomb[szam].vv.r.y = -tomb[szam].vv.r.y; // fejjel lefele
				tomb[szam].vv.v.y = -tomb[szam].vv.v.y; // fejjel lefele

				szam++;
			}
		}
	}
}

szakaszok::~szakaszok( void ) {
	if( tomb ) {
		for( int i = 0; i < szam; i++ ) {
			if( tomb[i].ptargy )
				DELETE tomb[i].ptargy; }
		}
		DELETE tomb; }
	}
	if( tertomb ) {
		DELETE tertomb; }
	}
	deleteallnodes();
}

void szakaszok::save( FILE* h, int targyas ) {
	fprintf( h, "Verzio: %d\n", int(5) );
	fprintf( h, "%d\n", szam );
	for( int i = 0; i < szam; i++ ) {
		double szorzo = 100000.0;
		long lx1 = tomb[i].vv.r.x*szorzo;
		long ly1 = tomb[i].vv.r.y*szorzo;
		long lx2 = (tomb[i].vv.r.x+tomb[i].vv.v.x)*szorzo;
		long ly2 = (tomb[i].vv.r.y+tomb[i].vv.v.y)*szorzo;
		fprintf( h, "%ld %ld %ld %ld\n",
			lx1, ly1, lx2, ly2 );
		if( targyas ) {
			savetargy( h, tomb[i].ptargy );
		}
		else {
			if( tomb[i].ptargy )
				error( "szakaszok::save-ben !targyas, megis van benne targy!" );
		}
	}
}

/*void szakaszok::add( vector2 r, vector2 v, int targyas ) {
	if( szam >= maxszam ) {
		error( "Nincs eleg hely uj szakasz hozzaadasahoz!" );
		return;
	}
	tomb[szam].vv.r = r;
	tomb[szam].vv.v = v;
	if( targyas ) {
		{ tomb[szam].ptargy = NEW(targy*) targy; }
		tomb[szam].ptargy->mozgo = 0;
		tomb[szam].ptargy->szam = 0;
		tomb[szam].ptargy->sebesseg = 0.0;
		tomb[szam].ptargy->fazis = 0.0;
		tomb[szam].ptargy->odavissza = 1;
	}
	else
		tomb[szam].ptargy = NULL;
	szam++;
} */

/*void szakaszok::remove( szakasz* psz ) {
	for( int i = 0; i < szam; i++ ) {
		if( &tomb[i] == psz ) {
			if( tomb[i].ptargy )
				DELETE tomb[i].ptargy; }
			for( int j = i; j < szam-1; j++ )
				tomb[j] = tomb[j+1];
			szam--;
			return;
		}
	}
	error( "szakaszok::remove-ban No talalja torlendo szakaszt!" );
} */

// NODE-kezeles:

/*#define CSOMAGBANNODESZAM (500)
struct soknodecsomag {
	LINEnode nodetomb[CSOMAGBANNODESZAM];
	soknodecsomag* nextcsomag;
};
soknodecsomag* pfirstcsomag;
int csomagbanbetelt;
*/

LINEnode* szakaszok::getnewnode( void ) {
	if( !pfirstcsomag ) {
		{ pfirstcsomag = NEW(soknodecsomag*) soknodecsomag; }
		if( !pfirstcsomag )
			uzenet( "memory!" );
		pfirstcsomag->nextcsomag = NULL;
		csomagbanbetelt = 0;
	}
	soknodecsomag* curcsomag = pfirstcsomag;
	while( curcsomag->nextcsomag )
		curcsomag = curcsomag->nextcsomag;
	if( csomagbanbetelt == CSOMAGBANNODESZAM ) {
		{ curcsomag->nextcsomag = NEW(soknodecsomag*) soknodecsomag; }
		curcsomag = curcsomag->nextcsomag;
		if( !curcsomag )
			uzenet( "memory!" );
		curcsomag->nextcsomag = NULL;
		csomagbanbetelt = 0;
	}
	csomagbanbetelt++;
	return &curcsomag->nodetomb[csomagbanbetelt-1];
}

void szakaszok::deleteallnodes( void ) {
	soknodecsomag* curcsomag = pfirstcsomag;
	pfirstcsomag = NULL;
	while( curcsomag ) {
		soknodecsomag* elozocsomag = curcsomag;
		curcsomag = curcsomag->nextcsomag;
		DELETE elozocsomag; }
	}
}



// Egy cellaba bekoti LINEat, ha meg nincs benne es rajta van tablan:
void szakaszok::bekot( int cellax, int cellay, LINE* pv ) {
	#ifdef TEST
		if( cellax < 0 || cellay < 0 )
			error( "cellax < 0 || cellay < 0!" );
	#endif
	if( cellax >= xdim || cellay >= ydim )
		return;
	LINEnode* pujnode = getnewnode();
	pujnode->pnext = NULL;
	pujnode->pLINE = pv;
	LINEnode* pn = tertomb[xdim*cellay+cellax];
	if( !pn ) {
		tertomb[xdim*cellay+cellax] = pujnode;
		return;
	}
	while( pn->pnext ) {
		pn = pn->pnext;
	}
	// Most pn az utolso node-ra mutat!
	pn->pnext = pujnode;
}

// Egy LINEat hozzarendel osszes olyan cellahoz, amihez koze lehet:
void szakaszok::beindexelLINEat( LINE* pv, double maxtav ) {
	// Kitolti LINE egyseg es hossz rubrikait:
	pv->hossz = abs( pv->v );
	if( pv->hossz < 0.00000001 )
		error( "LINE hossza tul kicsi!" );
	pv->egyseg = egys( pv->v );

	vector2 v = pv->v;
	vector2 r = pv->r - origo;

	v = v*(1/cellameret);
	r = r*(1/cellameret);
	maxtav *= 1.5/cellameret;
	// Ezentul cellameretet lehet 1.0-nak venni!
	int invertalt = 0;
	if( fabs( v.y ) > fabs( v.x ) ) {
		invertalt = 1;
		double tmpd = v.x;
		v.x = v.y;
		v.y = tmpd;
		tmpd = r.x;
		r.x = r.y;
		r.y = tmpd;
	}
	// Most x iranyban biztosan hosszabb vagy egyenlo y iranynal!
	if( v.x < 0 ) {
		// Vegpont csere:
		r = r+v;
		v = vector2null-v;
	}
	// Most v x-pozitiv iranyba mutat:
	// Kiszamoljuk y = m*x + y0 fuggveny parametereit:
	double m = v.y/v.x;
	double y0 = r.y-m*r.x;
	double xkezdo = r.x-maxtav;
	int cellax = 0;
	if( xkezdo > 0 )
		cellax = xkezdo;
	if( r.x+v.x+maxtav < 0 )
		error( "szakaszok::beindexelLINEat-ban r.x+v.x+maxtav < 0!" );
	int ucsocellax = r.x+v.x+maxtav;
	while( cellax <= ucsocellax ) {
		double y1 = m*cellax + y0;
		double y2 = m*(cellax+1) + y0;
		// y1-nek kissebnek kell lennie y2-nel:
		if( y1 > y2 ) {
			double tmpd = y1;
			y1 = y2;
			y2 = tmpd;
		}
		y1 -= maxtav;
		y2 += maxtav;
		#ifdef TEST
			if( y1 > y2 )
				error( "Bug!" );
		#endif
		if( y2 < 0.0 )
			error( "Bug!" );
		int cellay = 0;
		if( y1 > 0 )
			cellay = y1;
		int ucsocellay = y2;
		while( cellay <= ucsocellay ) {
			if( invertalt )
				bekot( cellay, cellax, pv );
			else
				bekot( cellax, cellay, pv );

			cellay++;
		}

		cellax++;
	}
}

void szakaszok::rendez( double maxtav ) {
	// Ellenorzes:
	if( szam <= 0 )
		error( "szakaszok::rendez-ben szam <= 0!" );
	if( tomb[0].ptargy )
		error( "szakaszok::rendez-ben tomb[0].ptargy igaz!" );
	if( tertomb )
		error( "szakaszok::rendez-ban nodetomb != NULL!" );

	#ifdef WATCOM
		cellameret = 1.0;
	#else
		cellameret = 2.0;
	#endif

	// Megkeresi palya kiterjedeset:
	ffirstrolasresetszak();
	szakasz* psz = getnextszak();
	if( !psz )
		error( "Nincs egyetlen szakasz sem definialva!" );
	double minx = psz->vv.r.x;
	double maxx = psz->vv.r.x;
	double miny = psz->vv.r.y;
	double maxy = psz->vv.r.y;
	while( psz ) {
		if( psz->vv.r.x < minx )
			minx = psz->vv.r.x;
		if( psz->vv.r.x > maxx )
			maxx = psz->vv.r.x;
		if( psz->vv.r.y < miny )
			miny = psz->vv.r.y;
		if( psz->vv.r.y > maxy )
			maxy = psz->vv.r.y;

		if( psz->vv.r.x+psz->vv.v.x < minx )
			minx = psz->vv.r.x+psz->vv.v.x;
		if( psz->vv.r.x+psz->vv.v.x > maxx )
			maxx = psz->vv.r.x+psz->vv.v.x;
		if( psz->vv.r.y+psz->vv.v.y < miny )
			miny = psz->vv.r.y+psz->vv.v.y;
		if( psz->vv.r.y+psz->vv.v.y > maxy )
			maxy = psz->vv.r.y+psz->vv.v.y;

		psz = getnextszak();
	}

	// Beallitja kezdopontot:
	origo = vector2( minx, miny );
	double xsized = maxx-minx;
	double ysized = maxy-miny;

	// Kiszamolja dimenziot:
	xdim = xsized/cellameret+1;
	ydim = ysized/cellameret+1;
	if( xdim < 0 || ydim < 0 )
		error( "xdim < 0 || ydim < 0!" );
	#ifndef WATCOM
		// DOS es DOSX286:
		if( xdim > 100 || ydim > 100 )
			error( "xdim > 100 || ydim > 100!" );
	#else
		// WATCOM:
		if( xdim > 200 || ydim > 200 )
			error( "xdim > 200 || ydim > 200!" );
	#endif

	// Lefoglalja tombot:
	unsigned tombmeret = xdim * ydim;
	{ tertomb = NEW(pLINEnode*) pLINEnode[tombmeret]; }
	if( !tertomb )
		uzenet( "memory!" );
	int hatar = xdim*ydim;
	for( int i = 0; i < hatar; i++ )
		tertomb[i] = NULL;

	// Most vegigmegy szakaszokon es beilleszti oket:
	ffirstrolasresetszak();
	psz = getnextszak();
	while( psz ) {
		beindexelLINEat( &psz->vv, maxtav );
		psz = getnextszak();
	}
}

void szakaszok::ffirstrolasreset( vector2 r ) {
	if( !tertomb )
		error( "szakaszok::ffirstrolasreset-ban !tertomb!" );
	r = (r-origo)*(1/cellameret);
	int cellax = 0;
	if( r.x > 0 )
		cellax = r.x;
	int cellay = 0;
	if( r.y > 0 )
		cellay = r.y;

	if( cellax > xdim )
		error( "szakaszok::ffirstrolasreset-ben cellax > xdim!" );
	if( cellax == xdim )
		cellax = xdim-1;
	if( cellay > ydim )
		error( "szakaszok::ffirstrolasreset-ben cellay > ydim!" );
	if( cellay == ydim )
		cellay = ydim-1;
	nextnode = tertomb[xdim*cellay+cellax];
}

LINE* szakaszok::getnext( void ) {
	if( !nextnode )
		return NULL;
	#ifdef TEST
		if( !nextnode->pLINE )
			error( "szakaszok::getnext-ben !nextnode->pLINE" );
	#endif
	LINE* pret = nextnode->pLINE;
	nextnode = nextnode->pnext;
	return pret;
}

// Szakaszos kereso:
void szakaszok::ffirstrolasresetszak( void ) {
	szakfuto = 0;
}

szakasz* szakaszok::getnextszak( void ) {
	if( szakfuto >= szam )
		return NULL;
	szakfuto++;
	return &tomb[szakfuto-1];
}


// Visszaadja kajak szamat!:
int szakaszok::setallaktiv( int targytipusszam, targytomb* ptt ) {
	int kajaszam = 0;
	int voltkezdo = 0;
	for( int i = 0; i < szam; i++ ) {
		if( !tomb[i].ptargy )
			error( "szakaszok::setallaktiv-ban No ptargy!" );
		tomb[i].ptargy->aktiv = 1;
		if( tomb[i].ptargy->szam >= targytipusszam )
			error( "tomb[i].ptargy->szam >= targytipusszam!" );
		if( ptt->targytipusok[tomb[i].ptargy->szam].tipus == T_KAJA )
			kajaszam++;
		if( ptt->targytipusok[tomb[i].ptargy->szam].tipus == T_KEZDO ) {
			if( voltkezdo )
				error( "Palyan ket kezdo is van!" );
			voltkezdo = 1;
			tomb[i].ptargy->aktiv = 0;
			vector2 diff = tomb[i].vv.r - Kor2.r;
			Kor1.r = Kor1.r + diff;
			Kor2.r = Kor2.r + diff;
			Kor4.r = Kor4.r + diff;
		}
	}
	if( !voltkezdo )
		error( "Palyan nincs kezdo!" );
	return kajaszam;
}

// Killereket veszi legelore, utan foodokat, majd vegul exit:
void szakaszok::killerekelore( targytomb* ptt ) {
	if( szam < 2 )
		error( "szakaszok::killerekelore-ben szam < 2!" );
	for( int j = 0; j < szam+4; j++ ) {
		for( int i = 0; i < szam-1; i++ ) {
			if( !tomb[i].ptargy )
				error( "szakaszok::setallaktiv-ban No ptargy!" );
			int tipus1 = ptt->targytipusok[tomb[i].ptargy->szam].tipus;

			if( !tomb[i+1].ptargy )
				error( "szakaszok::setallaktiv-ban No ptargy!" );
			int tipus2 = ptt->targytipusok[tomb[i+1].ptargy->szam].tipus;

			// Sullyozzuk tipusokat hogy sorrendiseget lehessen megallapitani:
			int tip1 = 10;
			if( tipus1 == T_HALALOS )
				tip1 = 1;
			if( tipus1 == T_KAJA )
				tip1 = 2;
			if( tipus1 == T_CEL )
				tip1 = 3;

			int tip2 = 10;
			if( tipus2 == T_HALALOS )
				tip2 = 1;
			if( tipus2 == T_KAJA )
				tip2 = 2;
			if( tipus2 == T_CEL )
				tip2 = 3;

			if( tip1 > tip2 ) {
				// Cserelnunk kell:
				szakasz tmp = tomb[i];
				tomb[i] = tomb[i+1];
				tomb[i+1] = tmp;
			}
		}
	}
}

szakasz* szakaszok::getptrszakasz( int sorszam ) {
	if( sorszam >= szam )
		error( "szakaszok::getptrszakasz-ban sorszam >= szam!" );
	return &tomb[sorszam];
}

int szakaszok::getsorszam( szakasz* pszakasz ) {
	for( int i = 0; i < szam; i++ ) {
		if( !tomb[i].ptargy )
			error( "szakaszok::getsorszam-ban No ptargy!" );
		if( &tomb[i] == pszakasz )
			return i;
	}
	error( "getsorszam-ban nincs meg szakasz!" );
	return -1;
}

void loadszakaszok( char* name, int kulso, szakaszok** ppszak,
						szakaszok** ppszakt ) {
	if( *ppszak || *ppszakt )
		error( "loadszakaszok-ban *ppszak || *ppszakt!" );

	FILE* h = NULL;
	if( kulso )
		h = fopen( name, "rt" );
	else
		h = qopen( name, "rt" );
	if( !h )
		error( "No tudta megnyitni file-t!: ", name );

	// Eldontese hogy regi vagy uj tipusu file-e:
	int ujtipus = 0;
	char tmp[10] = "AAAAA";
	if( fread( tmp, 1, 4, h ) != 4 )
		error( "No tudott olvasni txt file-bol!" );
	if( strncmp( tmp, "POT", 3 ) == 0 )
		ujtipus = 1;

	if( kulso )
		fclose( h );
	else
		qclose( h );

	// Beolvasas:
	if( ujtipus ) {
		topol* ptop = new topol( name, kulso );
		if( ptop->errors )
			uzenet( "This level file has some topology errors.",
					"If you can load it to the editor you may be able to correct these errors.",
					name );

		// Most megnezzuk hogy nincs-e tul sok pontja palyanak:
		if( kulso )
			ptop->checklevelforsarvari();

		{ *ppszak = NEW(szakaszok*) szakaszok( ptop, 0 ); }
		{ *ppszakt = NEW(szakaszok*) szakaszok( ptop, 1 ); }
		Ucsoloadszakasz_lezart = ptop->lezart;
		delete ptop;
	}
	else {
		if( kulso )
			error( "iopvwoiuvfriuo" );
		h = qopen( name, "rt" );
		{ *ppszak = NEW(szakaszok*) szakaszok( h, 0 ); }
		{ *ppszakt = NEW(szakaszok*) szakaszok( h, 1 ); }
		qclose( h );
	}
}

void saveszakaszok( char* name, szakaszok* pszak, szakaszok* pszakt ) {
	char tmp[30] = "..\\files\\";
	strcat( tmp, name );
	FILE* h = fopen( tmp, "wt" );
	if( !h )
		error( "No tudta megnyitni file-t!: ", name );
	pszak->save( h, 0 );
	pszakt->save( h, 1 );
	fclose( h );
}


