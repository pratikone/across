#include	"all.h"

// Nem engedi vonalon+egykicsi tovabb kereket!:
static void helyigazitas( kor* pk, vector2* pt ) {
	double hossz = abs( pk->r - *pt );
	vector2 n = (pk->r-*pt)*(1.0/hossz);
	if( hossz < pk->sugar-Belsosav )
		pk->r = pk->r + n*(pk->sugar-Belsosav-hossz);
}

static double Utodeshatar = 0.5;

// Igazzal ter vissza, ha talppont tenylegesen az:
static int talppontigazitas( kor* pk, vector2* pt, vector2 F ) {
	double hossz = abs( pk->r - *pt );
	vector2 n = (pk->r-*pt)*(1.0/hossz);
	if( n*pk->v > -Elszakadasisebhat && n*F > 0 )
		// Nem talppont:
		return 0;
	// Sebesseg korrigalasa csak n-re meroleges komponensre:
	vector2 kulonbseg = n*pk->v*n;
	pk->v = pk->v - kulonbseg;
	// Hang:
	double ero = abs( kulonbseg );
	if( ero > Utodeshatar ) {
		ero = ero/Utodeshatar*0.1;
		if( ero >= 0.99 )
			ero = 0.99;
		startwavegyujto( WAV_UTODES, ero, -1 );
	}
	return 1;
}

// Igaz, ha talppont t1:
static int biztostalppont( vector2 t1, vector2 t2, kor* pk, vector2 F, double M ) {
	double hossz = abs( pk->r - t2 );
	vector2 n = (pk->r-t2)*(1.0/hossz);
	vector2 n90 = rotateby90degrees( n );
	double Mtm = M + hossz*n90*F;
	if( (t1-t2)*n90*Mtm < 0 )
		return 0;
	else
		return 1;
}

void beallit( kor* pk, vector2 F, double M, double dt, int kelltalppont ) {
	int talppontszam = 0;
	vector2 t1, t2;
	if( kelltalppont )
		talppontszam = talppontkereses( pk->r, pk->sugar, &t1, &t2 );

	if( talppontszam > 0 )
		helyigazitas( pk, &t1 );
	if( talppontszam > 1 )
		helyigazitas( pk, &t2 );

	if( talppontszam == 2 ) {
		// Megprobalja csokkenteni talppontszamok szama ugy, hogy figyelembe
		// veszi masik talppont koruli elfordulast is!:
		if( !biztostalppont( t1, t2, pk, F, M ) ) {
			talppontszam = 1;
			t1 = t2;
		}
		else {
			if( !biztostalppont( t2, t1, pk, F, M ) ) {
				talppontszam = 1;
			}
		}
	}
	if( talppontszam == 2 ) {
		if( !talppontigazitas( pk, &t2, F ) )
			talppontszam = 1;
	}
	if( talppontszam >= 1 ) {
		if( !talppontigazitas( pk, &t1, F ) ) {
			if( talppontszam == 2 ) {
				talppontszam = 1;
				t1 = t2;
			}
			else
				talppontszam = 0;
		}
	}
	// Talppontok most mar be vannak allitva!
	/*if( talppontszam >= 1 )
		pk->utoljaraleert = 1;
	else
		pk->utoljaraleert = 0;
	*/
	if( talppontszam == 0 ) {
		// Szabadon forog a levegoben:
		double beta = M/pk->theta;
		pk->omega += beta*dt;
		pk->alfa += pk->omega*dt;
		vector2 a = F*(1.0/pk->m);
		pk->v = pk->v + a*dt;
		pk->r = pk->r + pk->v*dt;
		return;
	}
	if( talppontszam == 2 ) {
		// Kerek meg van fogva:
		pk->v = vector2null;
		pk->omega = 0;
		return;
	}
	// Egy talppont korul fordul el kerek:
	double hossz = abs( pk->r-t1 );
	vector2 n = (pk->r-t1)*(1.0/hossz);
	vector2 n90 = rotateby90degrees( n );
	pk->omega = pk->v*n90*(1.0/pk->sugar);
	M += F*n90*pk->sugar; // M-hez hozzajon meg ero is, mivel talpponthoz
						  // vonatkoztatjuk!
	double thetaszelso = pk->theta+pk->m*hossz*hossz;
	double beta = M/thetaszelso;

	pk->omega += beta*dt;
	pk->alfa += pk->omega*dt;
	pk->v = pk->omega*pk->sugar*n90;
	pk->r = pk->r + pk->v*dt;
	return;
}






