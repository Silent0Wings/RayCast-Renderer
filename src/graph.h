#ifndef GRAPH_H
#define GRAPH_H

#include "graphNode.h"
#include "object.h"


/**
 * @class graph
 * @brief describes a graph
 **/
class graph
{
public:
graphNode* root;
graph() = default;
vector<vector<tuple<graphNode, object>>> gridNode;
size_t Height;
size_t Width;
graph(const size_t height,const size_t width)
{
    if(width ==0 || height==0)
        throw std::invalid_argument("Width and height must be non-zero");
    
    Height=height;
    Width=width;
    // creating the graph like structure
    int increment=0;
    for (size_t i = 0 ; i < height ; i++)
    {
        vector<tuple<graphNode, object>> temprow;
        for (size_t z = 0 ; z < width ; z++)
        {
            graphNode tempNode;
            tempNode.value=increment;
            tempNode.index={i,z};
            temprow.push_back(std::make_tuple(tempNode, object{}));
        }   
        increment++;
        gridNode.push_back(temprow);
    }

    for (size_t i = 0 ; i < gridNode.size() ; i++)
    {
        for (size_t z = 0 ; z < gridNode.at(i).size() ; z++)
        {
            if(graphConstraing(i+1,z,width,height))
            {
                get<0>(gridNode.at(i).at(z)).children.push_back(&get<0>(gridNode.at(i+1).at(z)));
            }
             if(graphConstraing(i-1,z,width,height))
            {
                get<0>(gridNode.at(i).at(z)).children.push_back(&get<0>(gridNode.at(i-1).at(z)));
            }
             if(graphConstraing(i,z+1,width,height))
            {
                get<0>(gridNode.at(i).at(z)).children.push_back(&get<0>(gridNode.at(i).at(z+1)));
            }
             if(graphConstraing(i,z-1,width,height))
            {
                get<0>(gridNode.at(i).at(z)).children.push_back(&get<0>(gridNode.at(i).at(z-1)));
            }
        }   
    }
    root =&(get<0>(gridNode.at(0).at(0)));;
    get<0>(gridNode.at(height-1).at(width-1)).goal = true;
}

static bool graphConstraing(int x, int y, size_t height , size_t width)
{
    return !(x < 0 || x >= (int)width || y < 0 || y >= (int)height);
}


//vector<graphNode*> stack;
vector<graphNode*> path ={root};

bool step_dfs(int size)
{
    static vector<graphNode*> stack={root};
    
    //stack.push_back(root);
    graphNode* current=stack.at(stack.size()-1);
    // for(size_t i=0;i<Height*Width;i++)
    for(size_t i=0;i<size;i++)
    {
        current=stack.at(stack.size()-1);
        graphNode* next=nullptr;
        size_t the_x=current->index[0];
            size_t the_y=current->index[1];  

        if(current->goal)
        {
            get<1>(gridNode[the_x][the_y]).setColor(color(0,255,0));
            return 1;
        }

        for (const auto& child : current->children) {   
            
            if(child->explored==false)
            {
                next=child;
                break;
            }
        }
        if(next!=nullptr)
        {
            stack.push_back(next);
            next->explored=true;
             
            get<1>(gridNode[the_x][the_y]).setColor(color(0,0,255));

        }else
        {
            stack.erase(stack.begin() + stack.size()-1);
        }
    }
    return false;
}


    
};
#endif // GRAPH_H