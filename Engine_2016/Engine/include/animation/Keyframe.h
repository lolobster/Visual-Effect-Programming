#ifndef KEYFRAME_H_
#define KEYFRAME_H_



namespace animation
{

	/**
	 * Keyframe Declares value to certain time in key frame sequence.
	 */
	template<class T>
	struct Keyframe
	{
	public:
		/**
		 * Constructor. Declares value time pair, which tells what the value should be in wich time on animation.
		 */
		inline Keyframe(const T& v, float t);

		T		value;
		float	time;
	private:
		Keyframe();
	};

}

#include <animation/Keyframe.inl>

#endif

