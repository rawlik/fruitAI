#ifndef _GRAPH_HPP
#define _GRAPH_HPP

#include "iterator.hpp"
#include <string>

class Graph;    // deklaracja zapowiadająca


// Wierzchołek grafu.
// Ma numer (tylko do odczytu), etykietę i wagę.

class Vertex
{
    friend class Graph;

protected:
    Vertex(int number) : number(number)
    { }

    int number;

public:
    std::string label;
    int x;
    int y;
    int points;
    int op_dist;

    int Number()
    {
        return number;
    }
};


// Krawędź grafu (wszystko jedno, skierowanego czy nie).
// Ma referencje do wierzchołków (tylko do odczytu), etykietę i wagę.

class Edge
{
    friend class Graph;

protected:
    Vertex & v0;
    Vertex & v1;

    Edge(Vertex & v0, Vertex & v1) : v0(v0), v1(v1)
    { }

public:
    std::string label;
    int weight;

    Vertex & V0()
    {
        return v0;
    }

    Vertex & V1()
    {
        return v1;
    }

    Vertex & Mate(int v)
    {
        return v == v0.Number() ? v1 : v0;

        // brak zabezpieczenia przed sytuacją, w której argument v nie jest
        // równy ani v0, ani v1
    }

    Vertex & Mate(Vertex & v)
    {
        return Mate(v.Number());
    }
};


// Graf, a dokładniej abstrakcyjna klasa bazowa wszystkich grafów.
// Implementuje kilka prostych metod, tych, które nie będą się
// zmieniać w zależności od wybranej reprezentacji.

class Graph
{
protected:
    int numberOfVertices;
    int numberOfEdges;
    bool directed;

    Vertex * newVertex(int i)
    {
        return new Vertex(i);
    }

    Edge * newEdge(Vertex & u, Vertex & v)
    {
        return new Edge(u, v);
    }

public:
    Graph(int n = 0) : numberOfVertices(n), numberOfEdges(0), directed(false)
    { }

    // będziemy tworzyli klasy pochodne i korzystali z polimorfizmu,
    // trzeba więc w klasie bazowej zadeklarować destruktor wirtualny
    virtual ~Graph();

    virtual void MakeNull() = 0;

    int NumberOfVertices()
    {
        return numberOfVertices;
    }

    int NumberOfEdges()
    {
        return numberOfEdges;
    }

    bool Directed()
    {
        return directed;
    }

    virtual Vertex & AddVertex() = 0;

    virtual Vertex & SelectVertex(int number) = 0;

    virtual Edge & AddEdge(int u, int v) = 0;

    Edge & AddEdge(Vertex & u, Vertex & v)
    {
        return AddEdge(u.Number(), v.Number());
    }

    virtual bool IsEdge(int u, int v) = 0;

    bool IsEdge(Vertex & u, Vertex & v)
    {
        return IsEdge(u.Number(), v.Number());
    }

    virtual Edge & SelectEdge(int u, int v) = 0;

    Edge & SelectEdge(Vertex & u, Vertex & v)
    {
        return SelectEdge(u.Number(), v.Number());
    }

    virtual Iterator<Vertex> & Vertices() = 0;

    virtual Iterator<Edge> & Edges() = 0;

    virtual Iterator<Edge> & EmanatingEdges(int v) = 0;

    Iterator<Edge> & EmanatingEdges(Vertex & v)
    {
        return EmanatingEdges(v.Number());
    }

    virtual Iterator<Edge> & IncidentEdges(int v) = 0;

    Iterator<Edge> & IncidentEdges(Vertex & v)
    {
        return IncidentEdges(v.Number());
    }
};

#endif
