// Copyright (c) 2015, Joseph Mirabel
// Authors: Joseph Mirabel (joseph.mirabel@laas.fr)
//
// This file is part of hpp-manipulation.
// hpp-manipulation is free software: you can redistribute it
// and/or modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version.
//
// hpp-manipulation is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Lesser Public License for more details.  You should have
// received a copy of the GNU Lesser General Public License along with
// hpp-manipulation. If not, see <http://www.gnu.org/licenses/>.

#include <hpp/manipulation/path-optimization/config-optimization.hh>

#include <hpp/util/pointer.hh>

#include <hpp/core/path.hh>
#include <hpp/core/config-projector.hh>
#include <hpp/manipulation/device.hh>
#include <hpp/manipulation/graph/state.hh>
#include <hpp/manipulation/graph/edge.hh>
#include <hpp/manipulation/constraint-set.hh>

namespace hpp {
  namespace manipulation {
    namespace pathOptimization {
      ConfigProjectorPtr_t ConfigOptimizationTraits::getConfigProjector
        (const PathPtr_t& before, const PathPtr_t& after, bool& reverse)
      {
        ConstraintSetPtr_t setB =
          HPP_STATIC_PTR_CAST (ConstraintSet, before->constraints ());
        ConstraintSetPtr_t setA =
          HPP_STATIC_PTR_CAST (ConstraintSet, after->constraints ());
        assert (setA->edge () && setB->edge());
        graph::GraphPtr_t graph = setA->edge()->parentGraph ();
        /// Find if path were computed from init or goal config
        /// There is a nasty case: when build a path fails partially, then you
        /// end up where the edge->to() do not correspond to the node of
        /// path->end(). In that case, path->end () should be in edge->node().
        /// (obviously, in this case, we have edge->isInNodeFrom_ = true)
        const bool reverseB = setB->edge ()->direction(before);
        //const bool reverseA = setA->edge ()->direction(after);

        reverse = reverseB;

        ConfigProjectorPtr_t p = ConfigProjector::create (graph->robot(),
            "intersect_" + setB->edge()->name() + "_" + setA->edge()->name(),
            graph->errorThreshold (), graph->maxIterations ());

        setB->edge()->intersectionConstraint (setA->edge (), p);

        p->rightHandSideFromConfig (before->end ());
        return p;
      }
    } // namespace pathOptimization
  } // namespace manipulation
} // namespace hpp
