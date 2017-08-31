#include "matrix_graph.hpp"
#include <cstdlib>

GraphAsMatrix::GraphAsMatrix(int n)
: Graph(n), vertices(n), adjacencyMatrix(n)
{
    for (int i = 0; i < n; ++i) {
        vertices[i] = newVertex(i);
        adjacencyMatrix[i].resize(n);
        for (int j = 0; j < n; ++j)
            adjacencyMatrix[i][j] = NULL;
    }
}

GraphAsMatrix::~GraphAsMatrix()
{
    delete_all();
}

void GraphAsMatrix::delete_all()
{
    for (int i = 0; i < numberOfVertices; ++i) {
        delete vertices[i];
        for (int j = 0; j < numberOfVertices; ++j) {
            if (adjacencyMatrix[i][j] != NULL) {
                delete adjacencyMatrix[i][j];
                if (! directed)
                    adjacencyMatrix[j][i] = NULL;
            }
        }
    }
}

void GraphAsMatrix::MakeNull()
{
    delete_all();
    vertices.resize(0);
    adjacencyMatrix.resize(0);
    numberOfVertices = 0;
    numberOfEdges = 0;
}

Vertex & GraphAsMatrix::AddVertex()
{
    Vertex * p = newVertex(numberOfVertices);
    ++numberOfVertices;
    vertices.resize(numberOfVertices);
    vertices[numberOfVertices - 1] = p;
    // przedłuż już istniejące wiersze macierzy o 1 komórkę
    for (int i = 0; i <= numberOfVertices - 2; ++i) {
        adjacencyMatrix[i].resize(numberOfVertices);
        adjacencyMatrix[i][numberOfVertices - 1] = NULL;
    }
    // dodaj i zainicjuj nowy wiersz macierzy sąsiedztwa
    adjacencyMatrix.resize(numberOfVertices);
    adjacencyMatrix[numberOfVertices - 1].resize(numberOfVertices);
    for (int i = 0; i < numberOfVertices; ++i)
        adjacencyMatrix[numberOfVertices - 1][i] = NULL;
    return *p;
}

Vertex & GraphAsMatrix::SelectVertex(int number)
{
    if (number < 0 || number >= numberOfVertices)
        std::abort();
    return *( vertices[number] );
}

Edge & GraphAsMatrix::AddEdge(int u, int v)
{
    if (u < 0 || u >= numberOfVertices || v < 0 || v >= numberOfVertices)
        std::abort();
    Edge * p = adjacencyMatrix[u][v];
    if (p != NULL)
        return *p;
    p = newEdge(*(vertices[u]), *(vertices[v]));
    ++numberOfEdges;
    adjacencyMatrix[u][v] = p;
    if (! directed)
        adjacencyMatrix[v][u] = p;
    return *p;
}

bool GraphAsMatrix::IsEdge(int u, int v)
{
    if (u < 0 || u >= numberOfVertices || v < 0 || v >= numberOfVertices)
        std::abort();
    return adjacencyMatrix[u][v] != NULL;
}

Edge & GraphAsMatrix::SelectEdge(int u, int v)
{
    if (u < 0 || u >= numberOfVertices || v < 0 || v >= numberOfVertices)
        std::abort();
    Edge * p = adjacencyMatrix[u][v];
    if (p == NULL)
        std::abort();
    return *p;
}


Iterator<Vertex> & GraphAsMatrix::Vertices()
{
    return *( new AllVerticesIter(*this) );
}

bool GraphAsMatrix::AllVerticesIter::IsDone()
{
    return current >= owner.numberOfVertices;
}

Vertex & GraphAsMatrix::AllVerticesIter::operator*()
{
    return *( owner.vertices[current] );
}

void GraphAsMatrix::AllVerticesIter::operator++()
{
    ++current;
}


Iterator<Edge> & GraphAsMatrix::Edges()
{
    return *( new AllEdgesIter(*this) );
}

void GraphAsMatrix::AllEdgesIter::next()
{
    do {
        if (++col >= owner.numberOfVertices) {
            col = 0;
            if (++row >= owner.numberOfVertices)
                return;
        }
    } while (owner.adjacencyMatrix[row][col] == NULL);
}

bool GraphAsMatrix::AllEdgesIter::IsDone()
{
    return row >= owner.numberOfVertices;
}

Edge & GraphAsMatrix::AllEdgesIter::operator*()
{
    return *( owner.adjacencyMatrix[row][col] );
}

void GraphAsMatrix::AllEdgesIter::operator++()
{
    next();
}


Iterator<Edge> & GraphAsMatrix::EmanatingEdges(int v)
{
    return *( new EmanEdgesIter(*this, v) );
}

void GraphAsMatrix::EmanEdgesIter::next()
{
    do {
        if (++col >= owner.numberOfVertices)
            return;
    } while (owner.adjacencyMatrix[row][col] == NULL);
}

bool GraphAsMatrix::EmanEdgesIter::IsDone()
{
    return col >= owner.numberOfVertices;
}

Edge & GraphAsMatrix::EmanEdgesIter::operator*()
{
    return *( owner.adjacencyMatrix[row][col] );
}

void GraphAsMatrix::EmanEdgesIter::operator++()
{
    next();
}


Iterator<Edge> & GraphAsMatrix::IncidentEdges(int v)
{
    return *( new InciEdgesIter(*this, v) );
}

void GraphAsMatrix::InciEdgesIter::next()
{
    do {
        if (++row >= owner.numberOfVertices)
            return;
    } while (owner.adjacencyMatrix[row][col] == NULL);
}

bool GraphAsMatrix::InciEdgesIter::IsDone()
{
    return row >= owner.numberOfVertices;
}

Edge & GraphAsMatrix::InciEdgesIter::operator*()
{
    return *( owner.adjacencyMatrix[row][col] );
}

void GraphAsMatrix::InciEdgesIter::operator++()
{
    next();
}
