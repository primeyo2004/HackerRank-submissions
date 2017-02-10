// 
// // Author:            Jeune Prime Origines <primeyo2004@yahoo.com>
// // Challenge solved:  https://www.hackerrank.com/challenges/crush 
// // Compiler:          C++14
//
#include <iostream>
#include <algorithm>
#include <map>
using namespace std;

class Delta{
public:
   Delta(): start(0),value(0){}
   Delta(long s,long v): start(s),value(v){}
   void Add(Delta& d)
   {
      start = d.start;
      value += d.value;    
   }
    
   void Sum(long& sumVal){
       sumVal += value;
   } 

   long start;
   long value;   
};

typedef std::map<unsigned long,Delta> DeltaMap;


int main() {
    
    DeltaMap m;
    unsigned long N,M;
    
    cin >> N >> M;
    
    while(M--)
    {
        // build the deltas on a map
        unsigned long a,b,k;
        
        cin >> a >> b >> k;
        
        Delta dPos(a-1,k);
        Delta dNeg(b,b == N ? 0:-k);
        
        m[a-1].Add(dPos);
        m[b].Add(dNeg);
          
    }
    
    // find the max from the running sum
    long maxVal = 0;
    long runningSum = 0;
    std::for_each(m.begin(),m.end(),[&](std::pair<const unsigned long,Delta>& a){
            
            a.second.Sum(runningSum);
            if (maxVal < runningSum)
            {
                maxVal = runningSum;    
            }
            
            });
    
    cout << maxVal;
    
    return 0;
}
