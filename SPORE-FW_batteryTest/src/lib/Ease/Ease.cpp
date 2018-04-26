#include "Ease.h"

FadeCurve Ease::_curve;

FadeCurve Ease::useCurve(FadeCurve curve) {
	Ease::_curve = curve;
	return _curve;
}

float Ease::easeIn (float t,float b , float c, float d) {
	switch(_curve) {
		case BACK: {
			float s = 1.70158f;
			float postFix = t/=d;
			return c*(postFix)*t*((s+1)*t - s) + b;
		}
		case BOUNCE:
			return c - easeOut (d-t, 0, c, d) + b;
		case CIRC:
			return -c * (sqrt(1 - (t/=d)*t) - 1) + b;
		case CUBIC:
			return c*(t/=d)*t*t + b;
		case ELASTIC: {
			if (t==0) return b;  if ((t/=d)==1) return b+c;
			float p=d*.3f;
			float a=c;
			float s=p/4;
			float postFix =a*pow(2,10*(t-=1)); // this is a fix, again, with post-increment operators
			return -(postFix * sin((t*d-s)*(2*PI)/p )) + b;
		}
		case EXPO:
			return (t==0) ? b : c * pow(2, 10 * (t/d - 1)) + b;
		case LINEAR:
			return c*t/d + b;
		case QUAD:
			return c*(t/=d)*t + b;
		case QUART:
			return c*(t/=d)*t*t*t + b;
		case QUINT:
			return c*(t/=d)*t*t*t*t + b;
		case SINE:
			return -c * cos(t/d * (PI/2)) + c + b;
		default:
			return c*t/d + b;		// default linear.
	}

}
float Ease::easeOut(float t,float b , float c, float d) {
	switch(_curve) {
		case BACK: {
			float s = 1.70158f;
			return c*((t=t/d-1)*t*((s+1)*t + s) + 1) + b;
		}
		case BOUNCE:
			if ((t/=d) < (1/2.75f)) {
				return c*(7.5625f*t*t) + b;
			} else if (t < (2/2.75f)) {
				float postFix = t-=(1.5f/2.75f);
				return c*(7.5625f*(postFix)*t + .75f) + b;
			} else if (t < (2.5/2.75)) {
					float postFix = t-=(2.25f/2.75f);
				return c*(7.5625f*(postFix)*t + .9375f) + b;
			} else {
				float postFix = t-=(2.625f/2.75f);
				return c*(7.5625f*(postFix)*t + .984375f) + b;
			}
		case CIRC:
			return c * sqrt(1 - (t=t/d-1)*t) + b;
		case CUBIC:
			return c*((t=t/d-1)*t*t + 1) + b;
		case ELASTIC: {
			if (t==0) return b;  if ((t/=d)==1) return b+c;
			float p=d*.3f;
			float a=c;
			float s=p/4;
			return (a*pow(2,-10*t) * sin( (t*d-s)*(2*PI)/p ) + c + b);
		}
		case EXPO:
			return (t==d) ? b+c : c * (-pow(2, -10 * t/d) + 1) + b;
		case LINEAR:
			return c*t/d + b;
		case QUAD:
			return -c *(t/=d)*(t-2) + b;
		case QUART:
			return -c * ((t=t/d-1)*t*t*t - 1) + b;
		case QUINT:
			return c*((t=t/d-1)*t*t*t*t + 1) + b;
		case SINE:
			return c * sin(t/d * (PI/2)) + b;
		default:
			return c*t/d + b;		// default linear.
	}
}

float Ease::easeInOut(float t,float b , float c, float d) {
	switch(_curve) {
		case BACK: {
			float s = 1.70158f;
			if ((t/=d/2) < 1) return c/2*(t*t*(((s*=(1.525f))+1)*t - s)) + b;
			float postFix = t-=2;
			return c/2*((postFix)*t*(((s*=(1.525f))+1)*t + s) + 2) + b;
		}
		case BOUNCE:
			if (t < d/2) return easeIn (t*2, 0, c, d) * .5f + b;
			else return easeOut (t*2-d, 0, c, d) * .5f + c*.5f + b;
		case CIRC:
			if ((t/=d/2) < 1) return -c/2 * (sqrt(1 - t*t) - 1) + b;
			return c/2 * (sqrt(1 - t*(t-=2)) + 1) + b;
		case CUBIC:
			if ((t/=d/2) < 1) return c/2*t*t*t + b;
			return c/2*((t-=2)*t*t + 2) + b;
		case ELASTIC: {
			if (t==0) return b;  if ((t/=d/2)==2) return b+c;
			float p=d*(.3f*1.5f);
			float a=c;
			float s=p/4;

			if (t < 1) {
				float postFix =a*pow(2,10*(t-=1)); // postIncrement is evil
				return -.5f*(postFix* sin( (t*d-s)*(2*PI)/p )) + b;
			}
			float postFix =  a*pow(2,-10*(t-=1)); // postIncrement is evil
			return postFix * sin( (t*d-s)*(2*PI)/p )*.5f + c + b;
		}
		case EXPO:
			if (t==0) return b;
			if (t==d) return b+c;
			if ((t/=d/2) < 1) return c/2 * pow(2, 10 * (t - 1)) + b;
			return c/2 * (-pow(2, -10 * --t) + 2) + b;
		case LINEAR:
			return c*t/d + b;
		case QUAD:
			if ((t/=d/2) < 1) return ((c/2)*(t*t)) + b;
			return -c/2 * (((t-2)*(--t)) - 1) + b;
			/*
			originally return -c/2 * (((t-2)*(--t)) - 1) + b;

			I've had to swap (--t)*(t-2) due to diffence in behaviour in
			pre-increment operators between java and c++, after hours
			of joy
			*/
		case QUART:
			if ((t/=d/2) < 1) return c/2*t*t*t*t + b;
			return -c/2 * ((t-=2)*t*t*t - 2) + b;
		case QUINT:
			if ((t/=d/2) < 1) return c/2*t*t*t*t*t + b;
			return c/2*((t-=2)*t*t*t*t + 2) + b;
		case SINE:
			return -c/2 * (cos(PI*t/d) - 1) + b;
		default:
			return c*t/d + b;		// default linear.
	}
}
