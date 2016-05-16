#include <iostream>
#include <fstream>
#include "../sat/sat.h"
#include "shortTurn.h"
using namespace std;

void read( string name, bool ina[24][24], bool inb[24][24])
{
   ifstream fin( name.c_str());
   string s;

   for(int i=0;i<24;i++)
   {
      getline(fin,s);
      for(int j=0;j<24;j++)
         ina[i][j] = ( s[j] == '0' ? 0 : 1);
   }

   for(int i=0;i<24;i++)
   {
      getline(fin,s);
      for(int j=0;j<24;j++)
         inb[i][j] = ( s[j] == '0' ? 0 : 1);
   }
}

void copy( Var m1[24][24] , Var m2[24][24])
{
   for(int i=0;i<24;i++)
   {
      for(int j=0;j<24;j++)
         m1[i][j]=m2[i][j];
   }

}


int main()
{
   SatSolver solver;

   bool ina[24][24]={}; bool inb[24][24]={};
   read("test",ina,inb);

   for(int x=1;x<=20;x++)
   {
      cerr<<"trying to solve in "<<x<<" steps ... ";
      solver.initialize();
      Var inita[24][24]={};  Var initb[24][24]={};

      Var state1a[24][24]={};  Var state2a[24][24]={};
      Var state1b[24][24]={};  Var state2b[24][24]={};

      for(int i=0;i<24;i++)
      {   
         for(int j=0;j<24;j++)
         {
            inita[i][j]=solver.newVar();  initb[i][j]=solver.newVar();

            if(ina[i][j]==0)  solver.addConst0CNF(inita[i][j]);
            else              solver.addConst1CNF(inita[i][j]);

            if(inb[i][j]==0)  solver.addConst0CNF(initb[i][j]);
            else              solver.addConst1CNF(initb[i][j]);
         }
      }

      copy(state1a,inita); copy(state1b,initb);
      
      Var* Turns = new Var[3*x];
      for(int z=0; z<x;z++)
      {
         Turns[3*z]=solver.newVar(); Turns[3*z+1]=solver.newVar(); Turns[3*z+2]=solver.newVar();
         solver.addsp(Turns[3*z],Turns[3*z+1]);
      }


      for(int y=0;y<x-1;y++)
      {
         for(int i=0;i<24;i++)
         {   
            for(int j=0;j<24;j++)
            {
               state2a[i][j]=solver.newVar();
               state2b[i][j]=solver.newVar();
            }

         }    
         for(int i=0;i<24;i++)
         {
            for(int j=0;j<24;j++)
            {
               Var tempAnda[6]={}; Var tempAndb[6]={};
               for(int m=0;m<6;m++)
               {
                  tempAnda[m]=solver.newVar(); tempAndb[m]=solver.newVar(); 
                  vector<Var> via ; vector<bool>bia;
                  vector<Var> vib ; vector<bool>bib;

                  via.push_back(Turns[3*y+0]); via.push_back(Turns[3*y+1]); via.push_back(Turns[3*y+2]); via.push_back(state1a[i][a[m][j]]);
                  vib.push_back(Turns[3*y+0]); vib.push_back(Turns[3*y+1]); vib.push_back(Turns[3*y+2]); vib.push_back(state1b[i][b[m][j]]);

                  bia.push_back((m>>2)%2); bia.push_back((m>>1)%2); bia.push_back(m%2); bia.push_back(1); 
                  bib.push_back((m>>2)%2); bib.push_back((m>>1)%2); bib.push_back(m%2); bib.push_back(1); 

                  solver.addAndCNF(tempAnda[m],via,bia);
                  solver.addAndCNF(tempAndb[m],vib,bib);
               }
               solver.addOrCNF(state2a[i][j], tempAnda[0], tempAnda[1], tempAnda[2], tempAnda[3], tempAnda[4], tempAnda[5]);
               solver.addOrCNF(state2b[i][j], tempAndb[0], tempAndb[1], tempAndb[2], tempAndb[3], tempAndb[4], tempAndb[5]);
            }
         }  
           
         copy(state1a, state2a);
         copy(state1b, state2b);
      }

      for(int i=0;i<24;i++)
      {   
         for(int j=0;j<24;j++)
         {
            state2a[i][j]=solver.newVar();
            state2b[i][j]=solver.newVar();
         }

      }
      for(int i=0;i<24;i++)
      {      
         Var tempAnda[6]={}; Var tempAndb[6]={};
         for(int m=0;m<6;m++)
         {
            tempAnda[m]=solver.newVar(); tempAndb[m]=solver.newVar(); 
            vector<Var> via ; vector<bool>bia;
            vector<Var> vib ; vector<bool>bib;

            via.push_back(Turns[3*(x-1)+0]); via.push_back(Turns[3*(x-1)+1]); via.push_back(Turns[3*(x-1)+2]); via.push_back(state1a[i][a[m][i]]);
            vib.push_back(Turns[3*(x-1)+0]); vib.push_back(Turns[3*(x-1)+1]); vib.push_back(Turns[3*(x-1)+2]); vib.push_back(state1b[i][b[m][i]]);

            bia.push_back((m>>2)%2); bia.push_back((m>>1)%2); bia.push_back(m%2); bia.push_back(1); 
            bib.push_back((m>>2)%2); bib.push_back((m>>1)%2); bib.push_back(m%2); bib.push_back(1); 

            solver.addAndCNF(tempAnda[m],via,bia);
            solver.addAndCNF(tempAndb[m],vib,bib);
         }
         solver.addOrCNF(state2a[i][i], tempAnda[0], tempAnda[1], tempAnda[2], tempAnda[3], tempAnda[4], tempAnda[5]);
         solver.addOrCNF(state2b[i][i], tempAndb[0], tempAndb[1], tempAndb[2], tempAndb[3], tempAndb[4], tempAndb[5]);               
      }
      copy(state1a, state2a);
      copy(state1b, state2b);

      for(int i=0; i<24;i++)
      {
         solver.addConst1CNF(state2a[i][i]);
         solver.addConst1CNF(state2b[i][i]);
      }

      bool result;

      solver.assumeRelease();
      solver.assumeProperty(state2a[0][0], true);
      result = solver.assumpSolve();


      cout<< ( result ? "SAT" : "UNSAT" ) <<endl;
      
      if (result)
      {
         cout<<endl;
         for(int y=0;y<x;y++)
            cout << solver.getValue(Turns[3*y+0])*4 + solver.getValue(Turns[3*y+1])*2 + solver.getValue(Turns[3*y+2]) << ' ';
         cout<<endl<<endl;
         delete [] Turns;
         return 0;
      }
      delete [] Turns;
   }


    return 0;
}
