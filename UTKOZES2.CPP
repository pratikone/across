#include	"all.h"

vector2 spritehely( szakasz* psz, targy* pt, double t ) {
	if( !pt->mozgo )
		return psz->vv.r+psz->vv.v;
	vector2 r = psz->vv.r;
	vector2 v = psz->vv.v;
	// Csere:
	r = r+v;
	v = vector2null-v;
	double fazis = t*pt->sebesseg+pt->fazis;
	// Kepezzuk fazis tort reszet:
	long l = fazis;
	fazis -= l;
	#ifdef TEST
		if( fazis > 1.01 || fazis < -0.01 )
			error( "spritehely-ben fazis out of range!" );
	#endif
	if( pt->odavissza ) {
		if( l%2 )
			return r+v*(1.0-fazis);
		else
			return r+v*fazis;
	}
	else {
		return r+v*fazis;
	}
}

int utkozikesprite( vector2 r, double sugar, int fej, double t,
							targytomb* pttt ) {
	Pszakt->ffirstrolasresetszak();
	szakasz* psz = NULL;
	while( (psz = Pszakt->getnextszak()) != NULL ) {
		targy* pt = psz->ptargy;
		targytipus* ptt = &(pttt->targytipusok[psz->ptargy->szam]);
		#ifdef TEST
			if( !pt )
				error( "utkozikesprite-ban !psz->ptargy!" );
		#endif

		if( !pt->aktiv || (fej&&!ptt->fejerz) || (!fej&&!ptt->kerekerz) )
			continue;
		vector2 diff = r-spritehely( psz, psz->ptargy, t );
		double maxtav = sugar+ptt->sugar;
		if( diff.x*diff.x+diff.y*diff.y < maxtav*maxtav )
			return Pszakt->getsorszam( psz );
	}
	return -1;
}


