import numpy as np
from collections import Counter
from collections import deque
from collections import defaultdict
import warnings
import hypernetx as hnx

class HyperGraph():
    N = 0
    M = 0
    E = []
    node_index = {}
    node_attribute = {}
    elist = {}

    def __init__(self):
        self.N = 0
        self.M = 0
        self.V = set()
        self.E = []
        self.node_index = {}
        self.node_attribute = {}
        self.elist = {}

    def nodes(self):

        return set(self.V)

    def hyperedges(self):

        return list(self.E)

    def add_node(self, v, attr: dict=None):
        if v not in self.node_index:
            self.node_index[v] = len(self.node_index)
            self.N = len(self.node_index)
            if attr is not None:
                self.node_attribute[v] = attr
            self.elist[v] = []

        self.V = set(self.node_index.keys())

        return

    def set_node_attribute(self, v, attr: dict):
        if v not in self.node_index:
            self.add_node(v, attr)
        else:
            self.node_attribute[v] = attr

        return

    def add_nodes_from(self, nodes, attrs: dict=None):
        for v in nodes:
            if attrs is None or attrs is not dict:
                self.add_node(v)
            elif v not in self.node_attribute:
                warnings.warn('Node attributes were inputted but node ' + str(v) + ' has no attribute ' + str(attrs) + '.')
                self.add_node(v)
            else:
                self.add_node(v, attrs[v])

        return

    def add_hyperedge(self, e: list):
        self.add_nodes_from(e)
        self.E.append(list(e))
        m = len(self.E) - 1
        for v in e:
            self.elist[v].append(m)
        self.M = len(self.E)

        return

    def add_hyperedges_from(self, hyperedges: list):
        nodes = set()
        for e in hyperedges:
            nodes = nodes | set(e)
        self.add_nodes_from(nodes)

        for e in hyperedges:
            self.add_hyperedge(e)

        return

    def add_node_to_hyperedge(self, v, m):
        # Add node v to hyperedge E[e_i]

        if v not in self.elist or m < 0 or m >= self.M:
            print("Error: Given node is not found.")
            exit()

        self.E[m].append(v)
        self.elist[v].append(m)

        return

    def remove_node_from_hyperedge(self, v, m):
        # Remove node v from hyperedge E[e_i]

        if v not in self.elist:
            print("Error: Given node is not found.")
            exit()
        if m < 0 or m >= self.M:
            print("Error: Given hyperedge is not found.")
            exit()

        if m not in self.elist[v]:
            warnings.warn('Given node is not included in the given hyperedge.')
        else:
            self.elist[v].remove(m)

        if v not in self.E[m]:
            warnings.warn('Given node does not belong to the given hyperedge.')
        else:
            self.E[m].remove(v)

        return

    def nodes_incident_with_hyperedge(self, m):
        if m < 0 or m >= self.M:
            print("ERROR: hyperedge index is out of range.")
            return []

        return self.E[m]

    def hyperedges_incident_with_node(self, v):
        if v not in self.node_index:
            print("ERROR: node index is out of range.")
            return []

        return self.elist[v]

    def is_connected(self):

        searched = {i: 0 for i in self.node_index}
        nodes = set()
        v = np.random.choice(list(self.node_index.keys()))

        Q = deque()
        searched[v] = 1
        Q.append(v)
        while len(Q) > 0:
            v = Q.popleft()
            nodes.add(v)
            for m in self.elist[v]:
                for w in self.E[m]:
                    if searched[w] == 0:
                        searched[w] = 1
                        Q.append(w)

        return self.N == sum(list(searched.values()))

    def node_degree(self, v=None, ndarray=False):
        if v is None:
            if ndarray is False:
                node_degree = {}
                for m in range(0, self.M):
                    for v in self.E[m]:
                        node_degree[v] = node_degree.get(v, 0) + 1
                return node_degree
            else:
                node_degree = np.zeros(self.N, dtype=int)
                for m in range(0, self.M):
                    for v in self.E[m]:
                        node_degree[v] += 1
                return node_degree
        elif v not in self.elist:
            print("ERROR: node index is out of range.")
            return -1
        else:
            return len(self.elist[v])

    def hyperedge_size(self, m=None, ndarray=False):
        if m is None:
            if ndarray is False:
                return {m: len(self.E[m]) for m in range(0, self.M)}
            else:
                l = np.zeros(self.M, dtype=int)
                for m in range(0, self.M):
                    l[m] = len(self.E[m])
                return l
        elif m < 0 or m >= self.M:
            print("ERROR: hyperedge index is out of range.")
            return -1
        else:
            return len(self.E[m])

    def node_degree_distribution(self, func):
        if func == 'freq':
            return dict(Counter(list(self.node_degree().values())))
        elif func == 'prob':
            d = dict(Counter(list(self.node_degree().values())))
            return {k: float(d[k])/self.N for k in d}
        elif func == 'survival':
            d_ = dict(Counter(list(self.node_degree().values())))
            lst = sorted(list(d_.items()), key=lambda x:x[0], reverse=False)
            d = {}
            for i in range(0, len(lst)):
                k = lst[i][0]
                n = sum([lst[j][1] for j in range(i, len(lst))])
                d[k] = float(n)/self.N
            return d
        else:
            print("ERROR: given function is not supported.")
            return {}

    def hyperedge_size_distribution(self, func):
        if func == 'freq':
            return dict(Counter(list(self.hyperedge_size().values())))
        elif func == 'prob':
            d = dict(Counter(list(self.hyperedge_size().values())))
            return {k: float(d[k])/self.N for k in d}
        elif func == 'survival':
            d_ = dict(Counter(list(self.hyperedge_size().values())))
            lst = sorted(list(d_.items()), key=lambda x:x[0], reverse=False)
            d = {}
            for i in range(0, len(lst)):
                k = lst[i][0]
                n = sum([lst[j][1] for j in range(i, len(lst))])
                d[k] = float(n)/self.M
            return d
        else:
            print("ERROR: given function is not supported.")
            return {}

    def joint_node_degree_matrix(self, ndarray=False):
        # Calculate the number of hyperedges that nodes with degree k and nodes with degree k' share.

        node_degree = self.node_degree()
        node_degrees = set(list(node_degree.values()))

        jnd = {(k1, k2): 0 for k1 in node_degrees for k2 in node_degrees}

        for e in self.E:
            s = int(len(e))
            for i in range(0, s - 1):
                u = e[i]
                k1 = int(len(self.elist[u]))
                for j in range(i + 1, s):
                    v = e[j]
                    k2 = int(len(self.elist[v]))
                    jnd[(k1,k2)] += 1
                    jnd[(k2,k1)] += 1

        if ndarray:
            max_deg = max(node_degrees)
            jnd_ = np.zeros((max_deg+1, max_deg+1), dtype=float)
            for (k, l) in jnd:
                jnd_[k][l] += jnd[(k, l)]
            return jnd_
        else:
            return jnd

    def joint_node_degree_distribution(self, ndarray=False):

        node_degree = self.node_degree()
        node_degrees = set(list(node_degree.values()))

        jnd = {(k1, k2): 0 for k1 in node_degrees for k2 in node_degrees}
        norm = 0

        for e in self.E:
            s = int(len(e))
            norm += s * (s - 1)
            for i in range(0, s - 1):
                u = e[i]
                k1 = int(len(self.elist[u]))
                for j in range(i + 1, s):
                    v = e[j]
                    k2 = int(len(self.elist[v]))
                    jnd[(k1,k2)] += 1
                    jnd[(k2,k1)] += 1

        if ndarray:
            max_deg = max(node_degrees)
            jnd_ = np.zeros((max_deg+1, max_deg+1), dtype=float)
            for (k, l) in jnd:
                jnd_[k][l] = float(jnd[(k, l)]) / norm
            return jnd_
        else:
            for (k, l) in jnd:
                jnd[(k,l)] /= norm
            return jnd

    def average_degree_of_nearest_neighbors_of_nodes(self):
        sum_neighbor_node_degree = {}
        N_k = {}
        node_degree = self.node_degree()

        for v in self.nodes():
            k = node_degree[v]
            N_k[k] = N_k.get(k, 0) + 1

            x = 0
            y = 0
            for m in self.elist[v]:
                e = self.E[m]
                for w in e:
                    l = node_degree[w]
                    x += 1
                    y += l
                x -= 1
                y -= k
            if x > 0:
                sum_neighbor_node_degree[k] = sum_neighbor_node_degree.get(k, 0) + float(y) / x

        annd = defaultdict(float)
        for k in N_k:
            if N_k[k] > 0:
                annd[k] = float(sum_neighbor_node_degree[k]) / N_k[k]

        return annd

    def node_redundancy_coefficient(self, ndarray=False):
        # Calculate the redundancy coefficient of each node.
        # See the following paper for the detail of the redundancy coefficient.
        # Basic notions for the analysis of large two-mode networks, M. Latapy, C. Magnien, N. Del Vecchio, Social networks, 2008.

        nodes = self.nodes()
        rc = {v: 0.0 for v in nodes}

        for v in nodes:
            d = len(self.elist[v])

            if d < 2:
                rc[v] = 0
                continue

            for i in range(0, d - 1):
                e1 = self.elist[v][i]
                for j in range(i + 1, d):
                    e2 = self.elist[v][j]
                    if e1 == e2:
                        continue
                    s = set(set(set(self.E[e1]) & set(self.E[e2])) - set(list([v])))
                    if len(s) > 0:
                        rc[v] += 2.0

            rc[v] = float(rc[v]) /(d * (d - 1))

        if ndarray:
            rc_ = np.zeros(self.N, dtype=float)
            for v in nodes:
                rc_[v] = rc[v]
            return rc_
        else:
            return rc

    def degree_dependent_node_redundancy_coefficient(self, ndarray=False):
        # Calculate the degree-dependent redundancy coefficient of the node (i.e., the average of the redundancy coefficient over the nodes with degree k).

        rc = self.node_redundancy_coefficient()
        nd = self.node_degree()

        node_degrees = set()
        nodes = self.nodes()
        for v in nodes:
            k = int(len(self.elist[v]))
            node_degrees.add(k)

        ddrc = {k: 0.0 for k in node_degrees}
        n_k = {k: 0.0 for k in node_degrees}

        for v in nodes:
            k = nd[v]
            ddrc[k] += rc[v]
            n_k[k] += 1

        for k in ddrc:
            if n_k[k] > 0:
                ddrc[k] = float(ddrc[k]) / n_k[k]

        if ndarray:
            max_d = max(node_degrees)
            ddrc_ = np.zeros(max_d+1, dtype=float)
            for k in ddrc:
                ddrc_[k] = ddrc.get(k, 0)
            return ddrc_
        else:
            return ddrc

    def node_shortest_path_length_distribution(self):
        dist = {}

        for s in self.node_index:
            d = {w: -1 for w in self.node_index}
            Q = deque()
            d[s] = 0
            Q.append(s)

            while len(Q) > 0:
                v = Q.popleft()
                for m in self.elist[v]:
                    for w in self.E[m]:
                        if d[w] < 0:
                            d[w] = d[v] + 1
                            Q.append(w)

            for w in d:
                if w == s or d[w] <= 0:
                    continue
                k = d[w]
                dist[k] = dist.get(k, 0) + 1

        return dist

    def single_source_node_shortest_path_length(self, v):
        if v not in self.node_index:
            print("ERROR: given node is not found.")
            return {}

        d = {w: -1 for w in self.node_index}

        s = v
        Q = deque()
        d[s] = 0
        Q.append(s)

        while len(Q) > 0:
            v = Q.popleft()
            for m in self.elist[v]:
                for w in self.E[m]:
                    if d[w] < 0:
                        d[w] = d[v] + 1
                        Q.append(w)

        return d

    def rich_club_coefficient(self):
        node_degrees = set()
        for v in self.node_index:
            k = int(len(self.elist[v]))
            node_degrees.add(k)

        min_k, max_k = 1, max(node_degrees)
        rcc = {k: 0 for k in range(min_k, max_k + 1)}
        for e in self.E:
            k = min([self.node_degree(v) for v in e])
            for k_ in range(1, k):
                rcc[k_] += 1

        return rcc

    def print_info(self):
        node_degree = {i: 0 for i in self.node_index}
        hyperedge_size = {m: 0 for m in range(0, self.M)}
        for m in range(0, self.M):
            for i in self.E[m]:
                node_degree[i] += 1
            hyperedge_size[m] = len(self.E[m])

        node_degree_lst = list(node_degree.values())
        hyperedge_size_lst = list(hyperedge_size.values())

        print("Number of nodes:", self.N)
        print("Number of hyperedges:", self.M)
        print("Average degree of node:", float(sum(node_degree_lst)) / self.N)
        print("Maximum degree of node:", max(node_degree_lst))
        print("Frequency distribution of node degree:", dict(Counter(node_degree_lst)))
        print("Average size of hyperedge:", float(sum(hyperedge_size_lst)) / self.M)
        print("Maximum size of hyperedge:", max(hyperedge_size_lst))
        print("Frequency distribution of hyperedge size:", dict(Counter(hyperedge_size_lst)))
        print("Hypergraph is connected:", self.is_connected())
        print()

        return

    def convert_to_hnx_hypergraph(self):
        B = np.zeros((self.N, self.M), dtype=int)
        for m in range(0, self.M):
            for v in self.E[m]:
                B[v][m] += 1

        H_hnx = hnx.Hypergraph.from_incidence_matrix(B)

        return H_hnx

def read_hypergraph(hypergraph_name):
    # Read hypergraph named hypergraph_name.
    # The corresponding files to read must be in the folder ./hyper-dk-series/data/.

    f1_path = "../data/" + str(hypergraph_name) + "_nverts.txt"
    f1 = open(f1_path, 'r')
    f2_path = "../data/" + str(hypergraph_name) + "_hyperedges.txt"
    f2 = open(f2_path, 'r')

    lines1 = f1.readlines()
    lines2 = f2.readlines()

    E = []
    c = 0
    for line1 in lines1:
        nv = int(line1[:-1].split(" ")[0])

        e = []
        for i in range(0, nv):
            v = int(lines2[c+i][:-1])
            e.append(v)

        E.append(e)
        c += nv

    f1.close()
    f2.close()

    H = HyperGraph()
    H.add_hyperedges_from(E)

    return H

def write_hypergraph(H: HyperGraph, hypergraph_name):
    f1_path = "../data/" + str(hypergraph_name) + "_nverts.txt"
    f1 = open(f1_path, 'w')

    for e in H.E:
        f1.write(str(int(len(e))) + "\n")

    f1.close()

    f2_path = "../data/" + str(hypergraph_name) + "_hyperedges.txt"
    f2 = open(f2_path, 'w')

    for e in H.E:
        for v in e:
            f2.write(str(int(v)) + "\n")

    f2.close()

    # print("Wrote the " + str(k) + "-th hypergraph randomized with (" + d_v + ", " + d_e + ").\n")

    return
