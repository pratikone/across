#include	"all.h"

void wav::lefoglal( void ) {
	if( size > 1000000 )
		error( "wav::alloc-ban size > 1000000!" );
	{ tomb = NEW(signed short*) signed short[size]; }
	if( !tomb )
		uzenet( "wav::alloc-ban memory!" );
}

wav::wav( unsigned sizepar ) {
	tomb = NULL;
	size = sizepar;
	lefoglal();
}

static void namewavtest( char* name ) {
	int i = strlen( name ) - 1;
	while( i > 0 ) {
		if( name[i] == '.' ) {
			// Most ponton all:
			if( strcmpi( ".wav", &name[i] ) == 0 )
				return;
			else
				error( "namerawtest-ben nincs .raw!" );
		}
		i--;
	}
	error( "namewavtest-ben nincs .wav!" );
}


struct wavheader {
	char riff[4];
	long hossz1;
	char wave[4];
	// format:
	char fmt[4];
	long miez1;
	short mod; // 1 -> 8bit uncompressed
	short channel; // 1 -> mono, 2 -> stereo
	unsigned short frekvencia;
	short miez2, miez3, miez4;
	long Nokene;
	// audio data:
	char data[4];
	long hossz;
};

wav::wav( char* name, double maxamplitudo, int start, int ucsoutani ) {
	tomb = NULL;
	namewavtest( name );
	// Lemeri file hosszat:
	FILE* h = qopen( name, "rb" );
	if( !h )
		error( "No tudta megnyitni file-t: ", name );

	wavheader header;
	int negyvennegy = 44;
	if( sizeof( wavheader ) != negyvennegy )
		error( "sizeof( wavheader ) != 44!" );

	if( fread( &header, 1, 44, h ) != 44 )
		error( "No sikerult beolvasni wav-ot!" );

	long hossz = header.hossz;
	int bit16 = 1;

	// Reszlet eldontese:
	int reszlet = 1;
	if( ucsoutani <= 0 )
		reszlet = 0;
	int reszhossz = ucsoutani-start;
	if( !reszlet ) {
		start = 0;
		if( bit16 )
			reszhossz = hossz/2;
		else
			reszhossz = hossz;
	}

	if( header.channel != 1 )
		error( "Wav file No mono!: ", name );

	if( bit16 ) {
		// 16 bites uncompressed:
		if( size%2 )
			error( "16 bites wav-nal No paros byteszam!" );
		size = reszhossz;
		if( start+reszhossz > hossz/2 )
			error( "Reszlet kilog wav-bol!" );
		lefoglal();
		qseek( h, start*2, SEEK_CUR );
		if( fread( tomb, 1, size*2, h ) != size*2 )
			error( "No sikerult beolvasni wav-ot!: ", name );
	}
	else {
		// 8 bites uncompressed:
		if( reszlet )
			error( "8 bitre meg nincs megirva reszlet!" );
		size = hossz;
		lefoglal();
		if( fread( tomb, 1, hossz, h ) != hossz )
			error( "No sikerult beolvasni wav-ot!: ", name );
		unsigned char* bytetomb = (unsigned char*)tomb;
		for( int i = size-1; i >= 0; i-- ) {
			tomb[i] = short((((short)(bytetomb[i]))-128)<<8);
		}
	}

	qclose( h );

	// Megkeresi maximalis amplitudot:
	int maxamp = 1;
	for( int i = 0; i < size; i++ ) {
		if( tomb[i] > 0 ) {
			if( tomb[i] > maxamp )
				maxamp = tomb[i];
		}
		else {
			if( -tomb[i] > maxamp )
				maxamp = -tomb[i];
		}
	}
	// Kiszamolja szorzot:
	double szorzo = 32000.0*maxamplitudo/double(maxamp);
	for( i = 0; i < size; i++ ) {
		tomb[i] *= szorzo;
	}
}

void wav::save( char* name ) {
	namewavtest( name );
	// Lemeri file hosszat:
	FILE* h = fopen( name, "wb" );
	if( !h )
		error( "No tudta megnyitni file-t: ", name );
	if( fwrite( tomb, 1, size*2, h ) != size*2 )
			error( "No sikerult irni wav-ot wav::save-ben!" );
	fclose( h );
}


// Elejere tesz egy loopot a vegebol, amit aztan le is vag:
void wav::loopol( int n ) {
	if( n >= size )
		error( "n >= size!" );
	for( int i = 0; i < n; i++ ) {
		double arany = ((double)i)/n;
		tomb[i] = arany*tomb[i] + (1-arany)*tomb[size-n+i];
	}
	size -= n;
}

// pwav elejet rateszi vegere:
void wav::vegereilleszt( wav* pwav, int n ) {
	if( n >= size )
		error( "n >= size!" );
	if( n >= pwav->size )
		error( "n >= pwav->size!" );
	for( int i = 0; i < n; i++ ) {
		double arany = ((double)i)/n;
		tomb[size-n+i] = (1-arany)*tomb[i] + arany*pwav->tomb[i];
	}
}

// WAV2 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//short* tomb, mtomb;
wav2::wav2( wav* pwav ) {
	mtomb = NULL;
	tomb = pwav->tomb;
	#ifndef WATCOM
		error( "wav2 csak Watcom alatt mukodik!" );
	#endif
	if( pwav->size > 64000 )
		error( "wav2-ben size > 64000!" );
	{ mtomb = NEW(short*) short[pwav->size]; }
	if( !mtomb )
		uzenet( "memory" );
	for( int i = 0; i < pwav->size-1; i++ )
		mtomb[i] = tomb[i+1]-tomb[i];
	mtomb[pwav->size-1] = tomb[0]-tomb[pwav->size-1];
	ulsize = ((unsigned long)pwav->size)*65536;
	ulfuto = 0;
	plow = (unsigned short*)&ulfuto;
	phigh = plow+1;
}

void wav2::reset( int fazis ) {
	if( fazis < 0 )
		ulfuto = 0;
	else
		ulfuto = fazis*65536;
}

// dt 65536 szoros:
short wav2::getnextsample( long dt ) {
	ulfuto += dt;
	if( ulfuto >= ulsize )
		ulfuto -= ulsize;
	return tomb[*phigh] + ((mtomb[*phigh]*(*plow))>>16);
}

