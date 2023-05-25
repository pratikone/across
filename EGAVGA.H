
// Akkor igaz, ha regi rec file-t jatszunk le:
// Global replay allitja:
extern int Kellobjectrendezni;

// idot adja vissza szazadmasodpercben:
long lejatszo( palyaleiro* pleiro, st320* pst, recorder* prec, int replay );

void egesz( void );

void kitesz( int igen );
void kiteljeskerek( int x, int y, int sugar, double alfa = -10001.0 );
void LINEki( int x1, int y1, int x2, int y2 );
extern int ColorBG, ColorFG, ColorFG2;

void kinyom( int ertek ); // Csak teszt celokra!

extern long L;

extern kor Kor1, Kor2, Kor4;
extern vector2 Fejr; // Kor1-bol Kor12Fejr fuggveny allitja elo (leptet.h)!

extern szakaszok *Pszak, *Pszakt;

extern int Kajaszam, Kajakell;

extern int Hatra;

