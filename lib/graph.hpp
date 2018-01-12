#pragma once
#include "sparse.hpp"
#include <type_traits>

namespace graph
{
	template<typename Sparse, typename std::enable_if<std::is_same<Sparse, csr_matrix<typename Sparse::type>>::value ||
													  std::is_same<Sparse, coo_matrix<typename Sparse::type>>::value || 
													  std::is_same<Sparse, sbcsr_matrix<typename Sparse::type>>::value>::type* = nullptr 
														> inline int n_link(const Sparse &A)
	{
		return (int)A.values.size();
	}
	//============ centrality measures ========================
	struct
	{
		template<typename Sparse, typename Enable = void> auto operator()(const Sparse &, bool w = false);
		//========== CSR MATRIX =================//
		template<typename Sparse, typename std::enable_if<std::is_same<Sparse, csr_matrix<typename Sparse::type>>::value>::type> auto operator()(const Sparse &A, bool w)
		{
			using T = typename Sparse::type;
			T *degree = new T[A.Ncols];
			std::memset(degree, 0, A.Ncols * sizeof(T));
			switch ((int)w)
			{
			case 0:
			{
				for (int i = 0; i < (int)A.cols->size(); ++i) ++degree[A.cols->at(i)];
			}break;
			case 1:
			{
				for (int i = 0; i < (int)A.cols->size(); ++i) degree[A.cols->at(i)] += A.values->at(i);
			}break;
			}
			return degree;
		}
		//========== COO MATRIX =================//
		template<typename Sparse, typename std::enable_if<std::is_same<Sparse, coo_matrix<typename Sparse::type>>::value>::type> auto operator()(const Sparse &A, bool w)
		{
			using T = typename Sparse::type;
			T *degree = new T[A.Ncols];
			std::memset(degree, 0, A.Ncols * sizeof(T));
			switch ((int)w)
			{
			case 0:
			{
				for (int i = 0; i < A.index->size(); ++i) ++degree[A.index->at(i) % A.Ncols];
			}break;
			case 1:
			{
				for (int i = 0; i < A.index->size(); ++i) degree[A.index->at(i) % A.Ncols] += A.values->at(i);
			}break;
			}
			return degree;
		}
		//========== SBCSR MATRIX =================//
		template<typename Sparse, typename std::enable_if<std::is_same<Sparse, sbcsr_matrix<typename Sparse::type>>::value>::type> auto operator()(const Sparse &A, bool w)
		{
			using T = typename Sparse::type;
			T *degree = new T[A.Ncols];
			std::memset(degree, 0, A.Ncols * sizeof(T));
			switch ((int)w)
			{
			case 0:
			{
				//TO DO
			}break;
			case 1:
			{
				//TO DO
			}break;
			}
			return degree;
		}
	} out_degree;


	struct
	{
		template<typename Sparse, typename Enable = void> typename Sparse::type operator()(const Sparse &, bool w = false);
		//========== CSR MATRIX =================//
		template<typename Sparse, typename std::enable_if<std::is_same<Sparse, csr_matrix<typename Sparse::type>>::value>::type> typename Sparse::type operator()(const Sparse &A, bool w)
		{
			using T = typename Sparse::type;
			T *degree = new T[A.Nrows];
			switch ((int)w)
			{
			case 0:
			{
				for (int i = 1; i < A.Nrows + 1; ++i) degree[i - 1] = A.rows->at(i) - A.rows->at(i - 1);
			}break;
			case 1:
			{
				for (int i = 0; i < A.Nrows; ++i) degree[i] = std::accumulate(A.values->begin() + A.rows->at(i), A.values->begin() + A.rows->at(i + 1), (T)0.);
			}break;
			}
			return degree;
		}
		//========== COO MATRIX =================//
		template<typename Sparse, typename std::enable_if<std::is_same<Sparse, coo_matrix<typename Sparse::type>>::value>::type> typename Sparse::type operator()(const Sparse &A, bool w)
		{
			using T = typename Sparse::type;
			T *degree = new T[A.Nrows];
			std::memset(degree, 0, A.Ncols * sizeof(T));
			switch ((int)w)
			{
			case 0:
			{
				for (int i = 0; i < A.index->size(); ++i) ++degree[A.index->at(i) / A.Ncols];
			}break;
			case 1:
			{
				for (int i = 0; i < A.index->size(); ++i) degree[A.index->at(i) / A.Ncols] += A.values->at(i);
			}break;
			}
			return degree;
		}
		//========== SBCSR MATRIX =================//
		template<typename Sparse, typename std::enable_if<std::is_same<Sparse, sbcsr_matrix<typename Sparse::type>>::value>::type> typename Sparse::type operator()(const Sparse &A, bool w)
		{
			using T = typename Sparse::type;
			T *degree = new T[A.Nrows];
			std::memset(degree, 0, A.Nrows * sizeof(T));
			switch ((int)w)
			{
			case 0:
			{
				//TO DO
			}break;
			case 1:
			{
				//TO DO
			}break;
			}
			return degree;
		}
	} in_degree;

	template<typename Sparse> typename Sparse::type* betweenness_centrality(const Sparse &graph, bool normalized = false);

	//============ path functions =============================

	template<typename T> int* BFS(T *index, const int &n_el, const int &N, const int &M);
	template<typename T> int* BFS(T *irow, T *jcol, const int &n_el, const int &N);
	template<typename T> std::vector<int>* connected_components(T *index, const int &n_el, const int &N, T *lbl_comp); // ONLY FOR SQUARE MATRIX
	template<typename T> std::vector<int>* connected_components(T *irow, T *jcol, const int &n_el, const int &N, T *lbl_comp); // ONLY FOR SQUARE MATRIX

	struct
	{
		inline void push_path(int *path, const int &j, std::vector<int> &shortest_path)
		{
			if (path[j] == -1) return;
			push_path(path, path[j], shortest_path);
			shortest_path.push_back(j);
			return;
		}

		template<typename Sparse> auto operator()(const Sparse &graph, int src = 0, int *shortest_path = nullptr)
		{
			bool *seen = new bool[graph.Nrows];
			int *dist = new int[graph.Nrows], min, min_idx;
			std::memset(seen, graph.Nrows * sizeof(bool), false);
			for (int i = 0; i < graph.Nrows; ++i) { dist[i] = INT_MAX; shortest_path[i] = -1; }
			dist[src] = 0;
			for (int count = 0; count < graph.Nrows - 1; ++count)
			{
				min = INT_MAX;
				for (int v = 0; v < graph.Nrows; ++v)
					if (seen[v] == false && dist[v] <= min)
					{
						min = dist[v]; min_idx = v;
					}
				seen[min_idx] = true;
				for (int v = 0; v < graph.Nrows; ++v)
				{
					auto element = graph(min_idx, v);
					if (!seen[v] && element && dist[min_idx] != INT_MAX && dist[min_idx] + element < dist[v])
					{
						shortest_path[v] = min_idx;
						dist[v] = dist[min_idx] + element;
					}
				}
			}
			delete[] seen;
			return dist;
		}

		template<typename Sparse> auto operator()(const Sparse &graph, int src = 0, int dst = 0, std::vector<int> shortest_path = std::vector<int>())
		{
			bool *seen = new bool[graph.Nrows];
			int *dist = new int[graph.Nrows], min, min_idx, *path = new int[graph.Nrows];
			std::memset(seen, graph.Nrows * sizeof(bool), false);
			for (int i = 0; i < graph.Nrows; ++i) { dist[i] = INT_MAX; path[i] = -1; }
			dist[src] = 0;
			for (int count = 0; count < graph.Nrows - 1; ++count)
			{
				min = INT_MAX;
				for (int v = 0; v < graph.Nrows; ++v)
					if (seen[v] == false && dist[v] <= min)
					{
						min = dist[v]; min_idx = v;
					}
				seen[min_idx] = true;
				for (int v = 0; v < graph.Nrows; ++v)
				{
					auto element = graph(min_idx, v);
					if (!seen[v] && element && dist[min_idx] != INT_MAX && dist[min_idx] + element < dist[v])
					{
						path[v] = min_idx;
						dist[v] = dist[min_idx] + element;
					}
				}
				if (min_idx == dst || dist[min_idx] == INT_MAX)
				{
					delete[] seen;
					shortest_path.push_back(src);
					push_path(path, dst, shortest_path);
					return dist;
				}
			}
			delete[] seen;
			return dist;
		}

	} dijkstra;


	//=========== laplacian functions =========================
	struct
	{
		template<typename Sparse, typename Enable = void> Sparse operator()(const Sparse &, bool w = false);


		template<typename Sparse, typename std::enable_if<std::is_same<Sparse, csr_matrix<typename Sparse::type>>::value>::type> Sparse operator()(const Sparse &A, bool w)
		{
			using T = typename Sparse::type;
			Sparse L(A.Nrows, A.Ncols);
			L.values = new std::vector<T>(A.values->size());
			L.cols = new std::vector<int>(A.cols->size());
			for (int i = 0; i < (int)A.values->size(); ++i)
			{
				L.values->at(i) = -A.values->at(i);
				L.cols->at(i) = A.cols->at(i);
			}
			std::memcpy(L.rows, A.rows, A.rows * sizeof(T));
			T *degree = out_degree(A, w);
			for (int i = 0; i < A.Nrows; ++i) L(i, i, degree[i]);
			delete[] degree;
			return L;
		}
		template<typename Sparse, typename std::enable_if<std::is_same<Sparse, coo_matrix<typename Sparse::type>>::value>::type> Sparse operator()(const Sparse &A, bool w)
		{
			using T = typename Sparse::type;
			Sparse L(A.Nrows, A.Ncols);
			L.values = new std::vector<T>(A.values->size());
			L.index = new std::vector<int>(A.index->size());
			for (int i = 0; i < (int)A.values->size(); ++i)
			{
				L.values->at(i) = -A.values->at(i);
				L.index->at(i) = A.index->at(i);
			}
			T *degree = out_degree(A, w);
			for (int i = 0; i < A.Nrows; ++i) L(i, i, degree[i]);
			delete[] degree;
			return L;
		}
	} laplacian;
}



namespace graph
{
	template<typename T, typename Sparse> typename Sparse::type* betweenness_centrality(const Sparse &graph, bool normalized)
	{
		T *sigma_st_v = new T[graph.Nrows], *sigma_st = new T[graph.Nrows], *bc = new T[graph.Nrows];
		std::memset(sigma_st_v, 0, sizeof(T)*graph.Nrows);
		std::memset(sigma_st, 0, sizeof(T)*graph.Nrows);
		bool *seen = new bool[graph.Nrows];
		int *dist = new int[graph.Nrows], min, min_idx, *shortest_path = new int[graph.Nrows];
		T element;
		for(int src = 0; src < graph.Nrows; ++src)
		{
			// DIJKSTRA ALGORITHM
			std::memset(seen, false, sizeof(bool)*graph.Nrows);
			for(int i = 0; i < graph.Nrows; ++i) {dist[i] = INT_MAX; shortest_path[i] = -1;}
			dist[src] = 0;
			for(int count = 0; count < graph.Nrows - 1; ++count)
			{
				min = INT_MAX;
				for(int v = 0; v < graph.Nrows; ++v)
					if(seen[v] == false && dist[v] <= min)
					{
						min = dist[v]; min_idx = v;
					}
				seen[min_idx] = true;
				for(int v = 0; v < graph.Nrows; ++v)
				{
					element = graph(min_idx, v);
					if(!seen[v] && element && dist[min_idx] != INT_MAX && dist[min_idx] + element < dist[v])
					{
						if(shortest_path != nullptr) shortest_path[v] = min_idx;
						dist[v] = dist[min_idx] + element;
					}
				}

			} // end loop dijkstra
			for(int j = 0; j < graph.Nrows; ++j)
				push_path(shortest_path, src, j, sigma_st[src], sigma_st_v);
		} // end loop src
		
		if(normalized)
			for(int i = 0; i < graph.Nrows; ++i) bc[i] = (sigma_st_v[i] * 4.f) / ((sigma_st[i] - 1) * (graph.Nrows - 1) * (graph.Nrows - 2));
		else
			for(int i = 0; i < graph.Nrows; ++i) bc[i] = (sigma_st_v[i] * 2.f) / (sigma_st[i] - 1);

		delete[] seen;
		delete[] dist;
		delete[] shortest_path;
		delete[] sigma_st;
		delete[] sigma_st_v;
		return bc;
	}

	//==============================================================================================

	template<typename T> int* BFS(T *index, const int &n_el, const int &N, const int &M)
	{
		int *pi = new int[N], seen = 0, start = 0, stop = 1;
		pi[seen] = (int)(rand() % N);
		while(seen < N - 1)
		{
			for(int vi = start; vi < stop; ++vi)
			{
				for(int wi = 0; wi < n_el; ++wi)
				{
					if(index[wi] / M == pi[vi])
					{
						if(std::find(pi, pi + seen + 1, index[wi] % M) == pi + seen + 1)
						{
							++seen;
							pi[seen] = index[wi] % M;
						}
					}
				}
			}
			start = stop;
			stop = (N < seen + 1) ? seen : seen + 1;
		}
		return pi;
	}

	template<typename T> int* BFS(T *irow, T *jcol, const int &n_el, const int &N)
	{
		int *pi = new int[N], seen = 0, start = 0, stop = 1;
		pi[seen] = (int)(rand() % N);
		while(seen < N - 1)
		{
			for(int vi = start; vi < stop; ++vi)
			{
				for(int wi = 0; wi < n_el; ++wi)
				{
					if(irow[wi] == pi[vi])
					{
						if(std::find(pi, pi + seen + 1, jcol[wi]) == pi + seen + 1)
						{
							++seen;
							pi[seen] = jcol[wi];
						}
					}
				}
			}
			start = stop;
			stop = (N < seen + 1) ? seen : seen + 1;
		}
		return pi;
	}

	template<typename T> std::vector<int>* connected_components(T *index, const int &n_el, const int &N, T *lbl_comp)
	{
		std::vector<int> *num = new std::vector<int>(1, 1);
		int seen = 0, start = 0, stop = 1, tmp;
		bool *see = new bool[N];
		std::memset(see, true, N*sizeof(bool));
		lbl_comp[0] = 0;
		see[0] = false;
		while(seen < N-1)
		{
			for(int vi = start; vi < stop; ++vi)
			{
				for(int wi = 0; wi < n_el; ++wi)
				{
					tmp = index[wi] % N;
					if(index[wi] / N == lbl_comp[vi] && see[tmp])
					{
						++seen;
						lbl_comp[seen] = tmp;
						see[tmp] = false;
						++num->at(num->size()-1);
					}
				}
			}
			start = stop;
			stop = (N < seen + 1) ? seen : seen + 1;
			if(start == stop)
			{
				if(seen == N-1)
				{
					num->push_back(seen + 1);
					break;
				}

				for(int i = 0; i < N; ++i)
					if(see[i])
					{
						++seen;
						lbl_comp[seen] = i;
						see[i] = false;
						break;
					}
				num->push_back(seen+1);
				start = lbl_comp[seen];
				stop = start + 1;
			}
		}
		delete[] see;
		return num;
	}

	template<typename T> std::vector<int>* connected_components(T *irow, T *jcol, const int &n_el, const int &N, T *lbl_comp)
	{
		std::vector<int> *num = new std::vector<int>(1, 1);
		int seen = 0, start = 0, stop = 1;
		bool *see = new bool[N];
		std::memset(see, true, N*sizeof(bool));
		lbl_comp[0] = 0;
		see[0] = false;
		while(seen < N-1)
		{
			for(int vi = start; vi < stop; ++vi)
			{
				for(int wi = 0; wi < n_el; ++wi)
				{
					if(irow[wi] == lbl_comp[vi] && see[jcol[wi]])
					{
						++seen;
						lbl_comp[seen] = jcol[wi];
						see[jcol[wi]] = false;
						++num->at(num->size()-1);
					}
				}
			}
			start = stop;
			stop = (N < seen + 1) ? seen : seen + 1;
			if(start == stop)
			{
				if(seen == N-1)
				{
					num->push_back(seen + 1);
					break;
				}

				for(int i = 0; i < N; ++i)
					if(see[i])
					{
						++seen;
						lbl_comp[seen] = i;
						see[i] = false;
						break;
					}
				num->push_back(seen+1);
				start = lbl_comp[seen];
				stop = start + 1;
			}
		}
		delete[] see;
		return num;
	}


	//====================================================================================0

	
} // end namespace graph
