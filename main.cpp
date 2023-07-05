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
                         char *imx;
                         bool cchecked = false;

                 Node ( int _x, int _esize ) : x {_x}

                 { imx = new char[_esize]{}; for ( int i = 0; i <= _esize; i++ ) imx[i] = 0; } };

    class Edge { public: int from, to;
                         double weight = 0;
                         bool wchecked = false, cchecked = false;

        Edge( int _f, int _t ) : from {_f}, to {_t}

        { weight = static_cast<double>(randomize(1,1000)) * 0.01; }

    };

    std::vector<Edge> edges;
    std::vector<Node> nodes;

    public:

    bool iscycle() // searching for a cycle ->
    {
        bool cycle = false;

        class dfs { public: int e; dfs( int _e ) : e{_e} {}

            void operator()( bool &cycle, std::vector<Node> &nodes, Node &n0, int prev )

            {   if ( n0.cchecked ) cycle = true; else { n0.cchecked = true;

                for ( int n = 0; n <= e; n++ ) {

                    if ( n0.imx[n] == 1 && n != prev )

                        this->operator()( cycle, nodes, nodes[n], n0.x );


                    if ( cycle  ) break; }

                }
            }
        };

        for ( auto &n0 : nodes ) { dfs itNodes{esize};

            if ( !n0.cchecked ) {

                itNodes.operator()( cycle, nodes, n0, 99 ); if ( cycle ) break; }
        }

        for ( auto &n0 : nodes ) n0.cchecked = false;

        if ( cycle ) std::cout << "CYCLE ! -> ";

        return cycle;
    }

    // creates a random graph with esize vertices and nsize edges ->

    Graph( int _esize, int _nsize ) : esize {_esize}, nsize {_nsize}
    {       
        if ( ( ( ( esize + 1 ) * esize ) / 2 ) - 1 < nsize ) throw std::runtime_error("Too few vertices / too many edges !");

        else if ( esize > nsize + 1 ) throw std::runtime_error("Too many vertices / too few edges !");

        for ( int n = 0; n <= esize; n++ ) nodes.push_back( Node{n, esize} );

        for ( int i = 0; i <= nsize; i++ ) { bool yesno = true; int from2, to2;

            while ( yesno ) {  _re :

                from2 = ( i < esize ) ? i : randomize( 0, esize );
                to2 = ( i < esize ) ? randomize( 0, esize ) : randomize( 0, esize );

                if ( to2 == from2 ) goto _re; // ?

            if ( i > 0 ) { for ( auto& e : edges ) {

                yesno = ( ( to2 == e.from && from2 == e.to ) || ( to2 == e.to && from2 == e.from ) ) ?

                        true : false; if ( yesno ) break; } } else yesno = false;

            if ( i < esize && !yesno ) {

            nodes[from2].imx[to2] = 1; nodes[to2].imx[from2] = 1;

            yesno = iscycle();

            nodes[from2].imx[to2] = 0; nodes[to2].imx[from2] = 0;

            if ( yesno ) std::cout << from2 << " : " << to2 << std::endl; }

            } nodes[from2].imx[to2] = 1; nodes[to2].imx[from2] = 1;

            edges.push_back( Edge{ from2, to2 } );

            std::cout << from2 << " - " << to2 << " : " << edges.back().weight << std::endl;

        }

    };

    std::vector<Edge> presult;

    void Prim()
    {
        for ( auto &n0 : nodes ) for ( int i = 0; i <= esize; i++ ) n0.imx[i] = 0;

        std::cout << std::endl;

        if ( esize - 1 == nsize ) {

            std::cout << "The graph itself its own minimum spanning tree..." << std::endl; return; }

        int ecnt = 0;

        while ( ecnt < esize ) { int pos = 0, ind = 0; double minw = 99;

            for ( auto& e0 : edges ) {

                if ( e0.weight < minw && !e0.wchecked ) { minw = e0.weight; pos = ind; } ind++;

            } edges.at(pos).wchecked = true;

            nodes[edges.at(pos).from].imx[edges.at(pos).to] = 1;
            nodes[edges.at(pos).to].imx[edges.at(pos).from] = 1;

            if ( iscycle() ) { std::cout << minw << std::endl;

                nodes[edges.at(pos).from].imx[edges.at(pos).to] = 0;
                nodes[edges.at(pos).to].imx[edges.at(pos).from] = 0;

            } else { presult.push_back( edges.at(pos) ); ecnt++; }

        }

        for ( auto &p0 : presult ) std::cout << " : " << p0.weight << " : ";

        std::cout << std::endl;

    }

    void Dijkstra()
    {


    }

    ~Graph() { for ( auto &n0 : nodes ) delete[] n0.imx; }

};

int main()
{
    try { Graph g { 4, 6 };

          g.Prim();

          g.Dijkstra();

    } catch ( std::runtime_error e ) { std::cout << e.what() << std::endl; }

    return 0;
}
