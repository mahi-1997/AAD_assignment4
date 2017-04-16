#include<string>
#include<fstream>
#include<iostream>
#include<time.h>
#include<algorithm>
#include<vector>
#include<math.h>
#include <stdlib.h>   
using namespace std;

class point{
 private:
        int _x; 
        int _y;  
    public:
        point(int x, int y){ 
            
            _x = x;
            _y = y;
        }
        int getX(){ 
            return _x;
        }
        int getY(){
            return _y;
        }
   
};
//-------------------------------------------------------------------------------------------------
bool comp(point *a, point *b){  //comparator  x-coord
    
    return (a->getX()<b->getX());
}
//--------------------------------------------------------------------------------------------------

int determinant(point *p1, point *p2, point *p3){   //calculate determinant function
    
    int d = p1->getX() * p2->getY() + p3->getX() * p1->getY() + p2->getX() * p3->getY();
    d -= (p3->getX() * p2->getY() + p2->getX() * p1->getY() + p1->getX() * p3->getY());
    return d;
}
//-------------------------------------------------------------------------------------------------

void findPartialHull(vector<point*> &arr, vector<point*> &result, point* p1, point* pn){
    //main function to find a part of the hull; either upper part or lower part
    
    vector<point*> s1 = vector<point*>();   //holds points of a part
    for (vector<point*>::iterator it = arr.begin(); it < arr.end(); it++){  //iterate the vector
        if (determinant(p1,pn,*it) > 0){    //if the point is on the left of p1-->pn line
            s1.push_back(*it);  //put it into S1
        }
    }
    if (s1.empty()){    //if s1 is empty, write p1 to result array; base case
        result.push_back(p1);
    } else {    //still points to look at
        //find pmax
        float maxDist = 0;  //distance to p1-->pn line
        point *pmax;    //hold the pointer of pmax
        float dist; //height
        float length = sqrt(pow(p1->getX() - pn->getX(), 2) + pow(p1->getY() - pn->getY(), 2)); //length of p1-->pn line
        for (vector<point*>::iterator it = s1.begin(); it < s1.end(); it++){
            dist = determinant(p1,pn,*it) / length; //distance(height) = area / length
            if (dist > maxDist){    //find the point with max distance
                maxDist = dist;
                pmax = *it;
            }
        }
        //find points left to p1-->pmax and pmax-->pn
        vector<point*> s11 = vector<point*>();  //holds points left to p1-->pmax
        vector<point*> s12 = vector<point*>();  //holds points left to pmax-->pn
        for (vector<point*>::iterator it = s1.begin(); it < s1.end(); it++){
            //iterate s1 and find all points we are looking for
            if (determinant(p1,pmax,*it) > 0){  //left to p1-->pmax
                s11.push_back(*it);
            } else if (determinant(pmax,pn,*it) > 0){   //left to pmax-->pn
                s12.push_back(*it);
            }
        }
        findPartialHull(s11, result, p1, pmax); //recursively search in s11
        findPartialHull(s12, result, pmax, pn); //recursively search in s12
    }
}
int main(){
    ifstream fi;    
    string filename;    
    int N; 
    vector<point*> parray = vector<point*>();   //holds pointers of raw data
    vector<point*> upper = vector<point*>();    //holds the result of upper convex hull
    vector<point*> lower = vector<point*>();    //holds the result of lower convex hull
    
	
    cout << "Enter total number  of points N "<<endl;
            cin>>N;
     int check;
	 cout<<"Enter\n 1.to check random generated points .\n2.to ckeck any known case\n";
	   cin>>check;
	   if(check==1)  {
	   	 for (int i = 0; i < N; i++){   //read in file
        int x, y;
        x=rand() %500 + (-250);
        y=rand() %500 + (-250);
        point *p = new point(x,y);
        parray.push_back(p);
    } 
	   }  
	   if(check==2){
	   	
	   	 for (int i = 0; i < N; i++){   //read in file
        int x, y;
       cin>> x;
        cin>> y;
        point *p = new point(x,y);
        parray.push_back(p);
    } 
    
	   }
      
	
	    
    float t0 = clock();  //total time start
	t0=t0*1000;
    sort(parray.begin(),parray.end(),comp); //sort raw data; increasing order of x
   
    findPartialHull(parray, upper, parray[0], parray[N-1]);   //upper hull, line direction p1-->pn
    findPartialHull(parray, lower, parray[N-1], parray[0]);   //lower hull, line direction pn-->p1
    
    sort(upper.begin(),upper.end(),comp); //sort result; increasing order of x
    sort(lower.begin(),lower.end(),comp);
    float t3 = clock(); //second sort stop; also total time stop
	t3=t3*1000;
    cout << "The convex hull is made up of the following points:" << endl;
    //iterate and print results
    for (vector<point*>::iterator it = upper.begin(); it < upper.end(); it++){  //upper part, clock-wise
        cout << (*it)->getX() << ", " << (*it)->getY() << endl;
    }
    for (vector<point*>::iterator it = lower.end()-1; it >= lower.begin(); it--){
        //reverse iterating order for lower part; still clock-wise
        cout << (*it)->getX() << ", " << (*it)->getY() << endl;
    }
    cout << "Total time: " << (t3-t0)/CLOCKS_PER_SEC << " milli seconds" << endl;
    
    return 0;
}
