#include	"all.h"

//static void playextfile( long belyeg ) {
//	palyabe( -1, belyeg );

static long playextfile( char* filename ) {
	if( !palyabe( -1, 0, filename ) )
		error( "playextfile-ban !palyabe( -1, 0, filename )!" );
	int parreplay = 0;
	Korny->prec->erase( 0 );
	long ido = lejatszo( Korny->pleiro, &Korny->st,
						Korny->prec, parreplay );
	return ido;
}

// Nincs h-ja, haNo prototype ki van irva play.cpp-ben:
/*void playextmenu( void ) {
	static long* belyegtomb = NULL;
	if( !belyegtomb ) {
		belyegtomb = new long[MAXRUBRIKASZAM];
		if( !belyegtomb )
			error( "playextmenu-ben nincs eleg memoria belyegtomb-nek!" );
	}
	// Kiirjuk osszes lev file nameet ide:
	find_t ffblk;
	int done;
	done = _dos_findfirst( "*.lev", _A_NORMAL, &ffblk );
	int szam = 0;
	int kezdo = 0;
	while( !done ) {
		char* filename = ffblk.name;
		char* levelname = getlevelname( filename );
		if( !levelname ) {
			// Kiirunk egy erroruzenetet kepre:
			blt8( Korny->picbuffer, Korny->ppic_load );
			int y0 = 20;
			int dy = 30;
			Korny->pabc_load->writekozep( Korny->picbuffer, 160, y0+0*dy, "There was an error" );
			Korny->pabc_load->writekozep( Korny->picbuffer, 160, y0+1*dy,  "loading the file:" );
			Korny->pabc_load->writekozep( Korny->picbuffer, 160, y0+2*dy,  filename );
			Korny->pabc_load->writekozep( Korny->picbuffer, 160, y0+3*dy,  "It is not a valid level file!" );
			Korny->pabc_load->writekozep( Korny->picbuffer, 160, y0+4*dy,  "Press a key to continue!" );
			lassufizre( Korny->picbuffer, Korny->pal_load );
			mk_emptychar();
			mk_getextchar();
			done = _dos_findnext( &ffblk );
			continue;
		}

		if( strlen( levelname ) > MAXRUBRIKAHOSSZ )
			error( "mdvihgfpo!" );
		strcpy( Korny->rubrikak[szam], levelname );

		belyegtomb[szam] = getbelyeg( filename );

		if( strcmp( State->playextfilename, filename ) == 0 )
			kezdo = szam;

		done = _dos_findnext( &ffblk );
		szam++;
		if( szam >= MAXRUBRIKASZAM-4 )
			done = 1;
	}

	// Feltoltottuk nameeket, jon valasztas:
	valaszt val;
	val.szam = szam;
	val.kur = kezdo;
	val.egykepen = 6;
	val.x0 = 50;
	val.y0 = 40;
	val.dy = 25;
	val.hatter = Korny->picbuffer;
	val.pabc = Korny->pabc_playmenu;
	val.escelheto = 1;
	while( 1 ) {
		blt8( Korny->picbuffer, Korny->ppic_playmenu );
		Korny->pabc_playmenu->write( Korny->picbuffer, 10, 10, "Select external file!" );
		int eredmeny = val.valassz( Korny->pal_playmenu );
		if( eredmeny < 0 )
			return;
		val.kur = eredmeny;
		getbelyeghezname( belyegtomb[eredmeny], State->playextfilename );
		playextfile( belyegtomb[eredmeny] );
	}
} */

// Igaz, ha ujra jatszik, hamis ha listat ker:
static int afterplayext( char* filename, long ido ) {
	int kurrens = 0;
	while( 1 ) {
		// Megirjuk tablat:
		blt8( Korny->picbuffer, Korny->ppic_afterplaymenu );
		Korny->pabc_afterplaymenu->writekozep( Korny->picbuffer, 160, 10,
							   getlevelname( filename ) );
		// Kiirjuk ha lockolt:
		if( Ucsoloadszakasz_lezart )
			Korny->pabc_afterplaymenu->write( Korny->picbuffer,
					9, 173, "Locked" );

		// Ido feliratot keszitunk:
		char idoszov1[30] = "", idoszov2[30] = "";
		if( ido < 0 ) {
			strcpy( idoszov1, "You Failed" );
			strcpy( idoszov2, " to Finish!" );
		}
		else {
			strcpy( idoszov1, "Your time:" );
			ido2string( ido, idoszov2 );
		}

		Korny->pabc_afterplaymenu->write( Korny->picbuffer, 200, 104, idoszov1 );
		Korny->pabc_afterplaymenu->write( Korny->picbuffer, 200, 134, idoszov2 );

		//Korny->pal_afterplaymenu->set();
		// Valasztas:
		valaszt val;
		val.szam = 4;
		val.kur = kurrens;
		val.egykepen = 6;
		val.x0 = 50;
		val.y0 = 40;
		val.dy = 25;
		val.hatter = Korny->picbuffer;
		val.pabc = Korny->pabc_afterplaymenu;
		val.escelheto = 1;

		strcpy( Korny->rubrikak[0], "Play again" );
		strcpy( Korny->rubrikak[1], "Replay" );
		strcpy( Korny->rubrikak[2], "Save play" );
		strcpy( Korny->rubrikak[3], "File list" );
		//strcpy( Korny->rubrikak[4], "Main menu" );

		int eredmeny = val.valassz( Korny->pal_afterplaymenu );
		kurrens = eredmeny;

		if( eredmeny < 0 || eredmeny == 3 )
			return 0; // Lista
		if( eredmeny == 0 )
			return 1; // Play again
		if( eredmeny == 1 ) {
			replayjelenlegi(); // Replay
			continue;
		}
		if( eredmeny == 2 ) {
			unsigned long belyeg = getbelyeg( filename );
			saveplay( belyeg ); // Save play
			continue;
		}
		error( "Itt No szabadna lennie (afterplayext)!" );
	}
}

static void abcberendez( int szam ) {
	for( int i = 0; i < szam+3; i++ ) {
		for( int j = 0; j < szam-1; j++ ) {
			// Ha j es j+1 edik elem forditva van visszacsereljuk oket:
			char* name1 = Korny->rubrikak[j];
			char* name2 = Korny->rubrikak[j+1];
			if( strcmp( name1, name2 ) > 0 ) {
				// Csere:
				char tmp[20];
				strcpy( tmp, name1 );
				strcpy( name1, name2 );
				strcpy( name2, tmp );
			}
		}
	}
}

// Nincs h-ja, haNo prototype ki van irva play.cpp-ben:
void playextmenu( void ) {
	while( 1 ) {
		// Kiirjuk osszes lev file nameet ide:
		find_t ffblk;
		int done;
		done = _dos_findfirst( "*.lev", _A_NORMAL, &ffblk );
		int szam = 0;
		while( !done ) {
			char* filename = ffblk.name;
			//char* levelname = getlevelname( filename );
			/*if( !levelname ) {
				// Kiirunk egy erroruzenetet kepre:
				blt8( Korny->picbuffer, Korny->ppic_load );
				int y0 = 20;
				int dy = 30;
				Korny->pabc_load->writekozep( Korny->picbuffer, 160, y0+0*dy, "There was an error" );
				Korny->pabc_load->writekozep( Korny->picbuffer, 160, y0+1*dy,  "loading the file:" );
				Korny->pabc_load->writekozep( Korny->picbuffer, 160, y0+2*dy,  filename );
				Korny->pabc_load->writekozep( Korny->picbuffer, 160, y0+3*dy,  "It is not a valid level file!" );
				Korny->pabc_load->writekozep( Korny->picbuffer, 160, y0+4*dy,  "Press a key to continue!" );
				lassufizre( Korny->picbuffer, Korny->pal_load );
				mk_emptychar();
				mk_getextchar();
				done = _dos_findnext( &ffblk );
				continue;
			}

			if( strlen( levelname ) > MAXRUBRIKAHOSSZ )
				error( "mdvihgfpo!" );
			strcpy( Korny->rubrikak[szam], levelname ); */

			strcpy( Korny->rubrikak[szam], filename );

			done = _dos_findnext( &ffblk );
			szam++;
			if( szam >= MAXRUBRIKASZAM-4 )
				done = 1;
		}

		if( szam <= 0 )
			return;

		abcberendez( szam );

		int kezdo = 0; // Ha megtalalja eddigit, akkor kur-t raallitja
		for( int i = 0; i < szam; i++ ) {
			char tmp[50];
			strcpy( tmp, Korny->rubrikak[i] );
			//strcat( tmp, ".lev" );
			if( strcmpi( tmp, State->playextfilename ) == 0 )
				kezdo = i;
		}

		// Feltoltottuk nameeket, jon valasztas:
		valaszt val;
		val.szam = szam;
		val.kur = kezdo;
		val.egykepen = 6;
		val.x0 = 50;
		val.y0 = 40;
		val.dy = 25;
		val.hatter = Korny->picbuffer;
		val.pabc = Korny->pabc_playmenu;
		val.escelheto = 1;

		blt8( Korny->picbuffer, Korny->ppic_playmenu );
		Korny->pabc_playmenu->write( Korny->picbuffer, 10, 10, "Select external file!" );
		int eredmeny = val.valassz( Korny->pal_playmenu );
		if( eredmeny < 0 )
			return;

		char filename[20];
		if( strlen( Korny->rubrikak[eredmeny] ) > 15 )
			error( "playextmenu-ben Korny->rubrikak[eredmeny] ) > 15!" );
		strcpy( filename, Korny->rubrikak[eredmeny] );

		strcpy( State->playextfilename, filename );

		while( 1 ) {
			long ido = playextfile( filename );
			// Igaz, ha ujra jatszik, hamis ha listat ker:
			if( !afterplayext( filename, ido ) )
				break;
		}
	}
}






