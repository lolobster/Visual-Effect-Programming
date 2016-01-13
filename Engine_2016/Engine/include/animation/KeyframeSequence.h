#ifndef KEYFRAMESEQUENCE_H_
#define KEYFRAMESEQUENCE_H_


#include <core/Object.h>
#include <core/Ref.h>
#include <eastl/vector.h>
#include <animation/Keyframe.h>

namespace animation
{
	/**
	 * Keyframe sequence contains array of time value pairs (keyframes).
	 *
	 * If you need to use same keyframedata in several animation, it is recommended, that you share
	 * same KeyframeSequence-object between animations instead of keeping same data twice in the memory.  
	 */
	template<class T>
	class KeyframeSequence : public core::Object
	{
	public:
		typedef T (*InterpolateFuncType)(const T& v1, const T& v2, float interpolant);

		template<class ValueType>
		static ValueType LERP(const ValueType& v1, const ValueType& v2, float interpolant)
		{
			return (1.0f-interpolant)*v1 + interpolant*v2;
		}

		static slmath::quat LERP(const slmath::quat& v1, const slmath::quat& v2, float interpolant)
		{
			return SLERP(v1,v2,interpolant);
		}

		template<class ValueType>
		static ValueType STEP(const ValueType& v1, const ValueType& v2, float interpolant)
		{
			return v1;
		}

		template<class ValueType>
		static ValueType SLERP(const ValueType& v1, const ValueType& v2, float interpolant)
		{
			return slerp(v1, v2, interpolant);
		}

		/**
		 * Constructs KeyframeSequence from std::vector<Keyframe>. 
		 */
		inline KeyframeSequence(const eastl::vector< Keyframe<T> >& keyframes);

		/**
		 * Constructs KeyframeSequence from c-style array. Copies data to it's internal datastructure, so after construction,
		 * data allocated by caller can be free'ed.
		 */
		inline KeyframeSequence(const Keyframe<T>* const data, size_t len);
		
		virtual ~KeyframeSequence();

		/**
		 * Sets interpolation function used bu this key frame sequence. Can 
		 * be KeyframeSequence::LERP, KeyframeSequence::STEP or KeyframeSequence::SLERP.
		 * You can also provide your own interpolation function type if function is in 
		 * correct format.
		 */
		inline void setInterpolation(InterpolateFuncType func) { m_interpolate = func; }

		/**
		 * Returns interpolated value of keyframe sequence in given time. Time must be in range: 0 <= time <= getDuration()
		 */
		inline T getValue(float time) const;
		
		/**
		 * Returns duration of this KeyframeSequence.
		 */
		inline float getDuration() const;

	private:
		eastl::vector<Keyframe<T> >	m_keyframes;
		InterpolateFuncType			m_interpolate;

		inline void setKeyframes(const Keyframe<T>* const data, size_t len);

		KeyframeSequence();
		KeyframeSequence(const KeyframeSequence&);
		KeyframeSequence& operator=(const KeyframeSequence&);
	};


}

#include <animation/KeyframeSequence.inl>

#endif

