/*!
Copyright (C) 2014, 申瑞珉 (Ruimin Shen)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
@Article{,
  Title                    = {Quantitative function for community detection},
  Author                   = {Zhenping Li and Shihua Zhang and Rui-Sheng Wang and Xiang-Sun Zhang and Luonan Chen},
  Journal                  = {Physical Review E},
  Year                     = {2008},
  Number                   = {3},
  Pages                    = {036109},
  Volume                   = {77},
  Doi                      = {10.1103/PhysRevE.77.036109}
}
*/

#pragma once

#include <cassert>
#include "Metric.h"
#include "Utility.h"

namespace otl
{
namespace problem
{
namespace community_discovery
{
namespace metric
{
template <typename _TReal, typename _TMatrix>
class QLi : public Metric<_TReal, _TMatrix>
{
public:
	typedef _TReal TReal;
	typedef _TMatrix TMatrix;
	typedef Metric<TReal, TMatrix> TSuper;

	QLi(void);
	~QLi(void);

protected:
	TReal _DoEvaluate(const TMatrix &graph, const std::vector<std::set<size_t> > &communities);
};

template <typename _TReal, typename _TMatrix>
QLi<_TReal, _TMatrix>::QLi(void)
	: TSuper(true)
{
}

template <typename _TReal, typename _TMatrix>
QLi<_TReal, _TMatrix>::~QLi(void)
{
}

template <typename _TReal, typename _TMatrix>
_TReal QLi<_TReal, _TMatrix>::_DoEvaluate(const TMatrix &graph, const std::vector<std::set<size_t> > &communities)
{
	TReal q = 0;
	for (size_t i = 0; i < communities.size(); ++i)
	{
		const std::set<size_t> &community = communities[i];
		const TReal innerDegree = CommunityInnerDegree(graph, community);
		const TReal outerDegree = CommunityOuterDegree(graph, communities, i);
		q += (innerDegree - outerDegree) / community.size();
	}
	return q;
}
}
}
}
}
