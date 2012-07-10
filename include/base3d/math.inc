inline double rad2deg(double r)
{
	return r*180/M_PI;
}

inline double deg2rad(double r)
{
	return r*M_PI/180;
}

inline double clamp(double x, double minx, double maxx)
{
	if (x<minx)
		return minx;
	else if (x>maxx)
		return maxx;
	else
		return x;
}

inline double angle_normalize(double angle)
{
	if (angle>2*M_PI) {
		int i = int(angle/(2*M_PI));
		angle -= i*2*M_PI;
	}
	
	if (angle<0) {
		int i = int(angle/(2*M_PI))-1;
		angle -= i*2*M_PI;
	}
	
	return angle;
}

#define PERMMASK 0xff

inline double _noise_catmull(double t, int size, double *v)
{
	double t2 = t*t, t3 = t2*t;
	return 0.5*( (2*v[1]) + 
		(-v[0]+v[2])*t +
		(2*v[0]-5*v[1]+4*v[2]-v[3])*t2 +
		(-v[0]+3*v[1]-3*v[2]+v[3])*t3 );
}

