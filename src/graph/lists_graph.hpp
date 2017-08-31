#ifndef _LISTS_GRAPH_HPP
#define _LISTS_GRAPH_HPP

#include "graph.hpp"
#include <vector>
#include <list>

class GraphAsLists : public Graph
{
  //pamiec grafu
  std::vector<Vertex *> vertices;
  std::vector< std::list<Edge *> > adjacencyLists;

  void delete_all(); //pomocnicza funkcja kasujaca
  Edge * find(int u, int v); //szukanie krawedzi
  
  //klasa iteratora po wszystkich wierzcholkach
  class AllVerticesIter : public Iterator<Vertex>{
    GraphAsLists & owner;
    int current;
  public:
    AllVerticesIter(GraphAsLists & owner) : owner(owner), current(0){ }
    virtual bool IsDone();
    virtual Vertex & operator*();
    virtual void operator++();
  };

  //klasa iteratora po wszystkich krawedziach
  class AllEdgesIter : public Iterator<Edge>{
    GraphAsLists & owner;
    std::list<Edge *>::iterator it, itEnd;
    void next();
    bool AtEnd;
  public:
    int current;
    AllEdgesIter(GraphAsLists & owner);
    virtual bool IsDone();
    virtual Edge & operator*();
    virtual void operator++();
  };

  //klasa iteratora po krawedziach wychodzacych
  class EmanEdgesIter : public Iterator<Edge>{
    GraphAsLists & owner;
    bool AtEnd;
    std::list<Edge *>::iterator it, itEnd;
  public:
    EmanEdgesIter(GraphAsLists & owner, int v);
    virtual bool IsDone();
    virtual Edge & operator*();
    virtual void operator++();
  };

  //klasa iteratora po krawedziach wchodzacych
  class InciEdgesIter : public Iterator<Edge>{
    GraphAsLists & owner;
    AllEdgesIter it;
    int v;
    bool AtEnd;
    void next();
  public:
    InciEdgesIter(GraphAsLists & owner, int v) : owner(owner), it(owner), v(v), AtEnd(false){
      next();
    }
    virtual bool IsDone();
    virtual Edge & operator*();
    virtual void operator++();
  };

public:
  //METODY
  GraphAsLists(int n = 0);
  virtual ~GraphAsLists();
  virtual void MakeNull();
  virtual Vertex & AddVertex();
  virtual Vertex & SelectVertex(int number);
  virtual Edge & AddEdge(int u, int v);
  virtual bool IsEdge(int u, int v);
  virtual Edge & SelectEdge(int u, int v);
  virtual Iterator<Vertex> & Vertices();
  virtual Iterator<Edge> & Edges();
  virtual Iterator<Edge> & EmanatingEdges(int v);
  virtual Iterator<Edge> & IncidentEdges(int v);
};


class DigraphAsLists : public GraphAsLists
{
public:
    DigraphAsLists(int n = 0) : GraphAsLists(n)
    {
        directed = true;
    }
};

#endif
