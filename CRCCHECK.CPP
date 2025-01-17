#include	"all.h"

static int Beallitas = 0;
static unsigned long Crcszam = 0x05b839e45l;

static int Crcerror = 0; // 1 eseten across.exe, 2 eseten readme.txt valtozott,
						// 3 eseten kommand line No across-sal fejezodik
static int Crcellenorizve = 0;

enum { FILE_IS_MISSING = 10, FILE_HAS_CHANGED, FILE_NAME_CHANGED, ACROSS_EXE, README_TXT };

void check_kommand( const char* komline ) {
	if( Beallitas )
		return;
	const char* namemutato = &komline[strlen( komline ) - 10];

	/*FILE* h = fopen( "name.txt", "wt" );
	if( !h )
		error( "uigruovvruvrvriruvr" );
	fprintf( h, "name: <%s>", namemutato );
	fclose( h ); */

	char tmp[12];
	tmp[6] = '.';
	tmp[7] = 'e';
	tmp[3] = 'o';
	tmp[8] = 'x';
	tmp[0] = 'a';
	tmp[2] = 'r';
	tmp[10] = 0;
	tmp[5] = 's';
	tmp[1] = 'c';
	tmp[9] = 'e';
	tmp[4] = 's';

	if( strcmpi( namemutato, tmp ) == 0 ) {
		for( int i = 0; i < 11; i++ )
			tmp[i] = ' ';
		return;
	}
	for( int i = 0; i < 11; i++ )
		tmp[i] = ' ';
	// No stimmel kommand line name:
	Crcerror = 3;
	Crcellenorizve = 1;
}

static char* titstring( int azonosito, int secondbuff = 0 ) {
	static char titbuffer1[100];
	static char titbuffer2[100];
	static int titbufferi[100];
	titbufferi[99] = 0;
	switch( azonosito ) {
		case FILE_IS_MISSING:
			titbufferi[0] = 'F';
			titbufferi[1] = 'i';
			titbufferi[2] = 'l';
			titbufferi[3] = 'e';
			titbufferi[4] = ' ';
			titbufferi[5] = 'i';
			titbufferi[6] = 's';
			titbufferi[7] = ' ';
			titbufferi[8] = 'm';
			titbufferi[9] = 'i';
			titbufferi[10] = 's';
			titbufferi[11] = 's';
			titbufferi[12] = 'i';
			titbufferi[13] = 'n';
			titbufferi[14] = 'g';
			titbufferi[15] = '!';
			titbufferi[16] = ':';
			titbufferi[17] = ' ';
			titbufferi[18] = 0;
			break;
		case FILE_HAS_CHANGED:
			titbufferi[0] = 'F';
			titbufferi[1] = 'i';
			titbufferi[2] = 'l';
			titbufferi[3] = 'e';
			titbufferi[4] = ' ';
			titbufferi[5] = 'h';
			titbufferi[6] = 'a';
			titbufferi[7] = 's';
			titbufferi[8] = ' ';
			titbufferi[9] = 'c';
			titbufferi[10] = 'h';
			titbufferi[11] = 'a';
			titbufferi[12] = 'n';
			titbufferi[13] = 'g';
			titbufferi[14] = 'e';
			titbufferi[15] = 'd';
			titbufferi[16] = '!';
			titbufferi[17] = ':';
			titbufferi[18] = ' ';
			titbufferi[19] = 0;
			break;
		case FILE_NAME_CHANGED:
			titbufferi[0] = 'F';
			titbufferi[1] = 'i';
			titbufferi[2] = 'l';
			titbufferi[3] = 'e';
			titbufferi[4] = ' ';
			titbufferi[5] = 'n';
			titbufferi[6] = 'a';
			titbufferi[7] = 'm';
			titbufferi[8] = 'e';
			titbufferi[9] = ' ';
			titbufferi[10] = 'c';
			titbufferi[11] = 'h';
			titbufferi[12] = 'a';
			titbufferi[13] = 'n';
			titbufferi[14] = 'g';
			titbufferi[15] = 'e';
			titbufferi[16] = 'd';
			titbufferi[17] = '!';
			titbufferi[18] = ':';
			titbufferi[19] = ' ';
			titbufferi[20] = 0;
			break;
		case ACROSS_EXE:
			titbufferi[0] = 'a';
			titbufferi[1] = 'c';
			titbufferi[2] = 'r';
			titbufferi[3] = 'o';
			titbufferi[4] = 's';
			titbufferi[5] = 's';
			titbufferi[6] = '.';
			titbufferi[7] = 'e';
			titbufferi[8] = 'x';
			titbufferi[9] = 'e';
			titbufferi[10] = 0;
			break;
		case README_TXT:
			titbufferi[0] = 'r';
			titbufferi[1] = 'e';
			titbufferi[2] = 'a';
			titbufferi[3] = 'd';
			titbufferi[4] = 'm';
			titbufferi[5] = 'e';
			titbufferi[6] = '.';
			titbufferi[7] = 't';
			titbufferi[8] = 'x';
			titbufferi[9] = 't';
			titbufferi[10] = 0;
			break;
		default:
			error( "titstring-ben azonosito default!" );
	}
	for( int i = 0; i < 100; i++ ) {
		if( secondbuff ) {
			titbuffer1[i] = (char)titbufferi[i];
			if( !titbuffer1[i] )
				return titbuffer1;
		}
		else {
			titbuffer2[i] = (char)titbufferi[i];
			if( !titbuffer2[i] )
				return titbuffer2;
		}
	}
	error( "Bug475876!" );
	return titbuffer1;
}

// Valodi file-t nez:
static unsigned long getcrc( char* name_p, long ellenorzohossz ) {
	char name[40];
	strcpy( name, name_p );
	FILE* h = fopen( name, "rb" );
	if( !h )
		return 0;
	long hossz = filelength( fileno( h ) );
	if( hossz != ellenorzohossz && !Beallitas ) {
		//uzenet( "File size has changed!: ", name );
		Crcerror = 1;
		return 0;
	}

	long number = (hossz-3)/4;

	unsigned long sum = Crcszam;
	for( long l = 0; l < number; l++ ) {
		unsigned long ujszam;
		if( fread( &ujszam, 1, 4, h ) != 4 )
			return 0;
		sum ^= ujszam ^ Crcszam;
		if( sum > 0x7fffffffl )
			sum = (sum << 1) + 1;
		else
			sum = (sum << 1);
	}

	fclose( h );

	if( Beallitas ) {
		FILE* h2 = fopen( "biztlist.txt", "at" );
		if( !h2 )
			error( "!h2!" );
		fprintf( h2, "%ld\n%lu\n", hossz, sum );
		fclose( h2 );
	}

	return sum;
}

static unsigned long loadulong( FILE* h ) {
	unsigned long ertek = 0xfffffffful;
	char sor[200];
	if( fgets( sor, 190, h ) == NULL )
		error( "No tudott olvasni sort filebol!" );
	if( sscanf( sor, "%lu", &ertek ) != 1 )
		error( "loadulong-ban No tudta feldolgozni sort!: ", sor );
	if( ertek == 0xfffffffful )
		error( "loadulong-ban ertek == 0xfffffffful!: ", sor );
	return ertek;
}

void crccheck2( void ) {
     if( Crcerror ) {
		if( Crcerror == 1 )
			uzenet( titstring( FILE_HAS_CHANGED ), titstring( ACROSS_EXE, 1 ) );
		if( Crcerror == 2 )
			uzenet( titstring( FILE_HAS_CHANGED ), titstring( README_TXT, 1 ) );
		if( Crcerror == 3 )
			uzenet( titstring( FILE_NAME_CHANGED ), titstring( ACROSS_EXE, 1 ) );
	}
}

void crccheck( void ) {
	// Csak neha megy bele fuggvenybe:
	if( !Beallitas ) {
		unsigned short egesz, tort;
		getbiostort( &egesz, &tort );
		if( tort%120 )
			return;
	}

	if( Crcellenorizve )
		return;
	Crcellenorizve = 1;

	long hossz_across_exe = 0, hossz_readme_txt = 0;
	unsigned long crc_across_exe = 0, crc_readme_txt = 0;
	if( !Beallitas ) {
		#ifdef SARVARI
			FILE* h = qopen( "bizton_s.txt", "rt" );
		#else
			FILE* h = qopen( "bizton_r.txt", "rt" );
		#endif
		if( !h )
			error( "No tudta megnyitni bizton_?.txt file-t!" );
		hossz_across_exe = loadulong( h );
		crc_across_exe = loadulong( h );

		hossz_readme_txt = loadulong( h );
		crc_readme_txt = loadulong( h );

		qclose( h );
	}

	if( getcrc( titstring( ACROSS_EXE ), hossz_across_exe ) != crc_across_exe &&
							!Beallitas ) {
		//uzenet( "File has changed!: ", "across.exe" );
		Crcerror = 1;
		return;
	}
	if( Crcerror ) {
		Crcerror = 1;
		return;
	}
	if( getcrc( titstring( README_TXT ), hossz_readme_txt ) != crc_readme_txt &&
							!Beallitas ) {
		//uzenet( "File has changed!: ", "readme.txt" );
		Crcerror = 2;
		return;
	}
	if( Crcerror ) {
		Crcerror = 2;
		return;
	}

	if( Beallitas )
		error( "Kiirta kodokat!" );
}


