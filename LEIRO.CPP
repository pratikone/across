#include	"all.h"

int Displaysizex = 320, Displaysizey = 200;

void palyaleiro::foldbeolv_ratesz( int leiroindex ) {
	char foldname[20] = "";
	switch( leiroindex ) {
		case 0:
			strcpy( foldname, "4_fold1.pcx" );
			break;
		case 1:
			strcpy( foldname, "4_fold2.pcx" );
			break;
		case 2:
			strcpy( foldname, "4_fold3.pcx" );
			break;
		default:
			error( "foldbeolv-ban nincs meg leiroindex!" );
	}

	pic8* pfoldkis;
	if( leiroindex != 0 ) {
		{ pfoldkis = NEW(pic8*) pic8( foldname ); }
	}
	else {
		// Eloszor teljes meretu kepet olvassuk be:
		pic8* pfoldnagy;
		{ pfoldnagy = NEW(pic8*) pic8( foldname ); }
		// Lekicsinyitjuk:
		int kellmeret = 80;
		int kxsize = kellmeret*Aranyx/50.0;
		int kysize = kellmeret*Aranyy/50.0;
		{ pfoldkis = NEW(pic8*) pic8( kxsize, kysize ); }
		mintavetel8( pfoldkis, pfoldnagy );
		DELETE pfoldnagy; }
	}
	// Foldet x iranyban megsokszorozzuk:
	foldxalapsize = pfoldkis->getxsize();
	int szoros = 1;
	while( szoros*foldxalapsize < Displaysizex )
		szoros++;
	szoros++;
	if( pfold || pratesz )
		error( "palyaleiro::foldbeolv_ratesz-ben pfold || pratesz No NULL!" );
	{ pfold = NEW(pic8*) pic8( szoros*foldxalapsize, pfoldkis->getysize() ); }
	for( int i = 0; i < szoros; i++ ) {
		blt8( pfold, pfoldkis, i*foldxalapsize, 0 );
	}
	DELETE pfoldkis; }

	{ pratesz = NEW(ratesz*)ratesz( Displaysizey, pfold, foldxalapsize ); }

	if( leiroindex == 0 ) {
		vieweloter  = 39;
		viewhatter  = 127;
		viewmotorindex = 18;
	}
	if( leiroindex == 1 ) {
		vieweloter  = 24;
		viewhatter  = 127;
		viewmotorindex = 7;
	}
	if( leiroindex == 2 ) {
		vieweloter  = 98;
		viewhatter  = 127;
		viewmotorindex = 18;
	}
}

static void loadname( char* text, FILE* h ) {
	char sor[200];
	if( fgets( sor, 190, h ) == NULL )
		error( "No tudott olvasni sort filebol!" );
	char tmp[200];
	if( sscanf( sor, "%s", tmp ) != 1 )
		error( "loadname-ben No tudta feldolgozni sort!: ", sor );
	if( strlen( tmp ) > FILEnameHOSSZ )
		error( "loadname-ben name tul hosszu!: ", tmp );
	strcpy( text, tmp );
}

palyaleiro::palyaleiro( char* name ) {
	pal = NULL;
	ptt = NULL;
	pvpic8f = NULL;
	pviewbuffer = NULL;
	//pegviz = NULL;
	//pegfugg = NULL;
	//pfoldviz = NULL;
	//pfoldfugg = NULL;

	peg = NULL;
	pfelho = NULL;
	pfold = NULL;

	pratesz = NULL;

	pkiskerek = NULL;
	pkisa = NULL;
	pkisb = NULL;
	pkisc = NULL;
	pkisd = NULL;
	pkise = NULL;
	pkisfirstrud = NULL;
	pkishatsorud = NULL;
	pkisalkar = NULL;
	pkisfelkar = NULL;
	pkisfej = NULL;

	foldxalapsize = 0;

	int leiroindex = -1;
	if( strcmp( name, "leir1.lei" ) == 0 )
		leiroindex = 0;
	if( strcmp( name, "leir2.lei" ) == 0 )
		leiroindex = 1;
	if( strcmp( name, "leir3.lei" ) == 0 )
		leiroindex = 2;
	if( leiroindex < 0 )
		error( "palyaleiro::palyaleiro-ban No talalta meg nameet!: ", name );


	FILE* h = qopen( name, "rt" );
	if( !h )
		error( "No tudta megnyitni palyaleirot!: ", name );



	{ pvpic8f = NEW(pic8f*) pic8f( Config.viewxsize, Config.viewysize ); }
	{ pviewbuffer = NEW(pic8*) pic8( Config.viewxsize, Config.viewysize ); }

	filename tmp;
	loadname( tmp, h );
	{ ptt = NEW(targytomb*) targytomb( tmp ); }

	// EG:
	loadname( tmp, h );
	{ peg = NEW(pic8*) pic8( tmp ); }
	pcxtopal( tmp, &pal );
	// FELHO:
	loadname( tmp, h );
	// Nagy felho:
	if( State->res > 2 ) {
		loadname( tmp, h ); // Nagy felbontasban nagy felho felulir
	}
	else {
		filename tmp2;
		loadname( tmp2, h ); // Kis felbontasban nagy felho elveszik
	}
	{ pfelho = NEW(pic8*) pic8( tmp ); }
	unsigned char szin = pfelho->gpixel( 0, 0 );
	spriteosit( pfelho, szin );
	// FOLD:
	//loadname( tmp, h ); fold namee most be van huzalozva:
	foldbeolv_ratesz( leiroindex );

	// Kerek:
	loadname( tmp, h );
	{ pkiskerek = NEW(kiskep*) kiskep( tmp ); }
	// A:
	loadname( tmp, h );
	{ pkisa = NEW(kiskep*) kiskep( tmp ); }
	// B:
	loadname( tmp, h );
	{ pkisb = NEW(kiskep*) kiskep( tmp ); }
	// C:
	loadname( tmp, h );
	{ pkisc = NEW(kiskep*) kiskep( tmp ); }
	// D:
	loadname( tmp, h );
	{ pkisd = NEW(kiskep*) kiskep( tmp ); }
	// E:
	loadname( tmp, h );
	{ pkise = NEW(kiskep*) kiskep( tmp ); }
	// Lyukindexeket a-bol atmasoljuk:
	pkisb->lyuk = pkisc->lyuk = pkisd->lyuk = pkise->lyuk = pkisa->lyuk;
	// firstrud:
	loadname( tmp, h );
	{ pkisfirstrud = NEW(kiskep*) kiskep( tmp ); }
	// Hatsorud:
	loadname( tmp, h );
	{ pkishatsorud = NEW(kiskep*) kiskep( tmp ); }
	// Alkar:
	loadname( tmp, h );
	{ pkisalkar = NEW(kiskep*) kiskep( tmp ); }
	// Alkar:
	loadname( tmp, h );
	{ pkisfelkar = NEW(kiskep*) kiskep( tmp ); }
	// Fej:
	loadname( tmp, h );
	{ pkisfej = NEW(kiskep*) kiskep( tmp ); }

	// View indexek:
	vieweloter  = loadint( h );
	viewhatter  = loadint( h );
	viewmotorindex  = loadint( h );

	idoszin1 = loadint( h );
	idoszin2 = loadint( h );

	ugroindex = loadint( h );
	keretindex = loadint( h );

	int magicnumber = loadint( h );
	if( magicnumber != 17322 )
		error( "leiroban No stimmel magic number!: ", name );

	qclose( h );
}

palyaleiro::~palyaleiro( void ) {
	if( pal )
		DELETE pal; }
	if( ptt )
		DELETE ptt; }
	if( pvpic8f )
		DELETE pvpic8f; }
	if( pviewbuffer )
		DELETE pviewbuffer; }
	/*if( pegviz )
		DELETE pegviz; }
	if( pegfugg )
		DELETE pegfugg; }
	if( pfoldviz )
		DELETE pfoldviz; }
	if( pfoldfugg )
		DELETE pfoldfugg; } */

	if( peg )
		DELETE peg; }
	if( pfelho )
		DELETE pfelho; }
	if( pfold )
		DELETE pfold; }

	if( pratesz )
		DELETE pratesz; }

	if( pkiskerek )
		DELETE pkiskerek; }
	if( pkisa )
		DELETE pkisa; }
	if( pkisb )
		DELETE pkisb; }
	if( pkisc )
		DELETE pkisc; }
	if( pkisd )
		DELETE pkisd; }
	if( pkise )
		DELETE pkise; }
	if( pkisfirstrud )
		DELETE pkisfirstrud; }
	if( pkishatsorud )
		DELETE pkishatsorud; }
	if( pkisalkar )
		DELETE pkisalkar; }
	if( pkisfelkar )
		DELETE pkisfelkar; }
	if( pkisfej )
		DELETE pkisfej; }
}

// Csak foldet csereli ki Korny->leiroazonosito szerint:
// play-bol van hivva:
void csakfoldcsere( void ) {
	if( !Korny->pleiro->pfold || !Korny->pleiro->pratesz )
		error( "csakfoldcsere-ben !Korny->pleiro->pfold || !Korny->pleiro->pratesz!" );

	DELETE Korny->pleiro->pfold; }
	Korny->pleiro->pfold = NULL;
	DELETE Korny->pleiro->pratesz; }
	Korny->pleiro->pratesz = NULL;

	Korny->pleiro->foldbeolv_ratesz( Korny->leiroazonosito );
}


