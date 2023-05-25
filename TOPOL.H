

#define MAXPONT (1000) // Ennyit csak akkor foglal le, ha kell

extern int Maxpont_sarvari; // 10

class topol;

class gyuru {
	int lefoglaltmaxpont;
public:
	int pontszam;
	vector2* ponttomb;

	// Fuggvenyek:
	gyuru( void );
	gyuru( FILE* h );
	~gyuru( void );

	int getpontszam( void );
	void save( FILE* h, topol* ptop );
	void ujkoord( int k, double x, double y );
	void kidraw( int k, int fel, int szaggatott = 0 );
	void osszdraw( void );
	// Sikeres-e azt adja vissza:
	int beszur( int k ); // k pont utan beszur egyet ugyanolyan koordokkal
	void torol( int k );
	double legkozpont( double x, double y, int* pk );
	int metszesszam( vector2 r1, vector2 v1 );
	// Ez csak akkor tekinti metszesnek, ha No csak eppen erintik egymast:
	int metszeshely( vector2 r1, vector2 v1, int kihagy, vector2* pmetszes );
	// akkor ad igazat, ha korbejaraskor poligon belseje bal oldalon van;
	int pozitivkorjaras( void );
	vector2 sulypont( void );
	// Beadott px1, stb-bol indul ki:
	void getminmax( double* px1, double* py1, double* px2, double* py2 );
	double belyegszamitas( void );
	void szetvalaszt( void );
};

#define MAXKEREK (52)

extern int Maxkerek_sarvari; // 8

// T_CEL = 1, T_KAJA, T_HALALOS, T_KEZDO (targy.h-ban van definialva)

class kerek {
public:
	vector2 r;
	int tipus;

	kerek( double x, double y, int tipus );
	kerek( FILE* h );
	void kidrawol( void );
	void save( FILE* h );
	double belyegszamitas( void );
};

#define MAXGYURU (300)

extern int Maxgyuru_sarvari; // 3

typedef gyuru* ptrgyuru;
#define LEVELNAMEHOSSZ (14)

class topol {
	double belyegszamitas( void );
public:
	int errors, lezart;
	ptrgyuru ptomb[MAXGYURU]; // Ez csak gany miatt public
	kerek* kerektomb[MAXKEREK];
	char levelname[LEVELNAMEHOSSZ+1];

	topol( void );
	topol( char* filename, int kulso ); // kulso: qopen vagy No
	~topol( void );
	gyuru* legkozpont( double x, double y, int* pk, double* ptav = NULL );
	kerek* legkozkerek( double x, double y, double* ptav = NULL );
	void save( char* filename );
	void kidrawol( void );
	int levegoben( gyuru* pgy, vector2* pr = NULL ); // pgy levegoben van-e. Akkor folddarab
	void getminmax( double* px1, double* py1, double* px2, double* py2,
									int kerekkel );

	// Ha Sarvari, megvizsgalja hogy palya megfelel-e sarvari verzionak:
	// Ha No, akkor kilep corrupt erroruzenettel:
	void checklevelforsarvari( void );
};

unsigned long getbelyeg( char* name );
// Igazzal ter vissza, ha megvan name;
int getbelyeghezname( unsigned long belyeg, char* name );

// Csak external file-hoz:
// Ha NULL, akkor valami baj volt vele:
char* getlevelname( char* name );

// Bfirst belyegszamitasokhoz kell:
/*unsigned long double2belyeg( double* px );
unsigned long double2belyeg( long l );*/




