#include <iostream>
#include <string.h>
#include <vector>

#include <boost/operators.hpp>
class Node : public boost::less_than_comparable<Node> {
public:
    Node(int v) :val(v) {}
    
//private:
    int val;
}; 
inline bool operator< (const Node& lhs, const Node& rhs) {
    return lhs.val <  rhs.val;
}
int main() {
    Node n2(2), n1(1);
    if (n2 > n1) {
        std::cout << "success" << std:: endl; 
    }
    return 0;
}