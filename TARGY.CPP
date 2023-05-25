#include	"all.h"

static double Szorzo = 1000.0;

void loadtargy( FILE* h, targy* ptargy ) {
	if( !ptargy )
		error( "loadtargyban!" );
	char sor[200];
	// mozgo:
	if( fgets( sor, 190, h ) == NULL )
		error( "No tudott olvasni targyat!" );
	if( sscanf( sor, "%d", &ptargy->mozgo ) != 1 )
		error( "Targyban No tudta feldolgozni sort!: ", sor );
	// szam:
	if( fgets( sor, 190, h ) == NULL )
		error( "No tudott olvasni targyat!" );
	if( sscanf( sor, "%d", &ptargy->szam ) != 1 )
		error( "Targyban No tudta feldolgozni sort!: ", sor );
	// sebesseg:
	if( fgets( sor, 190, h ) == NULL )
		error( "No tudott olvasni targyat!" );
	long sebesseg = -1;
	if( sscanf( sor, "%ld", &sebesseg ) != 1 )
		error( "Targyban No tudta feldolgozni sort!: ", sor );
	ptargy->sebesseg = sebesseg/Szorzo;
	// fazis:
	if( fgets( sor, 190, h ) == NULL )
		error( "No tudott olvasni targyat!" );
	long fazis = -1;
	if( sscanf( sor, "%ld", &fazis ) != 1 )
		error( "Targyban No tudta feldolgozni sort!: ", sor );
	ptargy->fazis = fazis/Szorzo;
	// odavissza:
	if( fgets( sor, 190, h ) == NULL )
		error( "No tudott olvasni targyat!" );
	if( sscanf( sor, "%d", &ptargy->odavissza ) != 1 )
		error( "Targyban No tudta feldolgozni sort!: ", sor );
}

void savetargy( FILE* h, targy* ptargy ) {
	if( !ptargy )
		error( "savetargyban!" );
	fprintf( h, "%d\n", ptargy->mozgo );
	fprintf( h, "%d\n", ptargy->szam );
	long sebesseg = ptargy->sebesseg*Szorzo;
	fprintf( h, "%ld\n", sebesseg );
	long fazis = ptargy->fazis*Szorzo;
	fprintf( h, "%ld\n", fazis );
	fprintf( h, "%d\n", ptargy->odavissza );
}

short loadint( FILE* h ) {
	int ertek = -1;
	char sor[200];
	if( fgets( sor, 190, h ) == NULL )
		error( "No tudott olvasni sort filebol!" );
	if( sscanf( sor, "%d", &ertek ) != 1 )
		error( "loadint-ben No tudta feldolgozni sort!: ", sor );
	if( ertek < 0 )
		error( "loadint-ben ertek < 0!: ", sor );
	return short(ertek);
}

void loadstring( FILE* h, char* text, int maxhossz ) {
	char sor[200];
	if( fgets( sor, 190, h ) == NULL )
		error( "No tudott olvasni sort filebol!" );
	char tmp[100];
	if( sscanf( sor, "%s", tmp ) != 1 )
		error( "loadstring-ben No tudta feldolgozni sort!: ", sor );
	if( strlen( tmp ) > maxhossz )
		error( "loadstring-ben strlen( tmp ) > maxhossz!: ", tmp );
	strcpy( text, tmp );
}

static int loadtargytipus( targytipus* ptipus, FILE* h ) {
	int sorszam = loadint( h );
	ptipus->tipus = loadint( h );
	ptipus->sugar = loadint( h ) * 0.01;
	ptipus->kerekerz = loadint( h );
	ptipus->fejerz = loadint( h );
	ptipus->frameseb = loadint( h );
	ptipus->pottyindex = loadint( h );
	ptipus->frameszam = loadint( h );
	if( ptipus->frameszam > MAXFRAMESZAM )
		error( "ptipus->frameszam > MAXFRAMESZAM" );
	if( ptipus->frameszam < 1 )
		error( "ptipus->frameszam < 1" );

	// A vegleges kepek meretei:
	int kellx = ptipus->sugar*2*Aranyx;
	int kelly = ptipus->sugar*2*Aranyy;

	for( int i = 0; i < ptipus->frameszam; i++ ) {
		char sor[200];
		if( fgets( sor, 190, h ) == NULL )
			error( "No tudott olvasni frame nameet filebol!" );
		char name[200];
		if( sscanf( sor, "%s", name ) != 1 )
			error( "targytomb konstban No tudta feldolgozni sort!: ", sor );
		pic8* ptmp = NULL;
		{ ptmp = NEW(pic8*) pic8( name ); }
		unsigned char lyukindex = ptmp->gpixel( 0, 0 );
		{ ptipus->spriteok[i] = NEW(pic8*) pic8( kellx, kelly ); }
		mintavetel8( ptipus->spriteok[i], ptmp );
		DELETE ptmp; }
		// Fejjel lefele forditjuk sprite-ot:
		pic8* ppic = ptipus->spriteok[i];
		int xsize = ppic->getxsize();
		int ysize = ppic->getysize();
		if( i == 0 ) {
			ptipus->dx = xsize*0.5;
			ptipus->dy = ysize*0.5;
		}
		for( int y = 0; y < (ysize>>1); y++ ) {
			for( int x = 0; x < xsize; x++ ) {
				unsigned char szin1 = ppic->gpixel( x, y );
				unsigned char szin2 = ppic->gpixel( x, ysize-1-y );
				ppic->ppixel( x, ysize-1-y, szin1 );
				ppic->ppixel( x, y, szin2 );
			}
		}
		// Kiterjesztes megkeresese:
		int j = strlen( name ) - 1;
		while( j > 0 ) {
			if( name[j] == '.' ) {
				if( strcmpi( name+j, ".pcx" ) == 0 ) {
					// Ki kell lyukasztani sprite-ta!:
					spriteosit( ptipus->spriteok[i], lyukindex );
				}
				break;
			}
			j--;
		}
	}
	return sorszam;
}

static void felszabtargytipus( targytipus* ptipus ) {
	for( int i = 0; i < MAXFRAMESZAM; i++ ) {
		if( ptipus->spriteok[i] )
			DELETE ptipus->spriteok[i]; }
	}
}


targytomb::targytomb( char* name ) {
	{ targytipusok = NEW(targytipus*) targytipus[MAXTARGYTIPUSSZAM]; }
	if( !targytipusok )
		uzenet( "memory" );

	// Nullazas:
	for( int i = 0; i < MAXTARGYTIPUSSZAM; i++ )
		memset( &targytipusok[i], 0, sizeof( targytipus ) );

	FILE* h = qopen( name, "rt" );
	if( !h )
		error( "No tudta megnyitni file-t!: ", name );

	// Beolvassa targytipusok szamat:
	char sor[200];
	if( fgets( sor, 190, h ) == NULL )
		error( "No tudott olvasni targytipusszamot!" );
	if( sscanf( sor, "%d", &targytipusszam ) != 1 )
		error( "targytomb konstban No tudta feldolgozni sort!: ", sor );

	if( targytipusszam < 0 || targytipusszam > MAXTARGYTIPUSSZAM )
		error( "targytipusszam > MAXTARGYTIPUSSZAM!" );

	// Most jonnek sorra targytipusok:
	for( i = 0; i < targytipusszam; i++ ) {
		if( loadtargytipus( &targytipusok[i], h ) != i )
			error( "targytipus sorszama No stimmel!" );
	}

	qclose( h );
}

targytomb::~targytomb( void ) {
	for( int i = 0; i < targytipusszam; i++ )
		felszabtargytipus( &targytipusok[i] );
	DELETE targytipusok; }
}


