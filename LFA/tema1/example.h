class Line {
	public:
		void setLenght( double len );
		double getLenght( void );
		Line(); // constructor
		//Line(double l);

	private:double length;
}

class Rectangle {
	int width, height;
	public:
		Rectangle();
		Rectangle (int w, int h);
		int area (void);
		void draw(int x, int y, int w, int h);
		void draw(float x, float y, float w, float h);
}

class Line {
	public:
		int getLength( void );
		Line( int len );		//constructor
		Line( const Line &obj);	//constructor
		~Line();	//destructor
	private:
		int *ptr;
}
