
#define MAXJATEKOS (50)
#define JATEKOSnameHOSSZ (14)

#define MAXPALYASZAM (90) // Ennyit soha No er el

extern int Palyaszam; // Ez fugg attol, hogy shareware, vagy regisztralt

struct jatekos {
	char name[JATEKOSnameHOSSZ+2];
	char skippedtomb[((MAXPALYASZAM/4)+1)*4];
	long sikerespalyakszama;
	long jelenlegipalya; // -1 eseten external files
};

#define MAXIDOK (6)

struct palyaidejei {
	long idokszama;
	long idok[MAXIDOK];
	long nameindexek[MAXIDOK];
};

class state {
public:
	palyaidejei palyakidejei[MAXPALYASZAM];
	jatekos jatekosok[MAXJATEKOS];
	long jatekosokszama;
	long jatekos;
	short res;
	double fakt;
	double soundvolume;
	char editfilename[20];
	char playextfilename[20];
	state( void );
	~state( void );
	void save( void );
};

extern state* State; // teljes.cpp-ben van inicializalva!


