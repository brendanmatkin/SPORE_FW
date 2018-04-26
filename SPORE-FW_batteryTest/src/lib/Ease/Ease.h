#ifndef EASE_H
#define EASE_H

#include <math.h>

#ifndef PI
#define PI  3.14159265
#endif

enum FadeCurve {
 	BACK,
 	BOUNCE,
 	CIRC,
 	CUBIC,
 	ELASTIC,
 	EXPO,
 	LINEAR,
 	QUAD,
 	QUART,
 	QUINT,
 	SINE
 };

class Ease {

	public:

		static float easeIn(float t,float b , float c, float d);
		static float easeOut(float t,float b , float c, float d);
		static float easeInOut(float t,float b , float c, float d);
		static FadeCurve useCurve(FadeCurve curve);

	private:
		static FadeCurve _curve;
};

#endif /* EASE_H */
