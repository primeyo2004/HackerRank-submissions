// 
// Author:            Jeune Prime Origines <primeyo2004@yahoo.com>
// Challenge solved:  https://www.hackerrank.com/challenges/ctci-find-the-running-median
//
#include <iostream>
#include <algorithm>
#include <queue>
#include <iomanip>

// The only reason this is used is so we  can access the protected member c.reserve()
// When dealing with huge sizes, pre-allocation for array has a huge efficiency impact 
template <typename Compare>
class reserved_priority_queue : public std::priority_queue< int , std::vector<int>, Compare > {
    public:
        reserved_priority_queue(size_t capacity){
            this->c.reserve(capacity);
        }
};

// Summary:
//  - using a combined 2 heap data structures (q_max, and q_min )
//  - q_min is a min heap, but should contain all possible values >= media
//  - q_max is a max heap, but should contain all possible values <= median
//  - complexity should be O (log  n/n )  
//
class median_container{
   public:
       median_container(size_t capacity)
           :q_min(capacity),
            q_max(capacity){
       }

       void add(int val){
           if (is_empty()) { q_min.push(val); }
           else {
               if (static_cast<double>(val) <= get_median()){ q_max.push(val); }
               else{ q_min.push(val); }
           }
           normalize();
       };

       double get_median(){

           if (is_empty())                         { return static_cast<double>(0); }
           else if (q_max.size() == q_min.size())  { return (static_cast<double>(q_max.top()) +  static_cast<double>(q_min.top()) ) * 0.5; }
           else if ( q_max.size() > q_min.size() ) { return static_cast<double>(q_max.top()); }
           else                                    { return static_cast<double>(q_min.top()); }
       }

   protected:
   void normalize(){

       int diff = q_max.size() - q_min.size();

       if ( diff > 1 )     { q_min.push ( q_max.top() ); q_max.pop(); }
       else if (diff < -1) { q_max.push (q_min.top() ); q_min.pop(); }
   }

   bool is_empty(){
       return (q_max.size() == 0 && q_min.size() == 0);
   }

   private:
   reserved_priority_queue< std::greater<int> > q_min;
   reserved_priority_queue< std::less<int> >    q_max;
};

int main(){

    // input the number of elements from the console
    int size;
    std::cin >> size;

    median_container mid(size/2 + 1);

    for (int i = 0; i < size; i++){
        int val;
        // input each element
        std::cin >> val;
        mid.add(val);
        // display the running median
        std::cout << std::fixed << std::setprecision(1) << mid.get_median() << std::endl;
    }
}
