#ifndef _MATRIX_GRAPH_HPP
#define _MATRIX_GRAPH_HPP

#include "graph.hpp"
#include <vector>


class GraphAsMatrix : public Graph
{
    std::vector<Vertex *> vertices;
    std::vector< std::vector<Edge *> > adjacencyMatrix;

    void delete_all();

    class AllVerticesIter : public Iterator<Vertex>
    {
        GraphAsMatrix & owner;
        int current;

    public:
        AllVerticesIter(GraphAsMatrix & owner) : owner(owner), current(0)
        { }

        virtual bool IsDone();
        virtual Vertex & operator*();
        virtual void operator++();
    };

    class AllEdgesIter : public Iterator<Edge>
    {
        GraphAsMatrix & owner;
        int row, col;

        void next();

    public:
        AllEdgesIter(GraphAsMatrix & owner) : owner(owner), row(0), col(-1)
        {
            next();
        }

        virtual bool IsDone();
        virtual Edge & operator*();
        virtual void operator++();
    };

    class EmanEdgesIter : public Iterator<Edge>
    {
        GraphAsMatrix & owner;
        int row, col;

        void next();

    public:
        EmanEdgesIter(GraphAsMatrix & owner, int row)
        : owner(owner), row(row), col(-1)
        {
            next();
        }

        virtual bool IsDone();
        virtual Edge & operator*();
        virtual void operator++();
    };

    class InciEdgesIter : public Iterator<Edge>
    {
        GraphAsMatrix & owner;
        int row, col;

        void next();

    public:
        InciEdgesIter(GraphAsMatrix & owner, int col)
        : owner(owner), row(-1), col(col)
        {
            next();
        }

        virtual bool IsDone();
        virtual Edge & operator*();
        virtual void operator++();
    };

    friend class AllVerticesIter;
    friend class AllEdgesIter;
    friend class EmanEdgesIter;
    friend class InciEdgesIter;

public:
    GraphAsMatrix(int n = 0);
    virtual ~GraphAsMatrix();
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


class DigraphAsMatrix : public GraphAsMatrix
{
public:
    DigraphAsMatrix(int n = 0) : GraphAsMatrix(n)
    {
        directed = true;
    }
};

#endif
