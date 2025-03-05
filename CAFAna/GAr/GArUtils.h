#pragma once

#include "CAFAna/GAr/GArParams.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"
#include "duneanaobj/StandardRecord/SRVector3D.h"
#include "duneanaobj/StandardRecord/SRLorentzVector.h"

#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

// std::hash is not specialized for tuples
// However, if your tuple consists of standard hashable types like string
// and int the following code will automatically add such support in c++11
namespace std{
    namespace
    {

        // Code from boost
        // Reciprocal of the golden ratio helps spread entropy
        //     and handles duplicates.
        // See Mike Seymour in magic-numbers-in-boosthash-combine:
        //     https://stackoverflow.com/questions/4948780

        template <class T>
        inline void hash_combine(std::size_t& seed, T const& v)
        {
            seed ^= hash<T>()(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }

        // Recursive template code derived from Matthieu M.
        template <class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
        struct HashValueImpl
        {
          static void apply(size_t& seed, Tuple const& tuple)
          {
            HashValueImpl<Tuple, Index-1>::apply(seed, tuple);
            hash_combine(seed, get<Index>(tuple));
          }
        };

        template <class Tuple>
        struct HashValueImpl<Tuple,0>
        {
          static void apply(size_t& seed, Tuple const& tuple)
          {
            hash_combine(seed, get<0>(tuple));
          }
        };
    }

    template <typename ... TT>
    struct hash<std::tuple<TT...>> 
    {
        size_t
        operator()(std::tuple<TT...> const& tt) const
        {                                              
            size_t seed = 0;                             
            HashValueImpl<std::tuple<TT...> >::apply(seed, tt);    
            return seed;                                 
        }                                              

    };
}

namespace ana
{

    /* -------------------------------------------------------------------------- */
    /*                                 Operations                                 */
    /* -------------------------------------------------------------------------- */

    /**
    * Generates a linearly spaced vector of doubles.
    *
    * @param start The starting value of the sequence.
    * @param end The ending value of the sequence.
    * @param num_in The number of elements in the sequence.
    *               - If 0, returns an empty vector.
    *               - If 1, returns a vector with a single element `start_in`.
    * @return A vector of `num_in` linearly spaced values from `start_in` to `end_in`.
    *
    * @note The function ensures that both `start_in` and `end_in` are exactly represented
    *       as the first and last elements of the returned vector, respectively.
    *
    * @example
    * std::vector<double> result = fLinspace(0.0, 1.0, 5);
    * // result = {0.0, 0.25, 0.5, 0.75, 1.0}
    */
    std::vector<double> fLinspace(const double start, const double end, const int num_in);

    /**
    * Computes the centers of adjacent bins in a vector.
    *
    * @param vec A vector of doubles representing bin edges.
    *            Must contain at least two elements.
    * @return A vector of doubles representing the midpoints between 
    *         consecutive elements of the input vector.
    *
    * @note If the input vector has fewer than two elements, the returned 
    *       vector will be empty.
    *
    * @example
    * std::vector<double> edges = {0.0, 1.0, 2.0, 3.0};
    * std::vector<double> centers = fBinCenters(edges);
    * // centers = {0.5, 1.5, 2.5}
    */
    std::vector<double> fBinCenters(const std::vector<double> &vec);
    
    /**
    * Computes the cross product of two 3D vectors.
    *
    * @param a A vector of doubles representing the first 3D vector.
    * @param b A vector of doubles representing the second 3D vector.
    * @return A vector of doubles representing the cross product of `a` and `b`.
    *
    * @note The function assumes the input vectors are in a 3-dimensional space.
    *       The result will also be a 3D vector.
    *
    * @example
    * std::vector<double> vec1 = {1.0, 0.0, 0.0};
    * std::vector<double> vec2 = {0.0, 1.0, 0.0};
    * std::vector<double> result = fCrossProduct(vec1, vec2);
    * // result = {0.0, 0.0, 1.0}
    */
    std::vector<double> fCrossProduct(const std::vector<double> &a, const std::vector<double> &b);

    /* -------------------------------------------------------------------------- */
    /*                              Geometry-related                              */
    /* -------------------------------------------------------------------------- */

    /**
    * Determines if a point is inside a finite cylinder.
    *
    * @param pt1 A vector of doubles representing the first endpoint of the cylinder's axis.
    * @param pt2 A vector of doubles representing the second endpoint of the cylinder's axis.
    * @param r The radius of the cylinder.
    * @param q A vector of doubles representing the point to be checked.
    * @return `true` if the point `q` lies within the cylinder, `false` otherwise.
    *
    * @example
    * std::vector<double> pt1 = {0.0, 0.0, 0.0};
    * std::vector<double> pt2 = {0.0, 0.0, 10.0};
    * double radius = 2.0;
    * std::vector<double> q = {1.0, 0.0, 5.0};
    * bool inside = fPointInCylinder(pt1, pt2, radius, q);
    * // inside = true
    */
    bool fPointInCylinder(const std::vector<double> &pt1, const std::vector<double> &pt2, const double r, const std::vector<double> &q);

    /**
    * Checks if a point lies within the HPgTPC fiducial volume.
    *
    * @param q A vector of doubles representing the point to be checked.
    * @param delta_l A reduction in the length of the fiducial volume on each side wrt total half-length.
    * @param delta_r A reduction in the radius of the fiducial volume wrt total radius.
    * @return `true` if the point lies within the fiducial volume, `false` otherwise.
    *
    * @note This function depends on constants (`kHPgTPCLength`, `kHPgTPCX`, `kHPgTPCY`, 
    *       `kHPgTPCZ`, `kHPgTPCRadius`) to define the fiducial volume dimensions.
    */
    bool fContainedInFiducial(const std::vector<double> &q, const double delta_l = kHPgTPCFidCutX, const double delta_r = kHPgTPCFidCutR);

    /* -------------------------------------------------------------------------- */
    /*                           Vector transformations                           */
    /* -------------------------------------------------------------------------- */
    
    const std::vector<double> fGetVectorStd(const caf::SRVector3DProxy* vec);
    const caf::SRVector3D fGetVector3D(const caf::SRVector3DProxy* vec);
    const caf::SRLorentzVector fGetVectorLorentz(const caf::SRLorentzVectorProxy* vec);
    const float *fArrayFromVectorStd(const std::vector<float> vec);

    /* -------------------------------------------------------------------------- */
    /*                               Object getters                               */
    /* -------------------------------------------------------------------------- */

    std::vector<caf::SRTrueParticleProxy*> fGetTruePDG(const caf::SRProxy* sr, const int pdg, const bool sign = true, const float p_thres = 0.0);
    
    caf::SRTrueParticleProxy* fGetTrueParticleFromRecoParticle(caf::SRTrueInteractionProxy* true_interaction, caf::SRRecoParticleProxy* part);

}