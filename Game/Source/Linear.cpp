#include "Linear.h"

namespace T3{
	CLinear::CLinear() :
		m_fMin(0.0f),
		m_fMax(0.0f),
		m_fStep(1.0f),
		m_fValue(0.0f)
	{}

	CLinear::CLinear(const float32 fMin, const float32 fMax) :
		m_fMin(fMin),
		m_fMax(fMax),
		m_fValue(fMin),
		m_fStep(1.0f)
	{}

	CLinear::CLinear(const float32 fMin, const float32 fMax, const float32 fStep) :
		m_fMin(fMin),
		m_fMax(fMax),
		m_fValue(fMin),
		m_fStep(fStep)
	{}

	void	CLinear::Reset(){
		this->m_fValue = this->m_fMin;
	}

	void	CLinear::Fill(){
		this->m_fValue = this->m_fMax;
	}

	const float32	CLinear::Increment(const float32 fDelta){
		this->Update(fDelta);
		return this->m_fValue;
	}

	const float32	CLinear::Decrement(const float32 fDelta){
		this->Update(-fDelta);
		return this->m_fValue;
	}

	const float32	CLinear::GetValue() const{
		return this->m_fValue;
	}

	const bool	CLinear::IsMin() const{
		return this->m_fValue <= this->m_fMin;
	}

	const bool	CLinear::IsMax() const{
		return this->m_fValue >= this->m_fMax;
	}

	void	CLinear::Update(const float32 fTD){
		this->m_fValue += (this->m_fMax - this->m_fMin) * fTD * this->m_fStep;
		if(this->m_fValue > this->m_fMax){
			this->m_fValue = this->m_fMax;
		}
		if(this->m_fValue < this->m_fMin){
			this->m_fValue = this->m_fMin;
		}
	}
}