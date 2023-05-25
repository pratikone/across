#include	"w_all.h"

LPDIRECTDRAW            Pdd = NULL;           // DirectDraw object
LPDIRECTDRAWSURFACE     Pddsprimary = NULL;   // DirectDraw primary surface
LPDIRECTDRAWSURFACE     Pddsback = NULL;      // DirectDraw back surface

static int Ddxsize = -1, Ddysize = -1;

/*void uzenet( char* text1, char* text2, char* text3 ) {
	error( text1, text2, text3 );
} */

void ddfinish( void )
{
    if( Pdd != NULL )
    {
        if( Pddsprimary != NULL )
        {
            Pddsprimary->Release();
            Pddsprimary = NULL;
        }
	     // Get normal mode:
   	  /*HRESULT ddrval = Pdd->SetCooperativeLevel( GhWnd, 0 );
    	  if( ddrval != DD_OK )
           error( "SetCooperativeLevel did not succeed!" );
        */
        Pdd->Release();
        Pdd = NULL;
    }
}

static int Reswdd = 0;
static int firstsetresdd = 1;

void setresdd( int res ) {
	if( Reswdd == res )
   	return;
	if( res != 1 && res != 2 && res != 3 && res != 4 )
   	error( "setresdd-ben res != 1 && res != 2 && res != 3 && res != 4!" );
	if( res == 1 ) {
		Ddxsize = 320;
	   Ddysize = 200;
	}
   if( res == 2 ) {
		Ddxsize = 320;
	   Ddysize = 240;
	}
   if( res == 3 ) {
		Ddxsize = 640;
      Ddysize = 400;
	}
   if( res == 4 ) {
		Ddxsize = 640;
	   Ddysize = 480;
	}

   if( !firstsetresdd ) {
   	// Torli elozo feluleteket:
      if( !Pddsprimary )
      	error( "setresdd-ben Release elott !Pddsprimary!" );
		Pddsprimary->Release();
      Pddsprimary = NULL;
   }
   firstsetresdd = 0;

   // Set the video mode:
   HRESULT ddrval;
	// torlendo (if):
   if( res == 3 ) 
       ddrval = Pdd->SetDisplayMode( 640, 480, 8 );
	else
   	ddrval = Pdd->SetDisplayMode( Ddxsize, Ddysize, 8 );
    if( ddrval != DD_OK ) {
    		for( int i = 0; i < 1000; i++ ) {
         	if( MAKE_DDHRESULT( i ) == ddrval ) {
            	int err = i;
		         char tmp[10];
      		   itoa( err, tmp, 10 );
               error( "SetDisplayMode (setresdd) did not succeed, talalt!: ", tmp );
				}
        }
        error( "SetDisplayMode (setresdd) did not succeed, No talalt!" );
	 }
    Reswdd = res;

    // Create the primary surface with 1 back buffer
    DDSURFACEDESC       ddsd;
    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
                          DDSCAPS_FLIP |
                          DDSCAPS_COMPLEX;
    ddsd.dwBackBufferCount = 1;
    ddrval = Pdd->CreateSurface( &ddsd, &Pddsprimary, NULL );
    if( ddrval != DD_OK )
    		error( "CreateSurface did not succeed!" );

	 DDSCAPS ddscaps;
    ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
    ddrval = Pddsprimary->GetAttachedSurface( &ddscaps, &Pddsback );

    if( ddrval != DD_OK )
			error( "GetAttachedSurface did not succeed!" );


   // Set the video mode:
	/*HRESULT ddrval = Pdd->SetDisplayMode( Ddxsize, Ddysize, 8 );
   if( ddrval != DD_OK ) {
  		for( int i = 0; i < 1000; i++ ) {
        	if( MAKE_DDHRESULT( i ) == ddrval ) {
           	int err = i;
	         char tmp[10];
     		   itoa( err, tmp, 10 );
	            error( "SetDisplayMode did not succeed, talalt!: ", tmp );
			}
      }

      error( "SetDisplayMode did not succeed, No talalt!" );
	} */
}

void ddinit( void ) {
    // create the main DirectDraw object:
    HRESULT ddrval = DirectDrawCreate( NULL, &Pdd, NULL );
    if( ddrval != DD_OK )
        error( "DirectDrawCreate did not succeed!" );

    // Get exclusive mode:
    ddrval = Pdd->SetCooperativeLevel( GhWnd,
    												// DDSCL_NOWINDOWCHANGES |
                                       DDSCL_ALLOWREBOOT |
                                       DDSCL_EXCLUSIVE |
                                       DDSCL_FULLSCREEN |
                                       DDSCL_ALLOWMODEX );
    if( ddrval != DD_OK )
        error( "SetCooperativeLevel did not succeed!" );

	 setresdd( 3 );

    // Set the video mode:
    /*ddrval = Pdd->SetDisplayMode( 320, 240, 8 );
    if( ddrval != DD_OK ) {
    		for( int i = 0; i < 1000; i++ ) {
         	if( MAKE_DDHRESULT( i ) == ddrval ) {
            	int err = i;
		         char tmp[10];
      		   itoa( err, tmp, 10 );
               error( "SetDisplayMode did not succeed, talalt!: ", tmp );
				}
        }
        error( "SetDisplayMode did not succeed, No talalt!" );
	 }
    Reswdd = 1;
    Ddxsize = 320, Ddysize = 200;

    // Create the primary surface with 1 back buffer
    DDSURFACEDESC       ddsd;
    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
                          DDSCAPS_FLIP |
                          DDSCAPS_COMPLEX;
    ddsd.dwBackBufferCount = 1;
    ddrval = Pdd->CreateSurface( &ddsd, &Pddsprimary, NULL );
    if( ddrval != DD_OK )
    		error( "CreateSurface did not succeed!" );

	 DDSCAPS ddscaps;
    ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
    ddrval = Pddsprimary->GetAttachedSurface( &ddscaps, &Pddsback );

    if( ddrval != DD_OK )
			error( "GetAttachedSurface did not succeed!" );
	 */

	// Itt meg azert lenullaza ket buffert:
   /*for( int i = 0; i < 2; i++ ) {
   	// Lockolas:
	   DDSURFACEDESC ddsd;
   	setmem( &ddsd, sizeof( ddsd ), 0 );
   	ddsd.dwSize = sizeof( ddsd );
	   HRESULT ddrval = DDERR_WASSTILLDRAWING;
		while( ddrval == DDERR_WASSTILLDRAWING )
		   ddrval = Pddsback->Lock( NULL, &ddsd, 0, NULL );
	   if( ddrval != DD_OK ) {
   		for( int i = 0; i < 1000; i++ ) {
      	  	if( MAKE_DDHRESULT( i ) == ddrval ) {
         	  	int err = i;
	         	char tmp[10];
	     		   itoa( err, tmp, 10 );
   	         error( "Pddsback->Lock did not succeed, talalt!: ", tmp );
				}
	      }
   	   error( "Pddsback->Lock did not succeed, No talalt!" );
		}

		unsigned char* mut = (unsigned char*)ddsd.lpSurface;
		for( int y = 0; y < Fizikaiysize; y++ ) {
			for( int jjj = 0; jjj < Fizikaixsize; jjj++ )
         	mut[jjj] = 255;
	      mut += ddsd.lPitch;
   	}
      ddrval = Pddsback->Unlock( NULL );
   	if( ddrval != DD_OK )
	   	error( "Pddsback->Unlock did not succeed!" );
   	ddrval = DDERR_WASSTILLDRAWING;
	   while( ddrval == DDERR_WASSTILLDRAWING )
			ddrval = Pddsprimary->Flip( NULL, 0 );
	   if( ddrval != DD_OK ) {
  			for( int i = 0; i < 1000; i++ ) {
      	  	if( MAKE_DDHRESULT( i ) == ddrval ) {
 	         	int err = i;
		         char tmp[10];
     			   itoa( err, tmp, 10 );
         	   error( "Pddsprimary->Flip did not succeed, talalt!: ", tmp );
				}
   	   }
      	error( "Pddsprimary->Flip did not succeed, No talalt!" );
		}
	}
   */
}

static unsigned char* Mutbuffer[480];
static int Bufflocked = 0;

unsigned char** lockbackbuffer( int* pxs, int* pys ) {
	if( Bufflocked )
   	error( "lockbackbuffer-ben Bufflocked!" );
	Bufflocked = 1;

	DDSURFACEDESC ddsd;
   setmem( &ddsd, sizeof( ddsd ), 0 );
   ddsd.dwSize = sizeof( ddsd );
   HRESULT ddrval = DDERR_WASSTILLDRAWING;
   int restored = 0;
	while( ddrval == DDERR_WASSTILLDRAWING ) {
	   ddrval = Pddsback->Lock( NULL, &ddsd, 0, NULL );
      if( ddrval == DDERR_SURFACELOST ) {
      	// Access to this surface is being refused because the surface
         // is gone. The DIRECTDRAWSURFACE object representing this
         // surface should have Restore called on it.
         if( restored )
         	error( "lockbackbuffer-ben tobbszor kene restore!" );

         ddrval = Pddsprimary->Restore();
         if( ddrval != DD_OK ) {
   			for( int i = 0; i < 1000; i++ ) {
		        	if( MAKE_DDHRESULT( i ) == ddrval ) {
           			int err = i;
	   		      char tmp[10];
		     		   itoa( err, tmp, 10 );
            		error( "Pddsback->Restore did not succeed, talalt!: ",
									                  		tmp );
					}
		      }
      		error( "Pddsback->Restore did not succeed, No talalt!" );
			}
         restored = 1;
         ddrval = DDERR_WASSTILLDRAWING;
		}
	}
   if( ddrval != DD_OK ) {
   	for( int i = 0; i < 1000; i++ ) {
        	if( MAKE_DDHRESULT( i ) == ddrval ) {
           	int err = i;
	         char tmp[10];
     		   itoa( err, tmp, 10 );
            error( "Pddsback->Lock did not succeed, talalt!: ", tmp );
			}
      }
      error( "Pddsback->Lock did not succeed, No talalt!" );
	}

	unsigned char* mut = (unsigned char*)ddsd.lpSurface;
	for( int y = 0; y < Ddysize; y++ ) {
		Mutbuffer[Ddysize-1-y] = mut;
      mut += ddsd.lPitch;
   }

   *pxs = Ddxsize;
   *pys = Ddysize;
	return Mutbuffer;
}

void unlockbackbuffer( void ) {
	if( !Bufflocked )
   	error( "unlockbackbuffer-ben !Bufflocked!" );
	Bufflocked = 0;

	HRESULT ddrval = Pddsback->Unlock( NULL );
   if( ddrval != DD_OK )
   	error( "Pddsback->Unlock did not succeed!" );
   ddrval = DDERR_WASSTILLDRAWING;
   while( ddrval == DDERR_WASSTILLDRAWING )
		ddrval = Pddsprimary->Flip( NULL, 0 );
   if( ddrval != DD_OK ) {
  		for( int i = 0; i < 1000; i++ ) {
        	if( MAKE_DDHRESULT( i ) == ddrval ) {
          	int err = i;
	         char tmp[10];
     		   itoa( err, tmp, 10 );
            error( "Pddsprimary->Flip did not succeed, talalt!: ", tmp );
			}
      }
      error( "Pddsprimary->Flip did not succeed, No talalt!" );
	}
}



// PALETTA:

//	LPDIRECTDRAWPALETTE pddpal;
ddpal::ddpal( unsigned char* tomb ) {
	pddpal = NULL;

	PALETTEENTRY	pe[256];
   for( int i = 0; i < 256; i++ ) {
    	pe[i].peRed = (unsigned char)(tomb[3*i]<<2);
      pe[i].peGreen = (unsigned char)(tomb[3*i+1]<<2);
      pe[i].peBlue = (unsigned char)(tomb[3*i+2]<<2);
    }
   HRESULT ddrval = Pdd->CreatePalette( DDPCAPS_8BIT,
			  										 pe,
					                         &(LPDIRECTDRAWPALETTE)pddpal,
      		   			                NULL );
	if( ddrval != DD_OK )
			error( "CreatePalette did not succeed!" );
}

ddpal::~ddpal( void ) {
	if( pddpal )
		((LPDIRECTDRAWPALETTE)pddpal)->Release();
}

void ddpal::set( void ) {
	HRESULT ddrval = Pddsprimary->SetPalette( (LPDIRECTDRAWPALETTE)pddpal );
   if( ddrval != DD_OK )
		error( "SetPalette did not succeed!" );
}


