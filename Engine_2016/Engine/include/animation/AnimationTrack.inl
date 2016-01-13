

namespace animation
{


template<class ValueType, class TargetObjectType>
AnimationTrack<ValueType,TargetObjectType>::AnimationTrack(AnimationTimeline* timeline, TargetObjectType* targetObject, TargetValueSetterMethodType1 targetProperty)
	: Object()
	, m_timeline(timeline)
	, m_keyframes()
	, m_targetObject(targetObject)
	, m_targetProperty1(targetProperty)
	, m_targetProperty2(0)
{
}

template<class ValueType, class TargetObjectType>
AnimationTrack<ValueType,TargetObjectType>::AnimationTrack(AnimationTimeline* timeline, TargetObjectType* targetObject, TargetValueSetterMethodType2 targetProperty)
	: Object()
	, m_timeline(timeline)
	, m_keyframes()
	, m_targetObject(targetObject)
	, m_targetProperty1(0)
	, m_targetProperty2(targetProperty)
{
}

template<class ValueType, class TargetObjectType>
AnimationTrack<ValueType,TargetObjectType>::~AnimationTrack()
{
}

template<class ValueType, class TargetObjectType>
void AnimationTrack<ValueType,TargetObjectType>::addKeyframes( KeyframeSequence<ValueType>* keyframes, float weight )
{
	m_keyframes.push_back( KeyframeWeight(weight,keyframes) );
}

template<class ValueType, class TargetObjectType>
KeyframeSequence<ValueType>* AnimationTrack<ValueType,TargetObjectType>::addKeyframes( const Keyframe<ValueType>* const data, size_t len, float weight )
{
	KeyframeSequence<ValueType>* kfs = new KeyframeSequence<ValueType>(data,len);
	addKeyframes(kfs);
	return kfs;
}

template<class ValueType, class TargetObjectType>
void AnimationTrack<ValueType,TargetObjectType>::applyValues()
{
	//()setPosition( m_kf->getValue( m_timeline->getCurrentTime() ) );
	assert( m_keyframes.size() > 0 ); // Atleast one kf sequence required.
	float curTime = m_timeline->getCurrentTime();
	ValueType val = m_keyframes[0].second->getValue(curTime);
	val *= m_keyframes[0].first;

	for( size_t i=1; i< m_keyframes.size(); ++i )
	{
		ValueType v = m_keyframes[i].second->getValue(curTime);
		v *= m_keyframes[i].first;
		val += v;
	}

	if( m_targetProperty1 != 0 )
	{
		(m_targetObject->*m_targetProperty1)(val);
	}

	if( m_targetProperty2 != 0 )
	{
		(m_targetObject->*m_targetProperty2)(val);
	}
}


}
