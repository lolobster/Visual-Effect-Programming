#ifndef ANIMATIONTRACK_H_
#define ANIMATIONTRACK_H_



#include <animation/KeyframeSequence.h>
#include <animation/AnimationTimeline.h>

namespace animation
{
	/**
	 * AnimationTrack composes several matters together inorder to automate animation time mapping to some target property of an object.
	 * 
	 * AnimationTrack has always some (game)object attached to it. Each animation track controls certain target property of the object, 
	 * like position, rotation, transparency etc. vie targetProperty-setter function.
	 *
	 * There can be serveral Keyframes, which is used to control the target property of an object. These single keyframe values are take
	 * into account, when final target value is calculated. Actual final target value is calculated as weighted sum of each value of keyframe.
	 */
	template<class ValueType, class TargetObjectType>
	class AnimationTrack : public core::Object
	{
	public:
		typedef void (TargetObjectType::*TargetValueSetterMethodType1)(const ValueType&);
		typedef void (TargetObjectType::*TargetValueSetterMethodType2)(ValueType);

		inline AnimationTrack(AnimationTimeline* timeline, TargetObjectType* targetObject, TargetValueSetterMethodType1 targetProperty);
		inline AnimationTrack(AnimationTimeline* timeline, TargetObjectType* targetObject, TargetValueSetterMethodType2 targetProperty);
		
		virtual ~AnimationTrack();

		/**
		 * Adds new keyframe sequence with specifiec blend weight.
		 */
		inline void addKeyframes( KeyframeSequence<ValueType>* keyframes, float weight = 1.0f );
		inline KeyframeSequence<ValueType>* addKeyframes( const Keyframe<ValueType>* const data, size_t len, float weight = 1.0f );

		/**
		 * Calculates each keyframe value from KeyframeSequences according to time of AnimationTimeline object. 
		 * Calculates weighted sum of each keyframe values and set it to given target object using tergetProperty-method pointer.
		 */
		inline void applyValues();

	private:
		typedef eastl::pair<float, core::Ref<KeyframeSequence<ValueType> > > KeyframeWeight;

		AnimationTimeline*				m_timeline;
		eastl::vector<KeyframeWeight>	m_keyframes;
		TargetObjectType*				m_targetObject;
		TargetValueSetterMethodType1	m_targetProperty1;
		TargetValueSetterMethodType2	m_targetProperty2;

		AnimationTrack();
		AnimationTrack(const AnimationTrack&);
		AnimationTrack& operator=(const AnimationTrack&);
	};

}

#include <animation/AnimationTrack.inl>

#endif

