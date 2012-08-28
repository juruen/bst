#include <iostream>
#include <functional>
#include <sstream>

template <class T>
class Tree {
  public:
  struct Node {
    Node(T val) : value(val), left(0), right(0), id(0) {}
    T value;
    Tree<T>::Node *left;
    Tree<T>::Node *right;
    unsigned int id;
  };

  public:
  enum TraversalOrder { PreOrder, InOrder, PostOrder};

  Tree() : m_root(0), m_lastid(0) {};

  void insert(T val) {
    _insert(m_root, val);
  }
  
  void traverse(TraversalOrder order, std::function<void(Tree<T>::Node*)> f) {
    _traverse(m_root, order, f);
  }

  void printTraverse(TraversalOrder order) {
    std::cout << "Traverse: " << std::endl;
    _traverse(
        m_root,
        order,
        std::bind(
          &Tree<T>::print_node,
          this,
          std::placeholders::_1
        )
    );
    std::cout << std::endl;
  }

  private:
  void _insert(Node* &node, T val) {
    if (node == 0) {
      node = new Node(val);
      node->id = ++m_lastid;
    } else if (val < node->value) {
      _insert(node->left, val);
    } else {
      _insert(node->right, val);
    }
  }

  void _traverse(Node* node, TraversalOrder order, std::function<void(Tree<T>::Node*)> f) {
    if (node) {
      if (order == PreOrder) f(node);
      _traverse(node->left, order, f);
      if (order == InOrder) f(node);
      _traverse(node->right, order, f);
      if (order == PostOrder) f(node);
    }
  }

  void print_node(Tree<T>::Node* node) { 
    std::cout << node->value<< std::endl;
  }

  private:
  Node *m_root;
  unsigned int m_lastid;
};

template <class T>
class GraphVizTree {
  public:
  GraphVizTree(Tree<T>* tree) : m_tree(tree) {};

  void printDotNode(typename Tree<T>::Node* node) {
    vertex(node);
    if (node->left) {
      edgeStr(node, node->left, 0);
    }
    if (node->right) {
      edgeStr(node, node->right, 2);
    }
  }

  void vertex(typename Tree<T>::Node* node) {
    std::ostringstream oss;
    oss << "  node" << node->id << " [ label =\"<f0> | <f1> "<< node->value << " | <f2> \"];\n";
    m_edges += oss.str();
  }

  void edgeStr(typename Tree<T>::Node* parent, typename Tree<T>::Node* child, unsigned int f) {
    std::ostringstream oss;
    oss << "  \"node" << parent->id << "\":f" << f << " -> \"node" << child->id << "\":f1 ;\n";
    m_vertices += oss.str();
  }

  void generateDot() {
    m_tree->traverse(
        Tree<T>::TraversalOrder::PreOrder,
        std::bind(
          &GraphVizTree::printDotNode,
          this,
          std::placeholders::_1
        )
    );
    std::cout
      << 
      "digraph G\n" 
      "{\n"
      "  node [shape = record];\n"
      <<
      m_edges
      <<
      "\n\n"
      << 
      m_vertices
      <<
      "}\n"
    ;
  }

  private:
  Tree<T>* m_tree;
  std::string m_edges;
  std::string m_vertices;
};

int main() {
  Tree<int> t;
  t.insert(3);
  t.insert(5);
  t.insert(7);
  t.insert(4);
  t.insert(9);

  t.printTraverse(Tree<int>::InOrder);
  t.printTraverse(Tree<int>::PreOrder);
  t.printTraverse(Tree<int>::PostOrder);

  GraphVizTree<int> graphViz(&t);
  graphViz.generateDot();
}