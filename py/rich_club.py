import numpy as np
import hypergraph
import randomization
import random
import math

def permutation_test(H_rcc: float, rcc_rands: np.ndarray):
    n_rand = len(rcc_rands)
    mean_rcc = np.average(rcc_rands)
    ori_diff = H_rcc - mean_rcc

    n = 0
    for i in range(0, n_rand):
        x = np.copy(rcc_rands)
        a = rcc_rands[i]
        x[i] = H_rcc

        y = np.average(x)
        permutated_diff = a - y

        if permutated_diff > ori_diff:
            n += 1

    p_val = float(n) / n_rand

    return p_val

def calc_normalized_rich_club_coefficient(H: hypergraph.HyperGraph, num_rand=1000, min_rcc=5):

    rcc = H.rich_club_coefficient()
    node_degree_set = set(list(rcc.keys()))
    rcc_rand = {k: np.zeros(num_rand) for k in node_degree_set}

    for i in range(0, num_rand):
        H_rand = randomization.hyper_dk(H, d_v="1", d_e="1")
        rcc_ = H_rand.rich_club_coefficient()
        for k in node_degree_set:
            rcc_rand[k][i] = rcc_.get(k, 0)

    norm_rcc, p_val = {}, {}
    for k in node_degree_set:
        if rcc[k] >= min_rcc:
            norm_rcc[k] = float(rcc[k]) / np.average(rcc_rand[k])
            p_val[k] = permutation_test(rcc[k], rcc_rand[k])
        else:
            norm_rcc[k] = -1
            p_val[k] = -1

    return norm_rcc, p_val

def calc_hyperedge_size_dependent_normalized_rich_club_coefficient(H: hypergraph.HyperGraph, hyperedge_size_set: set,
                                                                   num_rand=1000, min_rcc=5):

    hs = H.hyperedge_size()
    min_hs, max_hs = min(list(hs.values())), max(list(hs.values()))
    if len([s for s in hyperedge_size_set if not min_hs <= s <= max_hs]):
        print("ERROR: given set of hyperedge sizes is not valied.")
        return 0

    norm_rcc, p_val = {}, {}
    for s in sorted(list(hyperedge_size_set)):
        H_s = H.s_size_hyperedge_induced_hypergraph(s)
        norm_rcc_, p_val_ = calc_normalized_rich_club_coefficient(H_s, num_rand, min_rcc)
        norm_rcc[s] = norm_rcc_
        p_val[s] = p_val_

    return norm_rcc, p_val