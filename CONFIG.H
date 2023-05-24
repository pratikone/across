
class conf {
public:
	void load( char* name );
	void save( char* name );
	int viewxsize, viewysize;
	int viewxorig, viewyorig;
	int viewxtolas;
	int zoom;
};


extern conf Config;

