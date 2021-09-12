from collections import defaultdict

class HyperGraph():
    V = []
    E = []
    elist = defaultdict(list)

    def __init__(self):
        self.V = []
        self.E = []
        self.elist = {}

    def read_hypergraph(self, graph_name):
        f1_path = "../data/" + str(graph_name) + "_nverts.txt"
        f1 = open(f1_path, 'r')
        f2_path = "../data/" + str(graph_name) + "_hyperedges.txt"
        f2 = open(f2_path, 'r')

        lines1 = f1.readlines()
        lines2 = f2.readlines()

        self.__init__()
        c = 0
        e_i = 0
        for line1 in lines1:
            nv = int(line1[:-1].split(" ")[0])

            e = []
            for i in range(0, nv):
                v = int(lines2[c+i][:-1])
                e.append(v)

            self.E.append(e)
            for v in e:
                if v not in self.V:
                    self.V.append(v)
                    self.elist[v] = []
                self.elist[v].append(e_i)
            c += nv
            e_i += 1

        f1.close()
        f2.close()

        print('The given hypergraph named ' +str(graph_name) + ' was successfully read.')
        print("Number of nodes: ", len(self.V))
        print("Number of hyperedges: ", len(self.E), "\n")

        return 0

    def add_node_to_hyperedge(self, v, e_i):
        if v not in self.elist:
            print("Error: Given node is not found.")
            exit()
        if e_i < 0 or len(self.E) <= e_i:
            print("Error: Given hyperedge is not found.")
            exit()

        self.E[e_i].append(v)
        self.elist[v].append(e_i)

        return 0

    def remove_node_from_hyperedge(self, v, e_i):
        if v not in self.elist:
            print("Error: Given node is not found.")
            exit()
        if e_i < 0 or len(self.E) <= e_i:
            print("Error: Given hyperedge is not found.")
            exit()

        if e_i not in self.elist[v]:
            print("Error: Given node is not included in the given hyperedge.")
            exit()
        self.elist[v].remove(e_i)

        if v not in self.E[e_i]:
            print("Error: Given node does not belong to the given hyperedge.")
            exit()
        self.E[e_i].remove(v)

        return 0

    def node_degree(self):

        nd = defaultdict(int)
        for v in self.V:
            nd[v] = int(len(self.elist[v]))

        return nd

    def maximum_node_degree(self):
        mnd = 0
        for v in self.V:
            k = int(len(self.elist[v]))
            if k > mnd:
                mnd = k 

        return mnd

    def num_jnt_node_deg(self):
        jnd = defaultdict(lambda: defaultdict(int))

        for e in self.E:
            s = int(len(e))
            for i in range(0, s-1):
                u = e[i]
                k1 = int(len(self.elist[u]))
                for j in range(i+1, s):
                    v = e[j]
                    k2 = int(len(self.elist[v]))
                    jnd[k1][k2] += 1
                    jnd[k2][k1] += 1

        return jnd

    def node_redundancy_coefficient(self):
        rc = defaultdict(float)
        nd = self.node_degree()

        for v in self.V:
            d = nd[v]

            if d < 2:
                continue

            for i in range(0, d-1):
                e1 = self.elist[v][i]
                for j in range(i+1, d):
                    e2 = self.elist[v][j]

                    s = set(set(self.E[e1]) & set(self.E[e2])) - {v}
                    if len(s) > 0:
                        rc[v] += 2

            rc[v] /= d*(d-1)

        return rc

    def degree_dependent_node_redundancy_coefficient(self):
        rc = self.node_redundancy_coefficient()
        nd = self.node_degree()

        ddrc = defaultdict(float)
        n_k = defaultdict(int)

        for v in self.V:
            k = nd[v]
            ddrc[k] += rc[v]
            n_k[k] += 1

        for k in ddrc:
            if n_k[k] > 0:
                ddrc[k] = float(ddrc[k])/n_k[k]

        return ddrc

    def hyperedge_size(self):
        hs = defaultdict(int)

        for e_i in range(0, len(self.E)):
            hs[e_i] = len(self.E[e_i])

        return hs