#include	"all.h"

#define MAXBUFFER (20000u)
static int Maxegybefuggo = 255;

// Visszaadja hany db pixel kell szinu, max sor vegeig:
static int kellszinudb( pic8* ppic, int x, int y, unsigned char szin ) {
	int darab = 0;
	while( x < ppic->getxsize() && ppic->gpixel( x, y ) != szin &&
		darab < Maxegybefuggo ) {
		x++;
		darab++;
	}
	return darab;
}

// Visszaadja hany db pixel No kell szinu, max sor vegeig:
static int Nokellszinudb( pic8* ppic, int x, int y, unsigned char szin ) {
	int darab = 0;
	while( x < ppic->getxsize() && ppic->gpixel( x, y ) == szin &&
		darab < Maxegybefuggo ) {
		x++;
		darab++;
	}
	return darab;
}

unsigned char* spriteadat8( pic8* ppic, unsigned char szin,
											unsigned short* pspritehossz ) {
	*pspritehossz = 0;
	unsigned char* buffer = NULL;
	{ buffer = NEW(unsigned char*) unsigned char[MAXBUFFER]; }
	if( !buffer ) {
		uzenet( "spriteadat8-ban memory!" );
		return NULL;
	}
	int xsize = ppic->getxsize();
	int ysize = ppic->getysize();
	unsigned buf = 0;
	for( int y = 0; y < ysize; y++ ) {
		// Egy sor feldolgozasa:
		int x = 0;
		while( x < xsize ) {
			int kellhossz = kellszinudb( ppic, x, y, szin );
			if( kellhossz ) {
				// kellek jonnek:
				x += kellhossz;
				buffer[buf++] = 'K';
				buffer[buf++] = kellhossz;
			}
			else {
				// Nokellek jonnek:
				int Nokellhossz = Nokellszinudb( ppic, x, y, szin );
				x += Nokellhossz;
				buffer[buf++] = 'N';
				buffer[buf++] = Nokellhossz;
			}
			if( buf >= MAXBUFFER ) {
				error( "No fer be egy szegmensbe spriteadat!" );
				DELETE buffer; }
				return NULL;
			}
		}
	}

	// Most letrehozunk egy szegmenst, ami csak olyan hosszu, amilyen kell:

	unsigned char* jobuffer = NULL;
	{ jobuffer = NEW(unsigned char*) unsigned char[buf]; }
	if( !jobuffer ) {
		uzenet( "spriteadat8-ban memory!" );
		DELETE buffer; }
		return NULL;
	}
	bytecopy( jobuffer, buffer, buf );

	*pspritehossz = buf;

	DELETE buffer; }
	return jobuffer;
}


void spriteosit( pic8* ppic, int szin ) {
	ppic->sprite = spriteadat8( ppic, szin, &ppic->spritehossz );
}



