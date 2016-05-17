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

	int x;

	while(cin>>x)
	{
		if(x==99)
			break;
		if(x%4==3)
		{
			cerr<<"illigal turn"<<endl;
			break;
		}
		turn(state1a, a[x], state2a );
		turn(state1b, b[x], state2b );

		copy(state1a,state2a);
		copy(state1b,state2b);
	}

	print(fout,state1a,state1b);


    return 0;
    
}