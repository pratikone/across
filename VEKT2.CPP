#include	"all.h"

vector2 vector2i( 1, 0 ), vector2j( 0, 1 ), vector2null( 0, 0 );
double Pi = 3.141592;

vector2 operator * ( double x, vector2 a ) {
	return vector2( a.x*x, a.y*x );
}

vector2 operator * ( vector2 a, double x ) {
	return vector2( a.x*x, a.y*x );
}

vector2::vector2( void ) {
	x = y = 0;
}

vector2::vector2( double xp, double yp ) {
	x = xp;
	y = yp;
}

vector2 vector2::operator + ( vector2 a ) {
	return vector2( x+a.x, y+a.y );
}

vector2 vector2::operator - ( vector2 a ) {
	return vector2( x-a.x, y-a.y );
}

double vector2::operator * ( vector2 a ) {
	return x*a.x + y*a.y;
}

double vector2::operator % ( vector2 b ) {
	return x*b.y-y*b.x;
}

void vector2::forgatas( double alfa ) {
	double a = sin( alfa );
	double b = cos( alfa );
	double xo = x;
	x = b*x-a*y;
	y = a*xo+b*y;
}

double abs( vector2 a ) {
	return gyok( a.x*a.x + a.y*a.y );
}

vector2 egys( vector2 a ) {
	return a*(1/abs( a ));
}

void vector2::normal( void ) {
	double recabs = 1/abs( *this );
	x *= recabs;
	y *= recabs;
}

vector2 rotateby90degrees( vector2 in ) {
	return vector2( -in.y, in.x );
}

vector2 rotatebyminus90degrees( vector2 in ) {
	return vector2( in.y, -in.x );
}

// Sajat negyzetgyok:
double gyok( double a ) {
	if( a < 0 ) {
		hiba( "Gyokvonas negativ szammal!" );
		return 1;
	}
	double x1 = sqrt( a );
	if( x1 == 0 )
		return 0;
	return .5*(x1+a/x1);
}

// Az r1-bol indulo v1 vektor altal es az r2-bol indulo v2 altal
// meghatarozott ket egyenes metszespontjanak visszaadasa:
// Parhuzamosok eseten hiba( char* ); fv.-t hivja meg.
// A keplet a kovetkezo: p = r2-v2*(n*(r2-r1))/(n*v2), ahol n meroleges v1-re.
vector2 crossingpoint( vector2 r1, vector2 v1, vector2 r2, vector2 v2 ) {
	vector2 n = rotateby90degrees( v1 );
	double nv2 = n*v2;
	if( fabs( nv2 ) < 0.00000001 ) {
		// Ket parhuzamos egyenesrol van szo, visszaadjuk egyiknek a
		// vegpontjat, ami masikon fekszik:
		if( v1*v2 < 0 ) {
			r2 = r2 + v2;
			v2 = vector2null - v2; // ezt mar felesleges, mivel mar nem hasznaljuk
		}
		// Most egyiranyba mutat v1 es v2:
		if( (r2-r1)*v1 > 0 )
			return r2;
		else
			return r1;
	}
	v1.normal();
	v2.normal();
	nv2 = n*v2;
	double nr21 = n*(r2-r1);
	return r2-v2*(nr21/nv2);
}

// Pont szakasztol valo tavolsagat szamolja ki:
// (Szakasz hossza vsz hossza).
double pontszakasztav( vector2 rpont, vector2 rsz, vector2 vsz ) {
	vector2 rr = rpont-rsz;
	double skalered = vsz*rr;
	if( skalered <= 0 ) {
		// Elso pont tavolsagat adja vissza:
		return abs( rr );
	}
	if( skalered >= vsz*vsz ) {
		// Masodik pont tavolsagat adja vissza:
		return abs( rr-vsz );
	}
	// Egyenestol valo tavolsagat adja vissza:
	vector2 n = rotateby90degrees( egys( vsz ) );
	return fabs( n*rr );
}

// Pont egyenestol valo tavolsagat szamolja ki:
double pontegyenestav( vector2 rpont, vector2 rsz, vector2 vsz ) {
	vector2 rr = rpont-rsz;
	vector2 n = rotateby90degrees( egys( vsz ) );
	return fabs( n*rr );
}

// Visszaadja ket kor metszetet, r1-bol r2-be bal oldalon (Ganyolt):
vector2 ketkormetszete( vector2 r1, vector2 r2, double l1, double l2 ) {
	vector2 v = r2-r1;
	double l = abs( v );
	// Egy kis ganyolas:
	if( l >= l1+l2 )
		return vector2( -10000001.0, 0.0 );
	if( l1 >= l+l2 )
		return vector2( -10000001.0, 0.0 );
	if( l2 >= l1+l )
		return vector2( -10000001.0, 0.0 );
	vector2 vegys = v * (1/l);
	vector2 normal = rotateby90degrees( vegys );

	double x = (l1*l1 - l2*l2 + l*l) / (2.0*l);
	double m = gyok( l1*l1 - x*x );

	vector2 r = r1 + x*vegys + m*normal;
	return r;
}


// Igaz, ha v1 altal meghatarozott egyenest metszi r2,v2,
// vagyis ha v2 kezdo es vegpontja egyenes ket oldalan fekszik:
static int ketoldalon( vector2 v1, vector2 r2, vector2 v2 ) {
	vector2 norm = rotateby90degrees( v1 );
	int elso = r2*norm > 0;
	int masodik = (r2+v2)*norm > 0;
	if( (elso && !masodik) || (!elso && masodik) )
		return 1;
	return 0;
}

// Igaz, ha v1 altal meghatarozott egyenest metszi r2,v2,
// vagyis ha v2 kezdo es vegpontja egyenes ket oldalan fekszik:
// Ez csak akkor tekinti metszesnek, ha nem csak eppen erintik egymast:
static int ketoldalon2( vector2 v1, vector2 r2, vector2 v2 ) {
	static double kishatar = 0.0000000001;
	vector2 norm = rotateby90degrees( v1 );
	int elso = 0;
	double elsoertek = r2*norm;
	if( elsoertek > kishatar )
		elso = 1;
	if( elsoertek < -kishatar )
		elso = -1;
	int masodik = 0;
	double masodikertek = (r2+v2)*norm;
	if( masodikertek > kishatar )
		masodik = 1;
	if( masodikertek < -kishatar )
		masodik = -1;
	if( (elso == -1 && masodik == 1) || (elso == 1 && masodik == -1) )
		return 1;
	return 0;
}

// Igaz, ha v1 altal meghatarozott egyenest metszi r2,v2,
// vagyis ha v2 kezdo es vegpontja egyenes ket oldalan fekszik:
// Ez mar akkor is metszesnek tekinti, ha kozel vannak egymashoz:
static int ketoldalon3( vector2 v1, vector2 r2, vector2 v2 ) {
	static double kishatar = 0.00000001;
	vector2 norm = rotateby90degrees( v1 );
	int elso = 0;
	double elsoertek = r2*norm;
	if( elsoertek > kishatar )
		elso = 1;
	if( elsoertek < -kishatar )
		elso = -1;
	int masodik = 0;
	double masodikertek = (r2+v2)*norm;
	if( masodikertek > kishatar )
		masodik = 1;
	if( masodikertek < -kishatar )
		masodik = -1;
	if( (elso == -1 && masodik == -1) || (elso == 1 && masodik == 1) )
		return 0;
	return 1;
}

// Igaz, ha az r1-bol indulo v1 vektor es az r2-bol indulo v2 vektor
// metszi egymast:
int vektormetszes( vector2 r1, vector2 v1, vector2 r2, vector2 v2 ) {
	if( ketoldalon( v1, r2-r1, v2 ) && ketoldalon( v2, r1-r2, v1 ) )
		return 1;
	return 0;
}

// Igaz, ha az r1-bol indulo v1 vektor es az r2-bol indulo v2 vektor
// metszi egymast:
// Ez csak akkor tekinti metszesnek, ha nem csak eppen erintik egymast:
int vektormetszes2( vector2 r1, vector2 v1, vector2 r2, vector2 v2 ) {
	if( ketoldalon2( v1, r2-r1, v2 ) && ketoldalon2( v2, r1-r2, v1 ) )
		return 1;
	return 0;
}

// Ez mar akkor is metszesnek tekinti, ha kozel vannak egymashoz:
int vektormetszes3( vector2 r1, vector2 v1, vector2 r2, vector2 v2 ) {
	if( ketoldalon3( v1, r2-r1, v2 ) && ketoldalon3( v2, r1-r2, v1 ) )
		return 1;
	return 0;
}

// Igazzal ter vissza, ha metszi kort egyenes:
// Ket metszespont kozul a v iranyaba haladva az elsot adja vissza:
int intersection( vector2 p1, vector2 v, vector2 r_kor, double sugar, vector2* p ) {
	vector2 r = r_kor-p1;
	v.normal();
	vector2 k = v*(v*r); // p1-bol mutat egyenes legkozelebbi pontjaba
	double tav = pontegyenestav( r_kor, p1, v );
	double gyokalatt = sugar*sugar - tav*tav;
	if( gyokalatt < 0.0 )
		return 0;
	double t = sqrt( gyokalatt );
	*p = p1 + k - v*t;
	return 1;
}



