/*!
 * \file
 * \brief Implementation of a class holding an LDPC code
 * \author Manu T S
 *
 * -----------------------------------------------------------------
 *
 * Copyright 2013 IIT Bombay.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 *
 * -----------------------------------------------------------------
 */

#include "cldpc.h"

cldpc::cldpc(const GF2Mat  X) {
    H = X;
    M = H.get_M();
    N = H.get_N();
    G = H.get_G(permute, rank_H);
    K = N - rank_H;
}

cldpc::cldpc(const alist _list) {
    H = GF2Mat(_list);
    M = H.get_M();
    N = H.get_N();
    G = H.get_G(permute, rank_H);
    K = N - rank_H;
}

void cldpc::set_alist(const alist _list) {
    H = GF2Mat(_list);
    M = H.get_M();
    N = H.get_N();
    G = H.get_G(permute, rank_H);
    K = N - rank_H;
}

std::vector<char> cldpc::get_systematic_bits(std::vector<char> in) {
    std::vector<char> data;
    data.resize(K);
    int index;
    for ( int i = 0; i < K; i++ ) {
        index = permute[i + rank_H];
        data[i] = in[index];
    }
    return data;
}

void cldpc::print_permute() {
    for ( int i = 0; i < permute.size(); i++ ) {
        std::cout << permute[i] << ", ";
    }
    std::cout << "\n";
}

std::vector<char> cldpc::syndrome(const std::vector<char> in) {
    std::vector<char> synd;
    synd.resize(rank_H);
    GF2Vec in_bvec;
    in_bvec.set_vec(in);
    for ( int i = 0; i < rank_H; i++ ) {
        synd[i] = H[i]*in_bvec;
    }
    return synd;
}

bool cldpc::is_codeword(const std::vector<char> in) {
    std::vector<char> synd;
    synd = syndrome(in);
    bool is_code;
    is_code = true;
    for ( int i = 0; i < rank_H; i++ ) {
        if ( synd[i] != char(0) ) {
            is_code = false;
        }
    }
    return is_code;
}

std::vector<char> cldpc::encode(std::vector<char> dataword) {
    if (dataword.size() == K) {
        GF2Vec x(N);
        GF2Vec data(K);
        data.set_vec(dataword);
        for ( int i = rank_H; i < N; i++ ) {
            x[i] = dataword[i - rank_H];
        }
        for ( int i = 0; i < rank_H; i++ ) {
            x[i] = G[i].sub_vector(N-K, N)*data;
        }
        GF2Vec y(N);
        for ( int i = 0; i < N; i++ ) {
            y[permute[i]] = x[i];
        }
        return y.get_vec();
    }
}

int cldpc::dimension() {
    return K;
}

int cldpc::get_M() {
    return M;
}

int cldpc::get_N() {
    return N;
}

GF2Mat cldpc::get_H() {
    return H;
}

GF2Mat cldpc::get_G() {
    return G;
}
