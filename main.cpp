#include <iostream> 
#include <exception>
#include <random>
#include <vector>

class Graph { int esize, nsize;

    static int randomize( int min, int max )
    {
        std::random_device rd; std::mt19937 gen{ rd() };
        std::uniform_int_distribution<> distrib{ min, max };

        return distrib(gen);
    }

    class Node { public: int x;
                         double *imx; // a line from the graph's adjacency matrix, contains weights
                         bool uchecked = false;
                         double dpath = INFINITY;

                 Node ( int _x, int _esize ) : x {_x}

                 { imx = new double[_esize]{}; for ( int i = 0; i <= _esize; i++ ) imx[i] = 0.0; }

                 void clearmem() { delete[] imx; } };

    class Edge { public: int fr, to;
                         double weight = 0;
                         bool wchecked = false, uchecked = false;

                 Edge( int _f, int _t, double _w ) : fr {_f}, to {_t}, weight {_w} {}

    };

    std::vector<Edge> edges;
    std::vector<Node> nodes;

    public:

    bool iscycle() // searching for a cycle using dfs ( used by constructor & Prim ) ->
    {
        bool cycle = false;

        class dfs { public: int esize; dfs( int _e ) : esize{_e} {}

            void operator()( bool &cycle, std::vector<Node> &nodes, Node &n, int prev )

            {   if ( n.uchecked ) cycle = true; else { n.uchecked = true;

                for ( int i = 0; i <= esize; i++ ) {

                    if ( n.imx[i] > 0.0 && i != prev )

                        this->operator()( cycle, nodes, nodes[i], n.x );

                    if ( cycle ) break; }

                }
            }
        };

        for ( auto &n : nodes ) { dfs itNodes{esize};

            if ( !n.uchecked ) {

                itNodes.operator()( cycle, nodes, n, 99999 ); if ( cycle ) break; }
        }

        for ( auto &n0 : nodes ) n0.uchecked = false;

        return cycle;
    }

    // constructs a random graph with esize vertices and nsize edges ->

    Graph( int _esize, int _nsize ) : esize {_esize}, nsize {_nsize}
    {       
        if ( ( ( ( esize + 1 ) * esize ) / 2 ) - 1 < nsize ) throw std::runtime_error("Too few vertices / too many edges !");

        else if ( esize > nsize + 1 ) throw std::runtime_error("Too many vertices / too few edges !");

        for ( int n = 0; n <= esize; n++ ) nodes.push_back( Node{n, esize} );

        for ( int i = 0; i <= nsize; i++ ) { bool wrong = true; int fr, to;

            double weight = static_cast<double>(randomize(1,1000)) * 0.01;;

            while ( wrong ) {  _re :

                fr = ( i < esize ) ? i : randomize( 0, esize );
                to = ( i < esize ) ? randomize( 0, esize ) : randomize( 0, esize );

                if ( to == fr ) goto _re; // ...

                if ( !i ) wrong = false; else {

                   for ( auto& e : edges ) {

                     wrong = ( ( to == e.fr && fr == e.to ) || ( to == e.to && fr == e.fr ) ) ? true : false;

                     if ( wrong ) break; } }

                if ( !wrong ) { nodes[fr].imx[to] = weight; nodes[to].imx[fr] = weight;

                   if ( i < esize && iscycle() ) { wrong = true;

                      nodes[fr].imx[to] = 0.0; nodes[to].imx[fr] = 0.0;

                      std::cout << "[ CYCLE! -> " << fr << " : " << to << " ]" << std::endl; }
                }

            } edges.push_back( Edge{ fr, to, weight } );

            std::cout << fr << " - " << to << " : " << edges.back().weight << std::endl;

        }

    };

    std::vector<Edge> presult;

    void Prim()
    {
        for ( auto &n : nodes ) { n.uchecked = false; // spoils adjacency matrix for further uses! ->

            for ( int i = 0; i <= esize; i++ ) n.imx[i] = 0; }

        std::cout << std::endl << "Minimum spanning tree : "<< std::endl;

        if ( esize - 1 == nsize ) {

            std::cout << "The graph itself its own minimum spanning tree..." << std::endl; return; }

        int ecnt = 0;

        while ( ecnt < esize ) { int pos = 0, ind = 0; double minw = INFINITY;

            for ( auto& e : edges ) {

                if ( e.weight < minw && !e.wchecked ) { minw = e.weight; pos = ind; } ind++;

            } edges.at(pos).wchecked = true;

            nodes[edges.at(pos).fr].imx[edges.at(pos).to] = 1.0;
            nodes[edges.at(pos).to].imx[edges.at(pos).fr] = 1.0;

            if ( iscycle() ) { std::cout << " [ CYCLE! -> " << minw << " ]" << std::endl;

                nodes[edges.at(pos).fr].imx[edges.at(pos).to] = 0.0;
                nodes[edges.at(pos).to].imx[edges.at(pos).fr] = 0.0;

            } else { presult.push_back( edges.at(pos) ); ecnt++; }

        } for ( auto &e : presult ) std::cout << " | " << e.weight;

        std::cout << " |" << std::endl;

    }

    void dist2all( int x ) // distances from x to every other vertice ( used by Dijkstra and centrVec )
    {
        int vcnt = esize;

        nodes[x].dpath = 0;

        while ( vcnt >= 0 ) { int cur = 0; double minp = INFINITY;

          for ( int i = 0; i <= esize; i++ ) {

              if ( !nodes[i].uchecked && nodes[i].dpath < minp ) { cur = i; minp = nodes[i].dpath; } }

          for ( int i = 0; i <= esize; i++ ) {

              if ( nodes[cur].imx[i] > 0.0 ) { double m = nodes[cur].dpath + nodes[cur].imx[i];

                  if ( m < nodes[i].dpath ) nodes[i].dpath = m; } }

          nodes[cur].uchecked = true; vcnt--; }

    }

    void Dijkstra() // searching the shortest path
    {
        int fr, to;

          _re : fr = randomize( 0, esize );
                to = randomize( 0, esize ); if ( to == fr ) goto _re;

        std::cout << std::endl << "Shortest path from " << fr << " to " << to << " :" << std::endl;

        dist2all( to );

        std::vector<int> spath; spath.push_back(fr); int prev = -1;

        while ( spath.back() != to ) {

            for ( int i = 0; i <= esize; i++ ) {

                if ( nodes[spath.back()].imx[i] > 0.0 && i != prev ) {

                    if ( nodes[i].dpath + nodes[spath.back()].imx[i] == nodes[spath.back()].dpath )

                    { prev = spath.back(); spath.push_back(i); } }

            }
        }

        for ( auto &n : spath ) std::cout << " " << n << " -";
        std::cout << "> total distance : " << nodes[fr].dpath << std::endl;

    }

    void centrVec()
    {
        double resdist = INFINITY; int resvec = -1;

        for ( int i = 0; i <= esize; i++ ) { double maxdist = 0;

            for ( auto &n : nodes ) { n.dpath = INFINITY; n.uchecked = false; }

            dist2all( i );

            for ( auto &n : nodes ) { if ( maxdist < n.dpath ) maxdist = n.dpath; }

            if ( maxdist < resdist ) { resdist = maxdist; resvec = i; }

        } std::cout << std::endl << "Center of the graph : " << std::endl << resvec << std::endl;

    }

    ~Graph() { for ( auto &n0 : nodes ) n0.clearmem(); }

};

int main()
{
    try { Graph g { 4, 8 };

          g.Dijkstra();

          g.centrVec();

          g.Prim();

    } catch ( std::runtime_error e ) { std::cout << e.what() << std::endl; }

    return 0;
}
