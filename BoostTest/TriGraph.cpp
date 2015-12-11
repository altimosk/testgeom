#include <utility> // std::pair
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/boykov_kolmogorov_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include "..\gengraph.h"
#include <math.h>
#include  <ctime>

using namespace boost;

class TriGraph
{
public:
	TriGraph(int nx, int ny) : NX(nx), NY(ny), NXY(nx*ny), Z(1000. / NY), graph(nx*ny * 2 + 2), src(2 * NXY), tgt(2 * NXY + 1)
	{
		for (int i = 0; i < nx; ++i)
			for (int j = 0; j < ny; ++j)
			{
				Vertex v = Vert(i, j, false);
				AddEdge(v, Vert(i, j, true));
				if (i > 0)
					AddEdge(v, Vert(i - 1, j, true));
				if (j > 0)
					AddEdge(v, Vert(i, j - 1, true));
				if (i < NX - 1)
					AddEdge(v, Vert(i + 1, j, true));
				if (j < NY - 1)
					AddEdge(v, Vert(i, j + 1, true));
				if (i < NX - 1 && j > 0)
					AddEdge(v, Vert(i + 1, j - 1, true));
				if (j < NY - 1 && i > 0)
					AddEdge(v, Vert(i - 1, j + 1, true));
			}
	}

	void AddSpecials(int sx, int sy, int tx, int ty, int w)
	{
		for (int i = sx - w + 1; i < sx + w; ++i)
			for (int j = sy - w + 1; j < sy + w; ++j)
			{
				if (i >= 0 && i < NX && j >= 0 && j < NY)
					AddEdge(src, Vert(i, j, true));
			}

		for (int i = tx - w + 1; i < tx + w; ++i)
			for (int j = ty - w + 1; j < ty + w; ++j)
			{
				if (i >= 0 && i < NX && j >= 0 && j < NY)
					AddEdge(Vert(i, j, false), tgt);
			}
	}

	int MaxFlowEdmondsKarp()
	{
		std::cout << "start edmonds_karp on a graph with " << num_vertices(graph) << 
			         " vertices and " << num_edges(graph) << " edges" << std::endl;
		std::clock_t clk = clock();

		int flow = edmonds_karp_max_flow( graph, src, tgt);
		clk = clock() - clk;
		int cost = find_flow_cost(graph);
		
		std::cout << "Found flow " << flow << " of cost " << cost << " in " << ((double)(clk)) / CLOCKS_PER_SEC << " seconds" << std::endl;
		return flow;
	}
		
	int MaxFlowPushRelabel()
	{
		std::cout << "start push_relabel on a graph with " << num_vertices(graph) <<
			" vertices and " << num_edges(graph) << " edges" << std::endl;
		std::clock_t clk = clock();

		int flow = push_relabel_max_flow(graph, src, tgt);
		clk = clock() - clk;
		int cost = find_flow_cost(graph);

		std::cout << "Found flow " << flow << " of cost " << cost << " in " << ((double)(clk)) / CLOCKS_PER_SEC << " seconds" << std::endl;
		return flow;
	}

	int MaxFlowBoykovKolmogorov()
	{
		std::cout << "start boykov_kolmogorov on a graph with " << num_vertices(graph) <<
			" vertices and " << num_edges(graph) << " edges" << std::endl;
		std::clock_t clk = clock();

		int flow = boykov_kolmogorov_max_flow(graph, src, tgt);
		clk = clock() - clk;
		int cost = find_flow_cost(graph);

		std::cout << "Found flow " << flow << " of cost " << cost << " in " << ((double)(clk)) / CLOCKS_PER_SEC << " seconds" << std::endl;
		return flow;
	}

	int MinCostMaxFlow()
	{
		std::cout << "start successive_shortest on a graph with " << num_vertices(graph) <<
			" vertices and " << num_edges(graph) << " edges" << std::endl;
		std::clock_t clk = clock();

		successive_shortest_path_nonnegative_weights(graph, src, tgt);
		clk = clock() - clk;
		int cost = find_flow_cost(graph);

		std::cout << "Found flow " << "???" << " of cost " << cost << " in " << ((double)(clk)) / CLOCKS_PER_SEC << " seconds" << std::endl;
		return cost;
	}

	void RemoveSpecials()
	{
		std::vector<Edge> spec_edges;
		graph_traits<Graph>::edge_iterator ei, ei_end;
		for (boost::tie(ei, ei_end) = edges(graph); ei != ei_end; ++ei)
		{
			if (IsSpecial(*ei))
				spec_edges.push_back(*ei);
		}
		for( auto e : spec_edges )
			remove_edge(e, graph);

	}

	void Draw(bool keep = false)
	{
		extern GenericGraphics* GG;
		if (keep)
		{
			GG->DeleteSavedGraphics();
			Point c = Coord(Vert(NX / 2, NY / 2, true));
			GG->Center(c.first, c.second);
			GG->Redraw();
		}

		int n = 0; 
		for (int i = 0; i < 2; ++i)
		{
			int color = i == 0 ? kLtGray : kRed;
			graph_traits<Graph>::edge_iterator ei, ei_end;
			for (boost::tie(ei, ei_end) = edges(graph); ei != ei_end; ++ei)
			{
				if ( (i || !IsSpecial(*ei)) && get(edge_capacity, graph)[*ei] <= 0)
					continue;

				if ( i == 1 && Flow(*ei) <= 0)
					continue;

				Point s = Coord(source(*ei, graph)),
					  t = Coord(target(*ei, graph));

				if (s.first == t.first && s.second == t.second)
					continue;

				GG->DrawLine(s.first, s.second, t.first, t.second, color, i, false, !keep);
				++n;
			}
		}
		std::cout << "Drew " << n << " lines" << std::endl;
	}

private:
//	typedef adjacency_list<> Graph;
	typedef std::pair<int, int> Point;
	typedef adjacency_list_traits < vecS, vecS, directedS > Traits;
	typedef Traits::edge_descriptor Edge;
	typedef Traits::vertex_descriptor Vertex;
	typedef adjacency_list < vecS, vecS, directedS, 
		property < vertex_name_t, std::string,
		property < vertex_index_t, long,
		property < vertex_color_t, boost::default_color_type,
		property < vertex_distance_t, long,
		property < vertex_predecessor_t, Traits::edge_descriptor > > > > >, 

		property < edge_capacity_t, int,
		property < edge_residual_capacity_t, int,
		property < edge_reverse_t, Edge,
		property <edge_weight_t, long
		> > > > > Graph;

 	int NX, NY, NXY;
	double Z;
	Graph graph;
	Vertex src, tgt;

	void AddEdge(Vertex v1, Vertex v2)
	{
		Edge e1, e2;
		bool add1, add2;
		boost::tie(e1, add1) = add_edge(v1, v2, graph);
		boost::tie(e2, add2) = add_edge(v2, v1, graph);
		if (!add1 || !add2)
			return;

		get(edge_reverse, graph)[e1] = e2;
		get(edge_reverse, graph)[e2] = e1;
		get(edge_capacity, graph)[e1] = 0;
		get(edge_capacity, graph)[e2] = 0;
		get(edge_weight, graph)[e1] = 0;
		get(edge_weight, graph)[e2] = 0;

		if (IsBlocked(v1) || IsBlocked(v2))
			return;

		bool zeroLen = (v1 % NXY == v2 % NXY); // if 0 length, flow gos from in to out, and weight is 0

		get(edge_capacity, graph)[e1] = (!IsIn(v1) && IsIn(v2)) != zeroLen;
		get(edge_capacity, graph)[e2] = (IsIn(v1) && !IsIn(v2)) != zeroLen;
		
		if (zeroLen)
			return;

		int w = 0;

		if (get(edge_capacity, graph)[e1] > 0)
			w = Weight(v1);
		else if (get(edge_capacity, graph)[e2] > 0)
			w = -Weight(v2);

		get(edge_weight, graph)[e1] = w;
		get(edge_weight, graph)[e2] = -w;
	}

	Vertex Vert(int i, int j, bool in) {
		return j*NX + i + (in ? 0 : NXY); }

	Point LogCoord(Vertex v)
	{
		if (v == src )
			return std::make_pair(-1, -1);
		if( v == tgt)
			return std::make_pair((int)NX, (int)NY);
		v = v % NXY;
		return std::make_pair((int)(v % NX), (int)(v / NX));
	}

	bool IsIn(Vertex v) {
		return v < (size_t)NXY || v == tgt;
	}

	bool IsSpecial(Vertex v) {
		return v >= 2 * (size_t)NXY;
	}
	bool IsSpecial(Edge e) {
		return IsSpecial(source(e, graph)) || IsSpecial(target(e, graph));
	}

	int Flow(Edge e) {
		return get(edge_capacity, graph)[e] - get(edge_residual_capacity, graph)[e];
	}

	// geometry
	Point Coord(Vertex v)
	{
		if (v == src || v == tgt)
		{
			Point p1 = Coord(Vert(0, 0, false));
			Point p2 = Coord(Vert(NX - 1, NY - 1, false));
			return (v == src) ? std::make_pair(p1.first, p2.second) : std::make_pair(p2.first, p1.second);
		}
		Point p = LogCoord(v);
		p.first = W*p.first + W / 2 * p.second;
		p.second = H*p.second;
		return p;
	}

	bool IsBlocked(Vertex v)
	{
		if (IsSpecial(v))
			return false;

		// block all points in radius of R around all points in GxG square grid
		const double R = 3*W;
		const double G = 10*W;
		Point p = Coord(v);
		Point c((int)(round(p.first / G) * G), (int)(round(p.second / G) * G));
		
		return hypot(p.first - c.first, p.second - c.second) <= R;
	}

	int Weight(Vertex v) {
		Point c = Coord(Vert(NX / 2, NY / 2, true));
		Point p = Coord(v);
		return (p.first - c.first)*(p.first - c.first);
	}

	// points 0,0 - 8,0 - 4,7 make a roughly equilateral triangle
	static const int H = 7;
	static const int W = 8;

};

TriGraph* CreateTriGraph(int nx, int ny) {
	return new TriGraph(nx, ny); }

void DrawTriGraph(TriGraph* t) {
	t->Draw();
}

void TriGraphMaxFlowEdmondsKarp(TriGraph* t, int sx, int sy, int tx, int ty, int w) {
	t->AddSpecials(sx, sy, tx, ty, w);
	t->MaxFlowEdmondsKarp();
	t->RemoveSpecials();
	t->Draw(true);
}

void TriGraphMaxFlowPushRelabel(TriGraph* t, int sx, int sy, int tx, int ty, int w) {
	t->AddSpecials(sx, sy, tx, ty, w);
	t->MaxFlowPushRelabel();
	t->RemoveSpecials();
	t->Draw(true);
}

void TriGraphMaxFlowBoykovKolmogorov(TriGraph* t, int sx, int sy, int tx, int ty, int w) {
	t->AddSpecials(sx, sy, tx, ty, w);
	t->MaxFlowBoykovKolmogorov();
	t->RemoveSpecials();
	t->Draw(true);
}
void TriGraphMinCostMaxFlow(TriGraph* t, int sx, int sy, int tx, int ty, int w) {
	t->AddSpecials(sx, sy, tx, ty, w);
	t->MinCostMaxFlow();
	t->RemoveSpecials();
	t->Draw(true);
}
void ReleaseTriGraph(TriGraph* t) {
	delete t; }
