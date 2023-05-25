
// Annak eldontesere, hogy kidrawolando doboz No log-e ki:
// peldaul: Hatarx1 = 2.0, Hatarx2 = 318.0 (resetkidrawol tolti ki):
extern double   Hatarx1,
				Hatarx2,
				Hatary1,
				Hatary2;

void resetkidrawol( void );

struct st320 {
	vector2 origo;
	int vxelozo, vyelozo;
	magassag *pmviz, *pmfugg;
};

class palyaleiro;

void kidrawol640( double t, targytomb* pttt, double ugrasnagysag );

extern double Mo_y, Mo_bal, Mo_dx; // 320-hoz
void kidrawol320( st320* pst, double t, palyaleiro* pleiro, int view,
		int timeki, double baljobb, double ugrasnagysag, int ugras1volt,
		double forgas );

// 640-es uzemmodba valt:
void setgraph( void );
void close640( void );

