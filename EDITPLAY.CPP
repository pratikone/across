#include	"all.h"

void editplay( void ) {
	invalidateegesz();
	if( Valtozott || State->editfilename[0] == 0 || Ptop->errors )
		if( !tolt_save( 0 ) )
			return;

	if( Ptop->errors )
		return;

	palyabe( -1, getbelyeg( State->editfilename ) );
	int parreplay = 0;
	Korny->prec->erase( 1 );
	lejatszo( Korny->pleiro, &Korny->st, Korny->prec, parreplay );

	mk_emptychar();

	// Visszaallitjuk eredeti allapotot:
	setresdd( 4 ); // 640x480
	Korny->pal_editor->set();
	initmou( 640, 480 );

	// Megprobaljuk kozepre hozni egeret:
	for( int i = 0; i < 1000; i++ )
		getmou( &Moux, &Mouy );
	int mmx = 300;
	int mmy = 255;
	Moux = mmx;
	Mouy = mmy;
	setmou( mmx, mmy );
	Moux = mmx;
	Mouy = mmy;
}



