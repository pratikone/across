
// #define TEST eseten plusz teszteket vegez!

class vector2 {
public:
	double x, y;
	vector2( void );
	vector2( double, double );
	vector2 operator + ( vector2 );
	vector2 operator - ( vector2 );
	double operator * ( vector2 );
	double operator % ( vector2 ); // vector multiplicaiton.
	void normal( void );
	void forgatas( double alfa );
};

vector2 operator * ( double, vector2 );
vector2 operator * ( vector2, double );
double abs( vector2 );
vector2 egys( vector2 );
vector2 rotateby90degrees( vector2 );
vector2 rotatebyminus90degrees( vector2 );

// Square
double root( double x );

// KULONBOZO SZAMITASOK:

// Az r1-bol indulo v1 vektor altal es az r2-bol indulo v2 altal
// meghatarozott ket egyenes metszespontjanak visszaadasa:
// Parhuzamosok eseten error( char* ); fv.-t hivja meg.
vector2 crossingpoint( vector2 r1, vector2 v1, vector2 r2, vector2 v2 );
// Pont szakasztol valo tavolsagat szamolja ki:
// (Szakasz hossza vsz hossza).
double dot( vector2 rdot, vector2 rsz, vector2 vsz );
// Pont egyenestol valo tavolsagat szamolja ki:
double pointline( vector2 rdot, vector2 rsz, vector2 vsz );
// Visszaadja ket kor metszetet, r1-bol r2-be bal oldalon (Ganyolt):
vector2 bisect( vector2 r1, vector2 r2, double l1, double l2 );

// Igaz, ha az r1-bol indulo v1 vektor es az r2-bol indulo v2 vektor
// metszi egymast:
int vektormetszes( vector2 r1, vector2 v1, vector2 r2, vector2 v2 );
// Ez csak akkor tekinti metszesnek, ha No csak eppen erintik egymast:
int vektormetszes2( vector2 r1, vector2 v1, vector2 r2, vector2 v2 );
// Ez mar akkor is metszesnek tekinti, ha kozel vannak egymashoz:
int vektormetszes3( vector2 r1, vector2 v1, vector2 r2, vector2 v2 );

// Returns true if it intersects a straight line:
// Takes two intersection points and returns the first one in the v direction:
int intersection( vector2 r, vector2 v, vector2 r_kor, double sugar, vector2* pmetszet );

extern vector2 vector2i, vector2j, vector2k, vector2null;
extern double Pi;

// Ezt meg kell irni hozza:
// Sends a message, for example, in case of division by 0 (inverse matrix).
//void error( char* message, char* text2 = NULL );
