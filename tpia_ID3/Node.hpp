/* 
 * File:   Node.hpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 *
 * Created on September 22, 2011, 2:38 PM
 */

#ifndef NODE_HPP
#define	NODE_HPP

#include <map>

class ANode
{
public:
    virtual ~ANode() {}
};

template <typename Specialized>
class   Node : public ANode {
    public:
        typedef std::map<Specialized, ANode *> ChildNode;
        
        Node() {}
        
        ~Node(){}
        
        void AddChild(Specialized const & value, ANode * child)
        {
            _childs[value] = child;
        }
        
        ChildNode & getChilds()
        {
            return (_childs);
        }
        
        
    private:
        ChildNode _childs;
};

#endif	/* NODE_HPP */

