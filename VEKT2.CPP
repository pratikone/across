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
	return root( a.x*a.x + a.y*a.y );
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

// Sajat negyzetroot:
double root( double a ) {
	if( a < 0 ) {
		error( "rootvonas negativ szammal!" );
		return 1;
	}
	double x1 = sqrt( a );
	if( x1 == 0 )
		return 0;
	return .5*(x1+a/x1);
}

// By the vector v1 starting from r1 and by v2 starting from r2
// return the intersection of two defined lines:
// Error in case of parallels( char* ); invites fv.
// The theorem follows: p = r2-v2*(n*(r2-r1))/(n*v2), where n is perpendicular to v1.
vector2 crossingpoint( vector2 r1, vector2 v1, vector2 r2, vector2 v2 ) {
	vector2 n = rotateby90degrees( v1 );
	double nv2 = n*v2;
	if( fabs( nv2 ) < 0.00000001 ) {
		// There are two parallel lines, we return a to one of them
// vegpoint, which lies on the machine:
		if( v1*v2 < 0 ) {
			r2 = r2 + v2;
			v2 = vector2null - v2; // this is unnecessary since we don't use it anymore
		}
		// Now v1 and v2 point in the same direction:
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

// Extracts a sequence from a point section:
// (Section length vs length).
double dot( vector2 rdot, vector2 rsz, vector2 vsz ) {
	vector2 rr = rdot-rsz;
	double scaled = vsz*rr;
	if( scaled <= 0 ) {
		// Returns the first dot array:
		return abs( rr );
	}
	if( scaled >= vsz*vsz ) {
		// Returns the full table of second point:
		return abs( rr-vsz );
	}
	// Returns the array with alignment:
	vector2 n = rotateby90degrees( egys( vsz ) );
	return fabs( n*rr );
}

// Extracts a line from a straight line:
double pointline( vector2 rdot, vector2 rsz, vector2 vsz ) {
	vector2 rr = rdot-rsz;
	vector2 n = rotateby90degrees( egys( vsz ) );
	return fabs( n*rr );
}

// Returns two age sections, from r1 to r2 on the left side (Ganyolt):
vector2 bisect( vector2 r1, vector2 r2, double l1, double l2 ) {
	vector2 v = r2-r1;
	double l = abs( v );
	// A bit tricky:
	if( l >= l1+l2 )
		return vector2( -10000001.0, 0.0 );
	if( l1 >= l+l2 )
		return vector2( -10000001.0, 0.0 );
	if( l2 >= l1+l )
		return vector2( -10000001.0, 0.0 );
	vector2 mixed = v * (1/l);
	vector2 normal = rotateby90degrees( mixed );

	double x = (l1*l1 - l2*l2 + l*l) / (2.0*l);
	double m = root( l1*l1 - x*x );

	vector2 r = r1 + x*mixed + m*normal;
	return r;
}


// True if r2,v2 intersects a line defined by v1,
// that is, if the start and end points of v2 lie on two sides of a straight line:
static int twosided( vector2 v1, vector2 r2, vector2 v2 ) {
	vector2 norm = rotateby90degrees( v1 );
	int first = r2*norm > 0;
	int second = (r2+v2)*norm > 0;
	if( (first && !second) || (!first && second) )
		return 1;
	return 0;
}

// Igaz, ha v1 altal meghatarozott egyenest metszi r2,v2,
// vagyis ha v2 kezdo es vegpontja egyenes ket oldalan fekszik:
// Ez csak akkor tekinti metszesnek, ha No csak eppen erintik egymast:
static int twosided2( vector2 v1, vector2 r2, vector2 v2 ) {
	static double kishatar = 0.0000000001;
	vector2 norm = rotateby90degrees( v1 );
	int first = 0;
	double firstertek = r2*norm;
	if( firstertek > kishatar )
		first = 1;
	if( firstertek < -kishatar )
		first = -1;
	int second = 0;
	double secondertek = (r2+v2)*norm;
	if( secondertek > kishatar )
		second = 1;
	if( secondertek < -kishatar )
		second = -1;
	if( (first == -1 && second == 1) || (first == 1 && second == -1) )
		return 1;
	return 0;
}

// Igaz, ha v1 altal meghatarozott egyenest metszi r2,v2,
// vagyis ha v2 kezdo es vegpontja egyenes ket oldalan fekszik:
// Ez mar akkor is metszesnek tekinti, ha kozel vannak egymashoz:
static int twosided3( vector2 v1, vector2 r2, vector2 v2 ) {
	static double kishatar = 0.00000001;
	vector2 norm = rotateby90degrees( v1 );
	int first = 0;
	double firstertek = r2*norm;
	if( firstertek > kishatar )
		first = 1;
	if( firstertek < -kishatar )
		first = -1;
	int second = 0;
	double secondertek = (r2+v2)*norm;
	if( secondertek > kishatar )
		second = 1;
	if( secondertek < -kishatar )
		second = -1;
	if( (first == -1 && second == -1) || (first == 1 && second == 1) )
		return 0;
	return 1;
}

// Igaz, ha az r1-bol indulo v1 vektor es az r2-bol indulo v2 vektor
// metszi egymast:
int vektormetszes( vector2 r1, vector2 v1, vector2 r2, vector2 v2 ) {
	if( twosided( v1, r2-r1, v2 ) && twosided( v2, r1-r2, v1 ) )
		return 1;
	return 0;
}

// Igaz, ha az r1-bol indulo v1 vektor es az r2-bol indulo v2 vektor
// metszi egymast:
// Ez csak akkor tekinti metszesnek, ha No csak eppen erintik egymast:
int vektormetszes2( vector2 r1, vector2 v1, vector2 r2, vector2 v2 ) {
	if( twosided2( v1, r2-r1, v2 ) && twosided2( v2, r1-r2, v1 ) )
		return 1;
	return 0;
}

// Ez mar akkor is metszesnek tekinti, ha kozel vannak egymashoz:
int vektormetszes3( vector2 r1, vector2 v1, vector2 r2, vector2 v2 ) {
	if( twosided3( v1, r2-r1, v2 ) && twosided3( v2, r1-r2, v1 ) )
		return 1;
	return 0;
}

// Igazzal ter vissza, ha metszi kort egyenes:
// Ket metszespont kozul a v iranyaba haladva az firstt adja vissza:
int intersection( vector2 p1, vector2 v, vector2 r_kor, double sugar, vector2* p ) {
	vector2 r = r_kor-p1;
	v.normal();
	vector2 k = v*(v*r); // p1-bol mutat egyenes legkozelebbi pontjaba
	double tav = pointline( r_kor, p1, v );
	double rootalatt = sugar*sugar - tav*tav;
	if( rootalatt < 0.0 )
		return 0;
	double t = sqrt( rootalatt );
	*p = p1 + k - v*t;
	return 1;
}



