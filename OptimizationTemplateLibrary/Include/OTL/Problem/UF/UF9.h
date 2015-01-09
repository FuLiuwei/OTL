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

#include <cassert>
#include <vector>
#include <boost/math/constants/constants.hpp>
#include "UF.h"
#include "Distance/Distance8.h"

namespace otl
{
namespace problem
{
namespace uf
{
template <typename _TReal>
class UF9 : public UF<_TReal>
{
public:
	typedef _TReal TReal;
	typedef UF<TReal> TSuper;
	typedef typename TSuper::TDecision TDecision;
	typedef typename TSuper::TSolution TSolution;
	typedef typename TSuper::TRange TRange;
	typedef typename TSuper::TBoundary TBoundary;

	UF9(const size_t nDistDecisions = 28, const TReal epsilon = 0.1);
	~UF9(void);
	static TBoundary GenerateDecisionSpace(const size_t dimension);

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);

private:
	TReal epsilon_;
};

template <typename _TReal>
UF9<_TReal>::UF9(const size_t nDistDecisions, const TReal epsilon)
	: TSuper(3, GenerateDecisionSpace(2 + nDistDecisions))
	, epsilon_(epsilon)
{
}

template <typename _TReal>
UF9<_TReal>::~UF9(void)
{
}

template <typename _TReal>
typename UF9<_TReal>::TBoundary UF9<_TReal>::GenerateDecisionSpace(const size_t dimension)
{
	TBoundary boundary(dimension);
	for (size_t i = 0; i < 2; ++i)
	{
		boundary[i].first = 0;
		boundary[i].second = 1;
	}
	for (size_t i = 2; i < boundary.size(); ++i)
	{
		boundary[i].first = -2;
		boundary[i].second = 2;
	}
	return boundary;
}

template <typename _TReal>
size_t UF9<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void UF9<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	objective.resize(TSuper::GetNumberOfObjectives());
	const TReal x = decision[0];
	const TReal y = decision[1];
	TReal temp = (1 + epsilon_) * (1 - 4 * (2 * x - 1) * (2 * x - 1));
	if(temp < 0)
		temp = 0;
	objective[0] = (temp + 2 * x) * y / 2 + 2 * distance::Distance8(decision, 3);
	objective[1] = (temp - 2 * x + 2) * y / 2 + 2 * distance::Distance8(decision, 4);
	objective[2] = 1 - y + 2 * distance::Distance8(decision, 2);
}
}
}
}
