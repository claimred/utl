#ifndef _PATH_BUILDER_H_
#define _PATH_BUILDER_H_

#include <vector>

#include "game_map.h"
#include "utils.h"

namespace utl_core
{
    
   class PathBuilder
   {
   public:

      struct Node
      {
         int x, y;
         Node *parent;
         std::vector<Node *> neighbours;
         double f_score, g_score;

         void setZero()
         {
            x = -1, y = -1;

            parent = 0;
            neighbours.clear();
            f_score = 0, g_score = 0;
         }
      };

      struct NodePtr
      {
         Node *node;

         NodePtr( Node *newnode ) : node(newnode) {};
      };
      
      struct PathVertex
      {
         intpair point;
         int f_score, g_score;
          
         PathVertex( intpair &a, int fscore, int gscore ) : 
            point(a), f_score(fscore), g_score(gscore) {}

         PathVertex( intpair &a ) : point(a) {}
      };

   public:

      PathBuilder( GameMap &gm );

      int getPathSize();
      void buildPath( intpair &a, intpair &b, intpair *charPos = 0 );
      intpair &operator[]( int index );
      void clear();
      mapelem::MapElem getCurrentState( int counter );
      void setBorders( int x0, int y0, int x1, int y1 );
      bool _checkBorders( int i, int j );

      ~PathBuilder();

   private:

      static bool _isMacroWalkable( GameMap &gm, int i, int j, const intpair &parent );
      static bool _isMicroWalkable( GameMap &gm, int i, int j, const intpair &parent );

      bool _isJumpOver( int x, int y, int px, int py );
      Node *_jumpHorizontal( int x, int y, int px, int py );
      Node *_jumpVertical( int x, int y, int px, int py );

      void _transformPoint( intpair &a );
      void _getWalkable( intpair &a, intpair &b );
      mapelem::MapElem _getState( intpair &a, intpair &b );
      int _heuristic( PathVertex &node, PathVertex &goal );
      double _heuristic( Node &a, Node &b );
      bool _isWalkable( int i, int j );
      void _buildPath( intpair &a, intpair &b, bool (*isWalkable)( GameMap &gm, int i, int j, const intpair &parent) );
      Node *_jump( int x, int y, int px, int py );
      void _findNeighbours( Node *node );
      void _identifySuccessors( Node *node );
      void _buildPathJPS( intpair &a, intpair &b );

      Node *getNode();
   
      int _x0, _y0, _x1, _y1;
      double _d, _d2;
      double _sqrt2;
      intpair *_charPos;
      GameMap &_gm;
      std::vector<intpair> _path;
      std::vector<int> _borders;
      std::vector<mapelem::MapElem> _states;
      Node *_bulk;
      int _allocCount;
      static const int _bulkSize = 500;

      Node _start, _goal;
      std::vector<NodePtr> _openset, _closedset;
   };
    
   bool operator<( const PathBuilder::PathVertex &a, const PathBuilder::PathVertex &b );
   bool operator<( const PathBuilder::NodePtr &a, const PathBuilder::NodePtr &b );
    

}

#endif /* _PATH_BUILDER_H_ */