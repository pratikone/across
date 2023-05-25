
#define MAXRUBRIKAHOSSZ (40)
#define MAXRUBRIKASZAM (600)

typedef char rubrika[MAXRUBRIKAHOSSZ+2];

// Felhasznalja, Korny rubrikak-at es picbuffer-t!
class valaszt {
	void kidraw( int ffirst, ddpal* ppal );
public:
	int szam, kur, egykepen;
	int x0, y0, dy;
	pic8* hatter;
	abc8* pabc;
	int escelheto, tomoren;

	valaszt( void );
	// Visszaadja valasztott szamat, ESC eseten -1-t:
	// tobbdraw eseten tobbszor is kidrawolja kezdetben (Win95):
	int valassz( ddpal* ppal, int tobbdraw = 0 );
};

