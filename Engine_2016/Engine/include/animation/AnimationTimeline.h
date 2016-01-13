#ifndef ANIMATIONTIMELINE_H_
#define ANIMATIONTIMELINE_H_

#include <core/Object.h>

namespace animation
{
	/**
	 * AnimationTimeline keeps track of time of the animation. Depending on the application, there might have several AnimationTimeline-objects.
	 * AnimationTimeline object is used to control timing of AnimationTrack and in case of shared AnimationTimeline between AnimationTracks,
	 * those AnimationTracks run in same time.
	 */
	class AnimationTimeline : public core::Object
	{
	public:
		enum RepeatMode
		{
			CONSTANT	= 1, // Plays only once
			LOOP		= -1  // Looping mode			
		};

		/** Constructs AnimationTimeline with maximum time. */
		AnimationTimeline(float maxTime, int initialRepeatMode = CONSTANT, float initialSpeed = 1.0f);

		virtual ~AnimationTimeline();

		// Sets speed of the animation. 0 = pause, 1 = forward, -1 = reverse, 2 = double speed etc...
		void setSpeed(float speed);
		
		// Sets current time in animation
		void setCurrentTime(float currentTime);
	
		// void sets repeat mode
		void setRepeatMode(int repeatMode);

		// Call this on update. Progresses animation according to given delta time and speed.
		void update(float deltaTime);

		// Returns scaled time of animation. Dependent on speed etc.
		float getCurrentTime() const;

		// Returns maximum time of the animation, if animation is in CONSTANT-repeat mode. This method must not be called in LOOP-repeat mode.
		float getMaxTime() const;

		// Returns true, if animation has ended (current time > maxTime) in CONSTANT-repeat mode. In case of LOOP-repeat mode, returns false always.
		bool hasEnded() const;

	private:
		RepeatMode		m_repeatMode;
		float			m_maxTime;
		float			m_speed;
		float			m_currentTime;

		AnimationTimeline();
		AnimationTimeline(const AnimationTimeline&);
		AnimationTimeline& operator=(const AnimationTimeline&);
	};


}


#endif

