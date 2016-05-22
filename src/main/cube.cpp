#include <iostream>
#include <fstream>
#include "../sat/sat.h"
#include "shortTurn.h"
using namespace std;

SatSolver solver;
bool ina[24][24]={};
bool inb[24][24]={};
Var inita[24][24]  ={};  Var initb[24][24]  ={};
Var state1a[24][24]={};  Var state2a[24][24]={};
Var state1b[24][24]={};  Var state2b[24][24]={};


void read( string , bool[24][24], bool[24][24]);
void copy( Var[24][24] , Var[24][24]);
void copy_2_to_1(){ copy(state1a, state2a); copy(state1b, state2b); }
void turns_init(Var*, int);
void make_initial_state();
void make_one_step(Var* , int);
void make_last_step(Var*, int);

int main()
{ 
   read("test",ina,inb);

   for(int x=1;x<=20;x++)
   {
      cerr<<"trying to solve in "<<x<<" steps ... ";
      solver.initialize();

      make_initial_state();

      copy(state1a,inita); copy(state1b,initb);
      
      Var* Turns = new Var[5*x];      
      turns_init(Turns, x);

      for(int y=0;y<x-1;y++)
      {
         make_one_step(Turns, y);           
         copy_2_to_1();
      }

      make_last_step(Turns, x);
      //copy_2_to_1();

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
            cout << solver.getValue(Turns[5*y+0])*16 +
                    solver.getValue(Turns[5*y+1])*8  + 
                    solver.getValue(Turns[5*y+2])*4  +
                    solver.getValue(Turns[5*y+3])*2  + 
                    solver.getValue(Turns[5*y+4]) 
                 << ' ';
         cout<<endl<<endl;
         delete [] Turns;
         return 0;
      }
      delete [] Turns;
   }

    return 0;
}

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

void make_initial_state()
{
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
}

void turns_init(Var* Turns , int x)
{
   for(int y=0; y<x;y++)
   {
      for(int z=0; z<5;z++)
         Turns[5*y+z]=solver.newVar();

      solver.addsp(Turns[5*y+0],Turns[5*y+1]);
      solver.addsp(Turns[5*y+3],Turns[5*y+4]);
      
      if(y!=0)
      {  
         Var tempxor[3]={};
         tempxor[0]=solver.newVar();tempxor[1]=solver.newVar();tempxor[2]=solver.newVar();
         solver.addXorCNF(tempxor[0], Turns[5*y+0], false, Turns[5*(y-1)+0], false);
         solver.addXorCNF(tempxor[1], Turns[5*y+1], false, Turns[5*(y-1)+1], false);
         solver.addXorCNF(tempxor[2], Turns[5*y+2], false, Turns[5*(y-1)+2], false);
         solver.addsp1(tempxor[0],tempxor[1],tempxor[2]);
         
         if(y!=1)
         {
            Var tempxor1[4]={};
            tempxor1[0]=solver.newVar();tempxor1[1]=solver.newVar();tempxor1[2]=solver.newVar();tempxor1[3]=solver.newVar();
            solver.addXorCNF(tempxor1[0], Turns[5*y+0], false, Turns[5*(y-2)+0], false);
            solver.addXorCNF(tempxor1[1], Turns[5*y+1], false, Turns[5*(y-2)+1], false);
            solver.addXorCNF(tempxor1[2], Turns[5*(y-1)+0], false, Turns[5*(y-2)+0], false);
            solver.addXorCNF(tempxor1[3], Turns[5*(y-1)+1], false, Turns[5*(y-2)+1], false);
            solver.addsp2(tempxor1[0],tempxor1[1],tempxor1[2],tempxor1[3],tempxor[0],tempxor[1]);
         }

      }
   }

}

void make_one_step(Var* Turns, int y)
{
   for(int i=0;i<24;i++)
      for(int j=0;j<24;j++)
      { state2a[i][j]=solver.newVar(); state2b[i][j]=solver.newVar(); }

   for(int i=0;i<24;i++)
   {
      for(int j=0;j<24;j++)
      {
         Var tempAnda[24]={}; Var tempAndb[24]={};
         for(int m=0;m<24;m++)
         {
            if(m%4!=3)
            {
               tempAnda[m]=solver.newVar(); tempAndb[m]=solver.newVar(); 
               vector<Var> via ; vector<bool>bia;
               vector<Var> vib ; vector<bool>bib;

               for(int n=0;n<5;n++)
               {
                  via.push_back(Turns[5*y+n]); bia.push_back((m>>(4-n))%2);
                  vib.push_back(Turns[5*y+n]); bib.push_back((m>>(4-n))%2);
               }
               via.push_back(state1a[i][a[m][j]]); 
               vib.push_back(state1b[i][b[m][j]]);

               bia.push_back(1);  bib.push_back(1); 

               solver.addAndCNF(tempAnda[m],via,bia);
               solver.addAndCNF(tempAndb[m],vib,bib);
            }
         }

         vector<Var> via ; vector<Var> vib ;

         for(int m=0;m<24;m++)
         {
           if(m%4!=3){ via.push_back(tempAnda[m]); vib.push_back(tempAndb[m]); }
         }

         solver.addOrCNF(state2a[i][j], via); solver.addOrCNF(state2b[i][j], vib);
      }
   }
}

void make_last_step(Var* Turns, int x)
{
   for(int i=0;i<24;i++)
   { state2a[i][i]=solver.newVar(); state2b[i][i]=solver.newVar(); }
   
   for(int i=0;i<24;i++)
   {      
      Var tempAnda[24]={}; Var tempAndb[24]={};
      for(int m=0;m<24;m++)
      {
         if(m%4!=3)
         {
            tempAnda[m]=solver.newVar(); tempAndb[m]=solver.newVar(); 
            vector<Var> via ; vector<Var> vib ; vector<bool>bia; vector<bool>bib;

            for(int n=0;n<5;n++)
            {
               via.push_back(Turns[5*(x-1)+n]); bia.push_back((m>>(4-n))%2);
               vib.push_back(Turns[5*(x-1)+n]); bib.push_back((m>>(4-n))%2);
            }
            via.push_back(state1a[i][a[m][i]]); 
            vib.push_back(state1b[i][b[m][i]]);

            bia.push_back(1);  bib.push_back(1); 

            solver.addAndCNF(tempAnda[m],via,bia);
            solver.addAndCNF(tempAndb[m],vib,bib);
         }
      }

      vector<Var> via ; vector<Var> vib ;

      for(int m=0;m<24;m++)
      {
         if(m%4!=3)
         {
            via.push_back(tempAnda[m]); vib.push_back(tempAndb[m]); 
         }
      }

      solver.addOrCNF(state2a[i][i], via); solver.addOrCNF(state2b[i][i], vib);               
   }
}

