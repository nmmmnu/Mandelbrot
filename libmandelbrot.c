#include "libmandelbrot.h"

#define ESCAPE2 4.0 /* square of mandelbrot escape */


// Coloring

inline color convertColorClassic(const Mandelbrot *m, color c){
	return m->iter - c;
}

inline color convertColorReverse(const Mandelbrot *m, color c){
	return c == m->iter ? 0 : c;
}

inline color convertColorCyclic(const Mandelbrot *m, color c){
	static const uint size2 = 64;
	static const uint size  = 64 / 2;

	int a = c == m->iter ? 0 : c % size2;

	if (a > size)
		a = size2 - a;

	return a * (m->iter / (double) size);
}

inline color convertColorCosmos(const Mandelbrot *m, color c){
	return c & 1 ? m->iter : 0;
}

inline color convertColorCosmosMix(const Mandelbrot *m, color c){
	return c & 1 ? convertColorClassic(m, c) : 0;
}

inline color convertColorNone(const Mandelbrot *m, color c){
	return c < m->iter ? m->iter : 0;
}

inline color convertColor(const Mandelbrot *m, color c){
	switch(m->colorscheme){
	case MANDELBROT_COLOR_CLASSIC:		return convertColorClassic(m, c);
	case MANDELBROT_COLOR_COSMOS:		return convertColorCosmos(m, c);
	case MANDELBROT_COLOR_COSMOSMIX:	return convertColorCosmosMix(m, c);
	case MANDELBROT_COLOR_REVERSE:		return convertColorReverse(m, c);
	case MANDELBROT_COLOR_CYCLIC:		return convertColorCyclic(m, c);
	case MANDELBROT_COLOR_NONE:		return convertColorNone(m, c);
	}

	return convertColorClassic(m, c);
}

// EO Coloring


// Resolution functions

inline float getResolution(float screen, float width){
	return width / (screen - 1);
}

inline float convertX(const Mandelbrot *m, uint x){
	return m->x + m->resx * x;
}

inline float convertY(const Mandelbrot *m, uint y){
	return m->y + m->resy * y;
}

// EO Resolution functions

#define DO_ABS(a)	if (a < 0) a = -a
#define DO_ABSR(a)	if (a > 0) a = -a

inline static float ABS(float a){
	return a > 0 ? a : -a;
}

inline static void SWAP(double *a, double *b){
	double t = *a;
	*a = *b;
	*b = t;
}

// Z itself

static color Z_mandelbrot(float x, float y, color it){
	float zx = 0;
	float zy = 0;

	color i;
	for(i = 0; i < it; ++i){
		float zx2 = zx * zx;
		float zy2 = zy * zy;

		if (zx2 + zy2 > ESCAPE2)
			return i;

		// z = z*z + c

		zy = 2 * zx * zy + y;
		zx = zx2 - zy2 + x;
	}

	return i;
}

static color Z_burningship(float x, float y, color it){
	float zx = 0;
	float zy = 0;

	color i;
	for(i = 0; i < it; ++i){
		DO_ABS(zx);
		DO_ABS(zy);

		float zx2 = zx * zx;
		float zy2 = zy * zy;

		if (zx2 + zy2 > ESCAPE2)
			return i;

		// z = z*z + c

		zy = 2 * zx * zy + y;
		zx = zx2 - zy2 + x;
	}

	return i;
}

static color Z_perpendicularburningship(float x, float y, color it){
	float zx = 0;
	float zy = 0;

	color i;
	for(i = 0; i < it; ++i){
		DO_ABS(zy);

		float zx2 = zx * zx;
		float zy2 = zy * zy;

		if (zx2 + zy2 > ESCAPE2)
			return i;

		// z = z*z + c

		zy = 2 * zx * zy + y;
		zx = zx2 - zy2 + x;
	}

	return i;
}

static color Z_perpendicularmandelbrot(float x, float y, color it){
	float zx = 0;
	float zy = 0;

	color i;
	for(i = 0; i < it; ++i){
		DO_ABSR(zx);

		float zx2 = zx * zx;
		float zy2 = zy * zy;

		if (zx2 + zy2 > ESCAPE2)
			return i;

		// z = z*z + c

		zy = 2 * zx * zy + y;
		zx = zx2 - zy2 + x;
	}

	return i;
}

static color Z_perpendicularbuffaloreal(float x, float y, color it){
	float zx = 0;
	float zy = 0;

	color i;
	for(i = 0; i < it; ++i){
	//	DO_ABS(zx);
	//	DO_ABS(zy);

		float zx2 = zx * zx;
		float zy2 = zy * zy;

		if (zx2 + zy2 > ESCAPE2)
			return i;

		// z = z*z + c

		zy = 2 * zx * zy - zy + y; 
		zx = zx2 - zy2 - zx + x;
	}

	return i;
}

static color Z_perpendicularbuffalo(float x, float y, color it){
	float zx = 0;
	float zy = 0;

	color i;
	for(i = 0; i < it; ++i){
		DO_ABS(zy);

		float zx2 = zx * zx;
		float zy2 = zy * zy;

		if (zx2 + zy2 > ESCAPE2)
			return i;

		// z = z*z + c

		zy = 2 * zx * zy + y;
		zx = ABS(zx2 - zy2) + x;
	}

	return i;
}

static color Z_celtic(float x, float y, color it){
	float zx = 0;
	float zy = 0;

	color i;
	for(i = 0; i < it; ++i){
		float zx2 = zx * zx;
		float zy2 = zy * zy;

		if (zx2 + zy2 > ESCAPE2)
			return i;

		// z = z*z + c

		zy = - 2 * zx * zy + y;
		zx = ABS(zx2 - zy2) + x;
	}

	return i;
}

static color Z_perpendicularceltic(float x, float y, color it){
	float zx = 0;
	float zy = 0;

	color i;
	for(i = 0; i < it; ++i){
		DO_ABSR(zx);

		float zx2 = zx * zx;
		float zy2 = zy * zy;

		if (zx2 + zy2 > ESCAPE2)
			return i;

		// z = z*z + c

		zy = 2 * zx * zy + y;
		zx = ABS(zx2 - zy2) + x;
	}

	return i;
}

static color Z_mandelbar(float x, float y, color it){
	float zx = 0;
	float zy = 0;

	color i;
	for(i = 0; i < it; ++i){
		zy = - zy;

		float zx2 = zx * zx;
		float zy2 = zy * zy;

		if (zx2 + zy2 > ESCAPE2)
			return i;

		// z = z*z + c

		zy = 2 * zx * zy + y;
		zx = zx2 - zy2 + x;
	}

	return i;
}

static color Z(const Mandelbrot *m, float x, float y){
	switch(m->absolute){
	case MANDELBROT_TYPE_MANDELBROT				: return Z_mandelbrot(x, y, m->iter);
	case MANDELBROT_TYPE_BURNINGSHIP			: return Z_burningship(x, y, m->iter);
	case MANDELBROT_TYPE_PERPENDICULAR_BURNINGSHIP		: return Z_perpendicularburningship(x, y, m->iter);
	case MANDELBROT_TYPE_PERPENDICULAR_MANDELBROT		: return Z_perpendicularmandelbrot(x, y, m->iter);
	case MANDELBROT_TYPE_CELTIC				: return Z_celtic(x, y, m->iter);
	case MANDELBROT_TYPE_PERPENDICULAR_CELTIC		: return Z_perpendicularceltic(x, y, m->iter);
	case MANDELBROT_TYPE_PERPENDICULAR_BUFFALO		: return Z_perpendicularbuffalo(x, y, m->iter);
	case MANDELBROT_TYPE_PERPENDICULAR_BUFFALO_ORGIGINAL	: return Z_perpendicularbuffaloreal(x, y, m->iter);
	case MANDELBROT_TYPE_MANDELBAR				: return Z_mandelbar(x, y, m->iter);
	}
	
	return Z_mandelbrot(x, y, m->iter);;
}

// EO Z itself


int mandelbrot_generate(const Mandelbrot *m, FILE *F){
	fprintf(F, "P2\n");
	fprintf(F, "%u %u\n", m->scrx, m->scry);
	fprintf(F, "%u\n", m->iter);

	uint x, y;

	for(y = 0; y < m->scry; ++y){
		float yr = convertY(m, y);
		for(x = 0; x < m->scrx; ++x){
			float xr = convertX(m, x);
			color c = Z(m, xr, yr);

			fprintf(F, "%2u ", convertColor(m, c));
		}

		fprintf(F, "\n");
	}

	return 0;
}

int mandelbrot_generate_stdout(const Mandelbrot *m){
	return mandelbrot_generate(m, stdout);
}

const Mandelbrot *mandelbrot_get(Mandelbrot *m,
		uint scrx,      uint scry,
		enum MandelbrotType absolute,	enum MandelbrotColor colorscheme,
		color iter,
		float centerx, float centery,
		float half_widthx
	){

	float half_widthy = half_widthx * scry / scrx;

	m->scrx = scrx;
	m->scry = scry;

	m->x = centerx - half_widthx;
	m->y = centery - half_widthy;

	m->resx = getResolution(scrx, half_widthx * 2);
	m->resy = m->resx;

	m->iter = iter;

	m->absolute = absolute;
	m->colorscheme = colorscheme;

	return m;
}

