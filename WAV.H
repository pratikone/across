
//enum { W_MONO = 1, W_LEFT, W_RIGHT };

class wav {
	void	allocate( void ); // reserve based on size!
public:
	signed short* 	tomb;
	unsigned 	 	size;

			wav( unsigned sizepar );
			// maxamplitudo varies between 0 and 1!:
			wav( char* name, double maxamplitudo,
									int start = -1, int ucsoutani = -1 );
	void    save( char* name );
	// Manipulalas:

	// First, make a loop from the vegebol:
	void loopol( int n );
	// pwav elejet rateszi vegere:
	void vegereilleszt( wav* pwav, int n );
};


class wav2 {
	short	*tomb, *mtomb;
	unsigned long ulsize, ulfuto;
	unsigned short *plow, *phigh;
public:
		wav2( wav* pwav );
	void reset( int fazis = -1 );
	// dt 65536 szoros:
	short getnextsample( long dt );
};

