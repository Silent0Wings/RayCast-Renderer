#ifndef GRAPH_H
#define GRAPH_H

#include "graphNode.h"
#include "object.h"
#include <queue>  // For std::priority_queue
#include <tuple>  // For std::tuple
#include <vector> // For std::vector (used internally)
#include <random>

/**
 * @class graph
 * @brief describes a graph
 **/
class graph
{
public:
    graphNode *root;
    graphNode *goal;

    graph() = default;
    vector<vector<tuple<graphNode, object>>> gridNode;
    size_t Height;
    size_t Width;

    vector<vector<int>> directions = {
        {1, 0},  // down
        {-1, 0}, // up
        {0, 1},  // right
        {0, -1}, // left

    };
    vector<vector<int>> full_directions = {
        {1, 1},   // down-right
        {-1, -1}, // up-left
        {1, -1},  // down-left
        {-1, 1},  // up-right
        {1, 0},   // down
        {-1, 0},  // up
        {0, 1},   // right
        {0, -1}   // left
    };

    graph(const size_t height, const size_t width)
    {
        if (width == 0 || height == 0)
            throw std::invalid_argument("Width and height must be non-zero");

        Height = height;
        Width = width;
        // creating the graph like structure
        int increment = 0;
        for (size_t i = 0; i < height; i++)
        {
            vector<tuple<graphNode, object>> temprow;
            for (size_t z = 0; z < width; z++)
            {
                graphNode tempNode;
                tempNode.value = increment;
                tempNode.index = {i, z};
                temprow.push_back(std::make_tuple(tempNode, object{}));
            }
            increment++;
            gridNode.push_back(temprow);
        }

        for (size_t i = 0; i < gridNode.size(); i++)
        {
            for (size_t z = 0; z < gridNode.at(i).size(); z++)
            {
                for (const auto &direct : directions)
                {
                    if (graphConstraing(i + direct[0], z + direct[1], height, Width))
                    {
                        size_t r = std::rand() % 100; // 0–99
                        get<0>(gridNode.at(i).at(z)).children.push_back(std::make_tuple(&get<0>(gridNode.at(i + direct[0]).at(z + direct[1])), r + i + z));
                    }
                }
            }
        }
        root = &(get<0>(gridNode.at(0).at(0)));
        root->explored = true;
        get<0>(gridNode.at(height - 1).at(width - 1))
            .goal = true;
        goal = &get<0>(gridNode.at(height - 1).at(width - 1));
    }

    static bool graphConstraing(int x, int y, size_t height, size_t width)
    {
        return !(x < 0 || x >= (int)height || y < 0 || y >= (int)width);
    }

    void print_connections()
    {
        for (const auto &row : gridNode)
        {
            for (const auto &cell : row)
            {
                graphNode node = std::get<0>(cell); // Pointer to graphNode

                for (const auto &child : node.children)
                {
                    // Print connection
                    cout << "(" << node.index[0] << "," << node.index[1] << ")"
                         << " -> "
                         << "(" << get<0>(child)->index[0] << "," << get<0>(child)->index[1]
                         << "| weight :" << get<1>(child) << ")\n";
                }
            }
        }
    }

    // vector<graphNode*> stack;
    vector<graphNode *> path = {root};

    bool step_dfs(int size)
    {
        static vector<graphNode *> stack = {root};

        // stack.push_back(root);
        graphNode *current = stack.at(stack.size() - 1);
        // for(size_t i=0;i<Height*Width;i++)
        for (size_t i = 0; i < (size_t)size; i++)
        {
            current = stack.at(stack.size() - 1);
            graphNode *next = nullptr;
            size_t the_x = current->index[0];
            size_t the_y = current->index[1];

            if (current->goal)
            {
                get<1>(gridNode[the_x][the_y]).setColor(color(0, 255, 0));
                return 1;
            }

            for (const auto &child : current->children)
            {

                if (get<0>(child)->explored == false)
                {
                    next = get<0>(child);
                    break;
                }
            }
            if (next != nullptr)
            {
                stack.push_back(next);
                next->parent = current;
                next->explored = true;

                get<1>(gridNode[the_x][the_y]).setColor(color(0, 0, 255));
            }
            else
            {
                stack.erase(stack.begin() + stack.size() - 1);
            }
        }
        return false;
    }

    void bfs()
    {
        static vector<graphNode *> queue = {root};
        // stack.push_back(root);
        graphNode *current = queue.at(0);
        // for(size_t i=0;i<Height*Width;i++)
        for (size_t i = 0; i < Width * Height; i++)
        {
            current = queue.at(0);
            size_t the_x = current->index[0];
            size_t the_y = current->index[1];

            if (current->goal)
            {
                get<1>(gridNode[the_x][the_y]).setColor(color(0, 255, 0));
                return;
            }
            else
            {
                get<1>(gridNode[the_x][the_y]).setColor(color(0, 0, 255));
            }

            for (const auto &child : current->children)
            {
                if (get<0>(child)->explored == false)
                {
                    queue.push_back(get<0>(child));
                    get<0>(child)->parent = current;
                    get<0>(child)->explored = true;
                    get<1>(gridNode[get<0>(child)->index[0]][get<0>(child)->index[1]]).setColor(color(0, 125, 125));
                }
            }
            queue.erase(queue.begin());
        }
    }

    bool step_bfs()
    {
        static vector<graphNode *> queue = {root};
        // stack.push_back(roaot);
        graphNode *current = queue.at(0);
        // for(size_t i=0;i<Height*Width;i++)
        for (size_t i = 0; i < 1; i++)
        {
            current = queue.at(0);
            queue.erase(queue.begin());

            size_t the_x = current->index[0];
            size_t the_y = current->index[1];

            if (current->goal)
            {
                get<1>(gridNode[the_x][the_y]).setColor(color(0, 255, 0));
                return true;
            }
            else
            {
                get<1>(gridNode[the_x][the_y]).setColor(color(0, 0, 255));
            }

            for (const auto &child : current->children)
            {
                if (get<0>(child)->explored == false)
                {
                    queue.push_back(get<0>(child));
                    get<0>(child)->parent = current;
                    get<0>(child)->explored = true;
                    get<1>(gridNode[get<0>(child)->index[0]][get<0>(child)->index[1]]).setColor(color(0, 125, 125));
                }
                else
                {
                    continue;
                }
            }
        }
        return false;
    }

    void dfs()
    {
        static vector<graphNode *> stack = {root};

        // stack.push_back(root);
        graphNode *current = stack.at(stack.size() - 1);
        // for(size_t i=0;i<Height*Width;i++)
        for (size_t i = 0; i < Width * Height; i++)
        {
            current = stack.at(stack.size() - 1);
            graphNode *next = nullptr;
            size_t the_x = current->index[0];
            size_t the_y = current->index[1];

            if (current->goal)
            {
                get<1>(gridNode[the_x][the_y]).setColor(color(0, 255, 0));
                return;
            }

            for (const auto &child : current->children)
            {

                if (get<0>(child)->explored == false)
                {
                    next = get<0>(child);
                    break;
                }
            }
            if (next != nullptr)
            {
                stack.push_back(next);
                next->parent = current;
                next->explored = true;

                get<1>(gridNode[the_x][the_y]).setColor(color(0, 0, 255));
            }
            else
            {
                stack.erase(stack.begin() + stack.size() - 1);
            }
        }
    }

    void trace_path()
    {
        if (goal == nullptr)
            return;
        graphNode *current = goal->parent;
        for (size_t i = 0; i < Width * Height; i++)
        {
            if (current == nullptr)
                return;
            size_t the_x = current->index[0];
            size_t the_y = current->index[1];
            get<1>(gridNode[the_x][the_y]).setColor(color(0, 255, 255));
            current = current->parent;
        }
    }

    bool step_Trace_Path()
    {
        if (goal == nullptr)
            return 1;
        static graphNode *current = goal->parent;

        if (current == nullptr)
            return 1;
        size_t the_x = current->index[0];
        size_t the_y = current->index[1];
        get<1>(gridNode[the_x][the_y]).setColor(color(0, 255, 255));
        current = current->parent;
        if (current == root)
            return true;

        return false;
    }

    std::vector<object> getObjects()
    {
        std::vector<object> allObjects;
        for (const auto &row : gridNode)
        {
            for (const auto &cell : row)
            {
                allObjects.push_back(std::get<1>(cell));
            }
        }
        return allObjects;
    }

    void greedyBestFirstSearch()
    {
        // c++ priority queue
        using NodeEntry = std::tuple<graphNode *, size_t>;

        struct Compare
        {
            bool operator()(const NodeEntry &a, const NodeEntry &b) const
            {
                return std::get<1>(a) > std::get<1>(b); // Min-heap by size_t
            }
        };

        static std::priority_queue<NodeEntry, std::vector<NodeEntry>, Compare> pq;

        static bool initialized = false;
        if (!initialized)
        {
            pq.push({root, 0});
            initialized = true;
        }

        // stack.push_back(root);
        graphNode *current = nullptr;
        // for(size_t i=0;i<Height*Width;i++)
        for (size_t i = 0; i < Height * Width; i++)
        {
            current = std::get<0>(pq.top());
            pq.pop();
            size_t the_x = current->index[0];
            size_t the_y = current->index[1];

            if (current->goal)
            {
                get<1>(gridNode[the_x][the_y]).setColor(color(0, 255, 0));
                return;
            }
            else
            {
                for (const auto &child : current->children)
                {
                    if (get<0>(child)->explored == false)
                    {
                        get<0>(child)->explored = true;
                        get<0>(child)->parent = current;
                        get<1>(gridNode[the_x][the_y]).setColor(color(0, 0, 255));
                        pq.push(child);
                    }
                }
            }
        }
        return;
    }

    bool stepGreedyBestFirstSearch()
    {
        // c++ priority queue
        using NodeEntry = std::tuple<graphNode *, size_t>;

        struct Compare
        {
            bool operator()(const NodeEntry &a, const NodeEntry &b) const
            {
                return std::get<1>(a) > std::get<1>(b); //  smallest size_t at top
            }
        };
        static std::priority_queue<NodeEntry, std::vector<NodeEntry>, Compare> pq;

        static bool initialized = false;
        if (!initialized)
        {
            pq.push({root, 0});
            initialized = true;
        }

        // stack.push_back(root);
        graphNode *current = nullptr;
        // for(size_t i=0;i<Height*Width;i++)
        for (size_t i = 0; i < 1; i++)
        {
            current = std::get<0>(pq.top());
            pq.pop();
            size_t the_x = current->index[0];
            size_t the_y = current->index[1];
            get<1>(gridNode[the_x][the_y]).setColor(color(74, 74, 74));

            if (current->goal)
            {
                get<1>(gridNode[the_x][the_y]).setColor(color(0, 255, 0));
                return true;
            }
            else
            {
                for (const auto &child : current->children)
                {
                    if (get<0>(child)->explored == false)
                    {
                        get<0>(child)->explored = true;
                        get<0>(child)->parent = current;
                        get<1>(gridNode[the_x][the_y]).setColor(color(0, 0, 255));
                        pq.push(child);
                    }
                }
            }
        }

        /*
        auto copy = pq;  // make a copy
         while (!copy.empty()) {
             std::cout << std::get<0>(copy.top()) << " | " << std::get<1>(copy.top()) << '\n';
             copy.pop();
         }
         cout << endl<< "______"<< endl;

         */
        return false;
    }

    void unifiedCostSearch()
    {

        // c++ priority queue
        using NodeEntry = std::tuple<graphNode *, size_t>;

        struct Compare
        {
            bool operator()(const NodeEntry &a, const NodeEntry &b) const
            {
                return std::get<1>(a) > std::get<1>(b); // Min-heap by size_t
            }
        };

        static std::priority_queue<NodeEntry, std::vector<NodeEntry>, Compare> pq;

        static bool initialized = false;
        if (!initialized)
        {
            pq.push({root, 0});
            initialized = true;
        }

        // stack.push_back(root);
        graphNode *current = nullptr;
        // for(size_t i=0;i<Height*Width;i++)
        for (size_t i = 0; i < Height * Width; i++)
        {
            current = std::get<0>(pq.top());
            size_t weight = std::get<1>(pq.top());
            pq.pop();
            size_t the_x = current->index[0];
            size_t the_y = current->index[1];

            if (current->goal)
            {
                get<1>(gridNode[the_x][the_y]).setColor(color(0, 255, 0));
                return;
            }
            else
            {
                for (const auto &child : current->children)
                {
                    if (get<0>(child)->explored == false)
                    {
                        get<0>(child)->explored = true;
                        get<0>(child)->parent = current;
                        get<1>(gridNode[the_x][the_y]).setColor(color(0, 0, 255));
                        pq.push({std::get<0>(child), std::get<1>(child) + weight});
                    }
                }
            }
        }
        return;
    }

    bool stepUnifiedCostSearch()
    {

        // c++ priority queue
        using NodeEntry = std::tuple<graphNode *, size_t>;

        struct Compare
        {
            bool operator()(const NodeEntry &a, const NodeEntry &b) const
            {
                return std::get<1>(a) > std::get<1>(b); // Min-heap by size_t
            }
        };

        static std::priority_queue<NodeEntry, std::vector<NodeEntry>, Compare> pq;

        static bool initialized = false;
        if (!initialized)
        {
            pq.push({root, 0});
            initialized = true;
        }

        // stack.push_back(root);
        graphNode *current = nullptr;
        // for(size_t i=0;i<Height*Width;i++)
        for (size_t i = 0; i < 1; i++)
        {
            current = std::get<0>(pq.top());
            size_t weight = std::get<1>(pq.top());
            pq.pop();
            size_t the_x = current->index[0];
            size_t the_y = current->index[1];

            if (current->goal)
            {
                get<1>(gridNode[the_x][the_y]).setColor(color(0, 255, 0));
                return true;
            }
            else
            {
                for (const auto &child : current->children)
                {
                    if (get<0>(child)->explored == false)
                    {
                        get<0>(child)->explored = true;
                        get<0>(child)->parent = current;
                        get<1>(gridNode[the_x][the_y]).setColor(color(0, 0, 255));
                        pq.push({std::get<0>(child), std::get<1>(child) + weight});
                    }
                }
            }
        }
        return false;
    }

    size_t heuristic(size_t x, size_t y)
    {
        size_t the_x = goal->index[0];
        size_t the_y = goal->index[1];
        get<1>(gridNode[the_x][the_y]).center;

        return gmath::distance(get<1>(gridNode[the_x][the_y]).center, get<1>(gridNode[x][y]).center);
    }
    void aStar()
    {

        // c++ priority queue
        using NodeEntry = std::tuple<graphNode *, size_t>;

        struct Compare
        {
            bool operator()(const NodeEntry &a, const NodeEntry &b) const
            {
                return std::get<1>(a) > std::get<1>(b); // Min-heap by size_t
            }
        };

        static std::priority_queue<NodeEntry, std::vector<NodeEntry>, Compare> pq;

        static bool initialized = false;
        if (!initialized)
        {
            pq.push({root, 0});
            initialized = true;
        }

        // stack.push_back(root);
        graphNode *current = nullptr;
        // for(size_t i=0;i<Height*Width;i++)
        for (size_t i = 0; i < Height * Width; i++)
        {
            current = std::get<0>(pq.top());
            size_t weight = std::get<1>(pq.top());
            pq.pop();
            size_t the_x = current->index[0];
            size_t the_y = current->index[1];

            if (current->goal)
            {
                get<1>(gridNode[the_x][the_y]).setColor(color(0, 255, 0));
                return;
            }
            else
            {
                for (const auto &child : current->children)
                {
                    if (get<0>(child)->explored == false)
                    {
                        get<0>(child)->explored = true;
                        get<0>(child)->parent = current;
                        get<1>(gridNode[the_x][the_y]).setColor(color(0, 0, 255));
                        pq.push({std::get<0>(child), std::get<1>(child) + weight + heuristic(the_x, the_y)});
                    }
                }
            }
        }
        return;
    }
};
#endif // GRAPH_H
