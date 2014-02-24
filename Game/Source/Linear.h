#pragma once

#include <Types.h>

namespace T3{
	class CLinear{
	private:
		const float32	m_fMin;
		const float32	m_fMax;
		float32	m_fValue;

	public:
		CLinear();
		CLinear(const float32 fMin, const float32 fMax);

		void	Reset();
		void	Fill();

		const float32	Increment(const float32 fDelta);
		const float32	Decrement(const float32 fDelta);

		const float32	GetValue() const;

	private:
		void	Update(const float32 fTD);
	};

}