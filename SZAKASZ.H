
struct szakasz;

struct targy;

struct LINE;

struct LINEnode {
	LINE* 	   pLINE;
	LINEnode* pnext;
};

typedef LINEnode* pLINEnode;

#define CSOMAGBANNODESZAM (500)
struct soknodecsomag {
	LINEnode nodetomb[CSOMAGBANNODESZAM];
	soknodecsomag* nextcsomag;
};

class topol;

class szakaszok {
	szakasz* tomb;
	int		 maxszam;
	int		 szam;
	int szakfuto;

	// Terbeli kereseshez:
	pLINEnode* tertomb;
	int xdim, ydim;
	double cellameret;
	vector2 origo;
	LINEnode* nextnode; // getnext ez alapjan dolgozik!


	void beindexelLINEat( LINE* pv, double maxtav );
	void bekot( int cellax, int cellay, LINE* pv );

	// Terbeli kereseshez nodkezeles:
	soknodecsomag* pfirstcsomag;
	int csomagbanbetelt;
	LINEnode* getnewnode( void );
	void deleteallnodes( void );
public:
	//szakaszok( void );
	szakaszok( FILE* h, int targyas );
	szakaszok( topol* ptop, int targyas );
	~szakaszok( void );
	void save( FILE* h, int targyas );
	//void add( vector2 r, vector2 v, int targyas );
	//void remove( szakasz* psz );

	// Terbeli fv.-ek:
	// rendez-t csak egyszer szabad hivni!
	void rendez( double maxtav );
	void ffirstrolasreset( vector2 r );
	LINE* getnext( void );

	// Osszes szakaszon vegigmennek:
	void ffirstrolasresetszak( void );
	szakasz* getnextszak( void );

	// Visszaadja kajak szamat!:
	// setallaktiv allitja be motor kezdeti helyzetet is!:
	int setallaktiv( int targytipusszam, targytomb* ptt );

	void killerekelore( targytomb* ptt );

	szakasz* getptrszakasz( int sorszam );
	int getsorszam( szakasz* pszakasz );
};

struct LINE {
	vector2 r, v, egyseg;
	double hossz;
};

struct szakasz {
public:
	LINE vv;
	targy*	 ptargy;
};

class leiro;

void loadszakaszok( char* name, int kulso, szakaszok** ppszak,
			szakaszok** ppszakt );
void saveszakaszok( char* name, szakaszok* pszak, szakaszok* pszakt );

// loadszakasz beleirja ptop->lezart-at ha topologia file-t olvas be:
// Ezt azutan afterplayext fuggveny hasznalja ki:
extern int Ucsoloadszakasz_lezart;






