#include "lists_graph.hpp"
#include <cstdlib>

//konstruktor
GraphAsLists::GraphAsLists(int n)
  : Graph(n), vertices(n), adjacencyLists(n){
  for(int i=0; i<n; i++){
    vertices[i] = newVertex(i);
  }
}


//destruktor
GraphAsLists::~GraphAsLists(){
  delete_all();
}


//kasowanie wszystkiego
void GraphAsLists::delete_all(){
  //petla kasujaca krawedzie
  for(int i=0; i < numberOfVertices; i++){
    for( std::list<Edge *>::iterator it = adjacencyLists[i].begin(); it != adjacencyLists[i].end(); it++){
      if(*it == NULL)
	continue;
      int u = (*it)->Mate(i).Number();
      if(u != i){
	for( std::list<Edge *>::iterator it2 = adjacencyLists[u].begin(); it2 != adjacencyLists[u].end(); it2++){
	  if(*it2 == NULL)
	    continue;
	  if((*it2)->Mate(u).Number() == i)
	    *it2 = NULL;
	}
      }
      delete *it;
    }
  }
  for(int i=0; i < numberOfVertices; i++)
    delete vertices[i]; //kasowanie wierzcholkow
}


//zerowanie
void GraphAsLists::MakeNull(){
  delete_all();
  vertices.resize(0);
  adjacencyLists.resize(0);
  numberOfVertices = 0;
  numberOfEdges = 0;
}


//dodawanie wierzcholka
Vertex & GraphAsLists::AddVertex(){
  Vertex * p = newVertex(numberOfVertices);
  numberOfVertices++;
  vertices.resize(numberOfVertices);
  vertices[numberOfVertices - 1] = p;
  adjacencyLists.resize(numberOfVertices);
  return *p;
}


//wybieranie wierzcholka
Vertex & GraphAsLists::SelectVertex(int number){
  if(number < 0 || number >= numberOfVertices) std::abort();
  return *( vertices[number] );
}


//dodawanie krawedzi
Edge & GraphAsLists::AddEdge(int u, int v){
  if(u<0 || u >= numberOfVertices || v<0 || v >= numberOfVertices)
    std::abort();
  Edge * p = find(u,v); //szukanie wczesniej istniejacej krawedzi
  if(p != NULL)
    return *p;
  p = newEdge( *(vertices[u]), *(vertices[v]) );
  numberOfEdges++;
  adjacencyLists[u].push_back(p);
  if( !directed )
    adjacencyLists[v].push_back(p); //jeśli nieskierowany - dodajemy druga
  return *p;
}


//czy jest krawedz?
bool GraphAsLists::IsEdge(int u, int v){
  return ( find(u,v) != NULL );
}


//Wybieranie krawedzi
Edge & GraphAsLists::SelectEdge(int u, int v){
  if(u<0 || u >= numberOfVertices || v<0 || v >= numberOfVertices)
    std::abort();
  Edge * p = find(u,v);
  if(p==NULL)
    std::abort();
  return *p;
}


//szukanie krawedzi
Edge * GraphAsLists::find(int u, int v)
{
  std::list<Edge *>::iterator it, endit;
  it = adjacencyLists[u].begin();
  endit = adjacencyLists[u].end();
  while (it != endit) {
    Edge * p = *it;
    if (p->Mate(u).Number() == v)
      return p;
    ++it;
  }
  return NULL;
}



/////////////////
/// ITERATORY ///
/////////////////

//po wierzcholkach
Iterator<Vertex> & GraphAsLists::Vertices(){
  return *( new AllVerticesIter(*this) );
}

bool GraphAsLists::AllVerticesIter::IsDone(){
  return current >= owner.numberOfVertices;
}

Vertex & GraphAsLists::AllVerticesIter::operator*(){
  return *( owner.vertices[current] );
}

void GraphAsLists::AllVerticesIter::operator++(){
  ++current;
}


//po krawedziach
Iterator<Edge> & GraphAsLists::Edges(){
  return *( new AllEdgesIter(*this) );
}

GraphAsLists::AllEdgesIter::AllEdgesIter(GraphAsLists & owner) : owner(owner){
  current = 0;
  if( owner.numberOfEdges == 0 )
    AtEnd = true;
  else{
    AtEnd = false;
    it = owner.adjacencyLists[0].begin();
    itEnd = owner.adjacencyLists[0].end();
  }
}

void GraphAsLists::AllEdgesIter::next(){
  it++;
  while(it == itEnd){
    current++;
    if( current + 1 > owner.numberOfVertices )
      break;
    it = owner.adjacencyLists[current].begin();
    itEnd = owner.adjacencyLists[current].end();
  }
}

bool GraphAsLists::AllEdgesIter::IsDone(){
  return AtEnd || current + 1 > owner.numberOfVertices;
}

Edge & GraphAsLists::AllEdgesIter::operator*(){
  return *(*it);
}

void GraphAsLists::AllEdgesIter::operator++(){
  next();
}


//po krawedziach wychodzacych
Iterator<Edge> & GraphAsLists::EmanatingEdges(int v){
  return *( new EmanEdgesIter(*this, v) );
}

GraphAsLists::EmanEdgesIter::EmanEdgesIter(GraphAsLists & owner, int v) : owner(owner){
  it = owner.adjacencyLists[v].begin();
  itEnd = owner.adjacencyLists[v].end();
}

bool GraphAsLists::EmanEdgesIter::IsDone(){
  return it==itEnd;
}

Edge & GraphAsLists::EmanEdgesIter::operator*(){
  return *(*it);
}

void GraphAsLists::EmanEdgesIter::operator++(){
  ++it;
}


//po krawedziach wchodzacych
Iterator<Edge> & GraphAsLists::IncidentEdges(int v){
  if(!directed)
    return *( new EmanEdgesIter(*this, v) );
  else
    return *( new InciEdgesIter(*this, v) );
}

void GraphAsLists::InciEdgesIter::next(){
  if(it.IsDone())
    return;
  while( (*it).V1().Number() != v){
    ++it;
    if( it.IsDone() ){
      AtEnd = true;
      return;
    }
  }
}

bool GraphAsLists::InciEdgesIter::IsDone(){
  return ( AtEnd || it.IsDone() );
}

Edge & GraphAsLists::InciEdgesIter::operator*(){
  return *it;
}

void GraphAsLists::InciEdgesIter::operator++(){
  ++it;
  next();
}
