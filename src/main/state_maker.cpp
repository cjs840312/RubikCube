#include <iostream>
#include <fstream>
#include "shortTurn.h"
using namespace std;

void turn(bool in[24][24] , int t[24] , bool out[24][24] )
{
	for(int i =0; i<24;i++)
	{
		for(int j=0;j<24;j++)
		{	
			out[i][j] = ( in[i][t[j]]==1 );
		}
	}
}

void print( ostream& out, bool cubea[24][24], bool cubeb[24][24])
{
	for(int i=0;i<24;i++)
	{
		for(int j=0;j<24;j++)
			out<<cubea[i][j];
		out<<endl;
	}
	for(int i=0;i<24;i++)
	{
		for(int j=0;j<24;j++)
			out<<cubeb[i][j];
		out<<endl;
	}

}

void copy( bool m1[24][24] , bool m2[24][24] )
{
   for(int i=0;i<24;i++)
   {
      for(int j=0;j<24;j++)
         m1[i][j]=m2[i][j];
   }

}


int main()
{
	ofstream fout("test");
	bool state1a[24][24]={}; bool state2a[24][24]={};

	bool state1b[24][24]={}; bool state2b[24][24]={};

	copy(state1a,I);
	copy(state1b,I);

	char t;

	while(cin>>t)
	{
		if(t=='o')
			break;
		int x;
		switch(t)
		{
			case '0' :	x=0;	break;
			case '1' :	x=1;	break;
			case '2' :	x=2;	break;
			case '3' :	x=3;	break;
			case '4' :	x=4;	break;
			case '5' :	x=5;	break;
		}

		turn(state1a, a[x], state2a );
		turn(state1b, b[x], state2b );

		copy(state1a,state2a);
		copy(state1b,state2b);
	}

	print(fout,state1a,state1b);


    return 0;
}