
#define EGYTOMBHAT (1000)

#define ERTEKKESZLET (15000)

extern double Magmaxelemszam;

struct hat {
	int ffirst; // Levego kezdodik!
	int y;
	hat* pkov;
};

typedef hat*  ptrhat;

struct hattomb {
	hat hatok[EGYTOMBHAT];
	hattomb* pkov;
};

class magassag {
	hat** oszlopok;
	int xsize;
	// iterator mutatoja:
	hat* pcurhat;
	// hat tarolasahoz:
	hattomb *pcur, *pfirst;
	int kovelem;
	hat* gethat( void );
	void torolhatok( void );
public:
		magassag( int xsize );
		~magassag( void );
	int getxsize( void );
	void addszakasz( szakasz*, int ffirst );
	void rendezget( void );
	void initoszlop( int x );
	hat* getoszlopfirsthat( int x );
	// Hamis eseten No valtoztatja ffirst-t es y-t!:
	int gethat( int* ffirst, int* py );
};


vector2 magassagokeloallitasa( szakaszok*, magassag**, magassag** );
void kitoltcsikot( magassag* pm, int x, int ystart, int ysize,
	unsigned char* tomb, unsigned char* tombeg, unsigned char* tombfold );
void kitoltcsikot( magassag* pm, int x, int ystart, int ysize,
	unsigned char* tomb, unsigned char viewhat, unsigned char viewelo,
	int zoom );

void testmagassag( void );

