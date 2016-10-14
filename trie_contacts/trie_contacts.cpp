// 
// Author:                 Jeune Prime Origines <primeyo2004@yahoo.com>
// Challenge solved:       https://www.hackerrank.com/challenges/ctci-contacts 
// Compiler  Requirement:  C++14
#include <map>
#include <list>
#include <string>
#include <iostream>
#include <algorithm>
#include <memory>
#include <cctype>
 
// Summary:
//  trie_contact 
//    - add(name)  - allows adding of word into the trie data structure
//    - find(partial) - find the partial (prefix) string from the trie contacts and return the number of partial word match     
//    - Uses iterative approach (as opposed to recursive), this should handle longer names without worrying about stack overflow issues.
//    - trie_contact::trie_node 
//       - uses std::map to dynamically store the child nodes (hence range of ascii char set)
//
class trie_contacts {
    protected:    
    class trie_node {
        public:
        trie_node () : _child_word_count (0),_complete_word{false} { }

        size_t get_child_word_count()  const  { return _child_word_count;  }
        bool   is_complete_word()      const  { return _complete_word;  }

        void  inc_child_word_count() { _child_word_count++; }
        void  set_complete_word ()   { _complete_word = true; }


        auto  add  (std::string::const_iterator itr) {
           auto search_itr = _children.find(*itr);
           if (search_itr  == _children.end() ){
              auto newitem_pair =  _children.emplace (*itr, std::make_unique <trie_node> ());
              return std::make_pair ( newitem_pair.first->second.get(), ++itr );
           }
           else{
               return std::make_pair ( search_itr->second.get(),++itr );
           }
        }

        auto  find (std::string::const_iterator itr) {
           auto search_itr = _children.find(*itr);
           if (search_itr  == _children.end() ){
              return std::make_pair ( static_cast<trie_node*>(nullptr), ++itr );
           }
           else{
               return std::make_pair ( search_itr->second.get(),++itr );
           }
        }

     //void dump(const std::string& label) {
     //    std::cout << label.c_str() << " this               :" << static_cast<void*>(this) << std::endl;
     //    std::cout << label.c_str() << " _child_word_count  :" << this->get_child_word_count() << std::endl;
     //    std::cout << label.c_str() << " _complete_word     :" << this->is_complete_word() << std::endl;
     //}

    private:

        std::map<char, std::unique_ptr < trie_node > > _children;
        size_t  _child_word_count;
        bool   _complete_word;

    };

public:
    trie_contacts(): _root ( std::make_unique<trie_node>() ) {
    }

    void add(const std::string& name){

        auto itr = name.cbegin();
        if ( itr != name.cend() ){
            std::list<trie_node*>  traversed_nodes;
            auto add_pair = _root->add (itr);
            while (add_pair.first != nullptr && add_pair.second != name.cend() ){
                // remember the nodes to update the count once a new unique word is found;
                traversed_nodes.push_back (add_pair.first);
                add_pair = add_pair.first->add ( add_pair.second );
            }

            
            if (add_pair.first != nullptr && !add_pair.first->is_complete_word() ){
                // Mark each unique word found (node is not an end of a complete word ) 
                add_pair.first->set_complete_word();
                add_pair.first->inc_child_word_count();

                // update the counter of all the parent nodes traversed so it will hold the number of partial words it contain
                // without the need to traverse again the children
                std::for_each(traversed_nodes.begin(),traversed_nodes.end(),[](trie_node* t){ t->inc_child_word_count();  });
            }
        }
    }

    size_t find (const std::string& partial){
        auto itr = partial.cbegin();
        if ( itr != partial.cend() ){
            auto find_pair = _root->find (itr);
            while ( find_pair.first != nullptr && find_pair.second != partial.cend() ){
                find_pair = find_pair.first->add ( find_pair.second );
            }

            if (find_pair.first != nullptr){
                // no sweat, we already have kept the count of partial words from this node
                // during the add()
                return find_pair.first->get_child_word_count();
            }
        }

        return 0;
    }

protected:
    std::unique_ptr<trie_node>  _root;
};


int main(){

   trie_contacts contacts;
   std::string   add_command("add");
   std::string   find_command("find");

   int  count;
   std::string command,param;
   std::cin >> count;
   
   for (int i = 0; i < count; i++ ){
      std::cin >> command >> param;
      
      if (command.size() == add_command.size() &&
           std::equal(add_command.cbegin(),add_command.cend(),
                      command.cbegin(),[](char a, char b) { return std::toupper(a) == std::toupper(b); } )) {
                      
          contacts.add (param);     
                      
      }
      else if (command.size() == find_command.size() &&
           std::equal(find_command.cbegin(),find_command.cend(),
                      command.cbegin(),[] (char a, char b) { return std::toupper(a) == std::toupper(b); }  )) {
                      
          std::cout << contacts.find(param) << std::endl;           
                      
      }
      
   }

   return 0;

}
