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

#pragma once

#include <cmath>
#include <boost/numeric/ublas/symmetric.hpp>
#include <OTL/Problem/Problem.h>
#include <OTL/Utility/WithSpaceBoundary.h>
#include "Decode.h"

namespace otl
{
namespace problem
{
namespace community_discovery
{
namespace cd1
{
template <typename _TReal>
class CD1 : public Problem<_TReal, std::vector<_TReal> >, public otl::utility::WithSpaceBoundary<_TReal>
{
public:
	typedef _TReal TReal;
	typedef std::vector<size_t> TDecision;
	typedef Problem<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;
	typedef typename otl::utility::WithSpaceBoundary<TReal>::TMinMax TMinMax;
	typedef typename otl::utility::WithSpaceBoundary<TReal>::TBoundary TBoundary;
	typedef boost::numeric::ublas::symmetric_matrix<TReal> TMatrix;
	typedef std::set<size_t> TCommunity;
	typedef std::vector<TCommunity> TCommunities;
	typedef TReal (*TFunction)(const TMatrix &, const TCommunities &);

	CD1(const TMatrix &graph, const std::vector<TFunction> &functions, const std::vector<bool> &maximize);
	~CD1(void);

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _DoFix(std::vector<TReal> &objective);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);

private:
	TMatrix graph_;
	std::vector<TFunction> functions_;
	std::vector<bool> maximize_;
	std::vector<std::vector<size_t> > list_;

	template<class _TArchive> void serialize(_TArchive &archive, const unsigned version);

	friend class boost::serialization::access;
};

template <typename _TReal>
template <typename _TIterator> CD1<_TReal>::CD1(const TMatrix &graph, const std::vector<TFunction> &functions, const std::vector<bool> &maximize)
	: TSuper(functions.size())
	, otl::utility::WithSpaceBoundary<TReal>(graph.size1(), TMinMax(0, graph.size1() - 1))
	, graph_(graph)
	, functions_(functions)
	, maximize_(maximize)
	, list_(MakeOrderedNeighborList(graph))
{
	assert(graph.size1() == graph.size2());
	assert(functions.size() == maximize.size());
}

template <typename _TReal>
CD1<_TReal>::~CD1(void)
{
}

template <typename _TReal>
size_t CD1<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void CD1<_TReal>::_DoFix(std::vector<TReal> &objective)
{
	assert(objective.size() == functions_.size());
	for (size_t i = 0; i < objective.size(); ++i)
	{
		if (functions_[i].second)
			objective[i] = -objective[i];
	}
}

template <typename _TReal>
void CD1<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	auto _communities = Decode(list_, decision);
	TCommunities communities(_communities.begin(), _communities.end());
	objective.resize(functions_.size());
	for (size_t i = 0; i < objective.size(); ++i)
	{
		objective[i] = functions_[i](graph_, communities);
		if (maximize_[i])
			objective[i] = -objective[i];
	}
}

template <typename _TReal>
template<class _TArchive> void CD1<_TReal>::serialize(_TArchive &archive, const unsigned version)
{
	archive & boost::serialization::base_object<TSuper>(*this);
	archive & boost::serialization::base_object<otl::utility::WithSpaceBoundary<TReal> >(*this);
}
}
}
}
}
