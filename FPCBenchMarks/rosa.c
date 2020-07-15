#include <math.h>
#define TRUE 1
#define FALSE 0

double ex0(double u, double v, double T) {
	double t1 = (331.4 + (0.6 * T));
	return ((-t1 * v) / ((t1 + u) * (t1 + u)));
}

double ex1(double u, double v, double T) {
	double t1 = (331.4 + (0.6 * T));
	return ((-t1 * v) / ((t1 + u) * (t1 + u)));
}

double ex2(double u, double v, double T) {
	double t1 = (331.4 + (0.6 * T));
	return ((-t1 * v) / ((t1 + u) * (t1 + u)));
}

double ex3(double x1, double x2, double x3) {
	return (((-(x1 * x2) - ((2.0 * x2) * x3)) - x1) - x3);
}

double ex4(double x1, double x2, double x3) {
	return (((((((2.0 * x1) * x2) * x3) + ((3.0 * x3) * x3)) - (((x2 * x1) * x2) * x3)) + ((3.0 * x3) * x3)) - x2);
}

double ex5(double x1, double x2) {
	double t = ((((3.0 * x1) * x1) + (2.0 * x2)) - x1);
	double t_42_ = ((((3.0 * x1) * x1) - (2.0 * x2)) - x1);
	double d = ((x1 * x1) + 1.0);
	double s = (t / d);
	double s_42_ = (t_42_ / d);
	return (x1 + (((((((((2.0 * x1) * s) * (s - 3.0)) + ((x1 * x1) * ((4.0 * s) - 6.0))) * d) + (((3.0 * x1) * x1) * s)) + ((x1 * x1) * x1)) + x1) + (3.0 * s_42_)));
}

double ex6(double v, double w, double r) {
	return (((3.0 + (2.0 / (r * r))) - (((0.125 * (3.0 - (2.0 * v))) * (((w * w) * r) * r)) / (1.0 - v))) - 4.5);
}

double ex7(double v, double w, double r) {
	return (((6.0 * v) - (((0.5 * v) * (((w * w) * r) * r)) / (1.0 - v))) - 2.5);
}

double ex8(double v, double w, double r) {
	return (((3.0 - (2.0 / (r * r))) - (((0.125 * (1.0 + (2.0 * v))) * (((w * w) * r) * r)) / (1.0 - v))) - 0.5);
}

double ex9(double x) {
	double r = 4.0;
	double K = 1.11;
	return ((r * x) / (1.0 + (x / K)));
}

double ex10(double x) {
	double r = 4.0;
	double K = 1.11;
	return (((r * x) * x) / (1.0 + ((x / K) * (x / K))));
}

double ex11(double v) {
	double p = 35000000.0;
	double a = 0.401;
	double b = 4.27e-05;
	double t = 300.0;
	double n = 1000.0;
	double k = 1.3806503e-23;
	return (((p + ((a * (n / v)) * (n / v))) * (v - (n * b))) - ((k * n) * t));
}

double ex12(double x) {
	return (((x - (((x * x) * x) / 6.0)) + (((((x * x) * x) * x) * x) / 120.0)) - (((((((x * x) * x) * x) * x) * x) * x) / 5040.0));
}

double ex13(double x) {
	return ((((1.0 + (0.5 * x)) - ((0.125 * x) * x)) + (((0.0625 * x) * x) * x)) - ((((0.0390625 * x) * x) * x) * x));
}

double ex14(double x) {
	return ((0.954929658551372 * x) - (0.12900613773279798 * ((x * x) * x)));
}

double ex15(double c) {
	double a = 3.0;
	double b = 3.5;
	double discr = ((b * b) - ((a * c) * 4.0));
	double temp;
	if ((((b * b) - (a * c)) > 10.0)) {
		double temp_1;
		if ((b > 0.0)) {
			temp_1 = ((c * 2.0) / (-b - sqrt(discr)));
		} else {
			double temp_2;
			if ((b < 0.0)) {
				temp_2 = ((-b + sqrt(discr)) / (a * 2.0));
			} else {
				temp_2 = ((-b + sqrt(discr)) / (a * 2.0));
			}
			temp_1 = temp_2;
		}
		temp = temp_1;
	} else {
		temp = ((-b + sqrt(discr)) / (a * 2.0));
	}
	return temp;
}

double ex16(double x) {
	double temp;
	if ((((x * x) - x) >= 0.0)) {
		temp = (x / 10.0);
	} else {
		temp = ((x * x) + 2.0);
	}
	return temp;
}

double ex17(double x) {
	double temp;
	if ((x < 1e-05)) {
		temp = (1.0 + (0.5 * x));
	} else {
		temp = sqrt((1.0 + x));
	}
	return temp;
}

double ex18(double x) {
	double temp;
	if ((x < 0.0001)) {
		temp = (1.0 + (0.5 * x));
	} else {
		temp = sqrt((1.0 + x));
	}
	return temp;
}

double ex19(double a, double b, double c) {
	double s = (((a + b) + c) / 2.0);
	return sqrt((((s * (s - a)) * (s - b)) * (s - c)));
}

double ex20(double a, double b, double c) {
	double s = (((a + b) + c) / 2.0);
	return sqrt((((s * (s - a)) * (s - b)) * (s - c)));
}

double ex21(double a, double b, double c) {
	double s = (((a + b) + c) / 2.0);
	return sqrt((((s * (s - a)) * (s - b)) * (s - c)));
}

double ex22(double a, double b, double c) {
	double s = (((a + b) + c) / 2.0);
	return sqrt((((s * (s - a)) * (s - b)) * (s - c)));
}

double ex23(double a, double b, double c) {
	double s = (((a + b) + c) / 2.0);
	return sqrt((((s * (s - a)) * (s - b)) * (s - c)));
}

double ex24(double a, double b, double c) {
	double s = (((a + b) + c) / 2.0);
	return sqrt((((s * (s - a)) * (s - b)) * (s - c)));
}

double ex25(double a, double b, double c) {
	double s = (((a + b) + c) / 2.0);
	return sqrt((((s * (s - a)) * (s - b)) * (s - c)));
}

double ex26(double a, double b, double c) {
	double s = (((a + b) + c) / 2.0);
	return sqrt((((s * (s - a)) * (s - b)) * (s - c)));
}

double ex27(double a, double b, double c) {
	double s = (((a + b) + c) / 2.0);
	return sqrt((((s * (s - a)) * (s - b)) * (s - c)));
}

double ex28(double a, double b, double c) {
	double s = (((a + b) + c) / 2.0);
	return sqrt((((s * (s - a)) * (s - b)) * (s - c)));
}

double ex29(double a, double b, double c) {
	double s = (((a + b) + c) / 2.0);
	return sqrt((((s * (s - a)) * (s - b)) * (s - c)));
}

double ex30(double a, double b, double c) {
	double s = (((a + b) + c) / 2.0);
	return sqrt((((s * (s - a)) * (s - b)) * (s - c)));
}

double ex31(double a, double b, double c) {
	double s = (((a + b) + c) / 2.0);
	return sqrt((((s * (s - a)) * (s - b)) * (s - c)));
}

double ex32(double u) {
	return (-((u * u) * u) / 6.0);
}

double ex33(double a, double b, double c) {
	double temp;
	if ((a < b)) {
		temp = (sqrt(((((c + (b + a)) * (a - (c - b))) * (a + (c - b))) * (c + (b - a)))) / 4.0);
	} else {
		temp = (sqrt(((((c + (a + b)) * (b - (c - a))) * (b + (c - a))) * (c + (a - b)))) / 4.0);
	}
	return temp;
}

double ex34(double x0, double y0, double z0, double vx0, double vy0, double vz0) {
	double dt = 0.1;
	double solarMass = 39.47841760435743;
	double x_1 = x0;
	double y_1 = y0;
	double z_1 = z0;
	double vx_1 = vx0;
	double vy_1 = vy0;
	double vz_1 = vz0;
	double i = 0.0;
	int test = (i < 100.0);
	while (test) {
		double distance = sqrt((((x_1 * x_1) + (y_1 * y_1)) + (z_1 * z_1)));
		double mag = (dt / ((distance * distance) * distance));
		double vxNew = (vx_1 - ((x_1 * solarMass) * mag));
		double x_2 = (x_1 + (dt * vxNew));
		double distance_1 = sqrt((((x_1 * x_1) + (y_1 * y_1)) + (z_1 * z_1)));
		double mag_1 = (dt / ((distance_1 * distance_1) * distance_1));
		double vyNew = (vy_1 - ((y_1 * solarMass) * mag_1));
		double y_2 = (y_1 + (dt * vyNew));
		double distance_2 = sqrt((((x_1 * x_1) + (y_1 * y_1)) + (z_1 * z_1)));
		double mag_2 = (dt / ((distance_2 * distance_2) * distance_2));
		double vzNew = (vz_1 - ((z_1 * solarMass) * mag_2));
		double z_2 = (z_1 + (dt * vzNew));
		double distance_3 = sqrt((((x_1 * x_1) + (y_1 * y_1)) + (z_1 * z_1)));
		double mag_3 = (dt / ((distance_3 * distance_3) * distance_3));
		double vx_2 = (vx_1 - ((x_1 * solarMass) * mag_3));
		double distance_4 = sqrt((((x_1 * x_1) + (y_1 * y_1)) + (z_1 * z_1)));
		double mag_4 = (dt / ((distance_4 * distance_4) * distance_4));
		double vy_2 = (vy_1 - ((y_1 * solarMass) * mag_4));
		double distance_5 = sqrt((((x_1 * x_1) + (y_1 * y_1)) + (z_1 * z_1)));
		double mag_5 = (dt / ((distance_5 * distance_5) * distance_5));
		double vz_2 = (vz_1 - ((z_1 * solarMass) * mag_5));
		double i_1 = (i + 1.0);
		x_1 = x_2;
		y_1 = y_2;
		z_1 = z_2;
		vx_1 = vx_2;
		vy_1 = vy_2;
		vz_1 = vz_2;
		i = i_1;
		test = (i < 100.0);	}
	return x_1;
}

double ex35(double t0, double w0, double N) {
	double h = 0.01;
	double L = 2.0;
	double m = 1.5;
	double g = 9.80665;
	double t_1 = t0;
	double w_1 = w0;
	double n = 0.0;
	int test = (n < N);
	while (test) {
		double k1w = ((-g / L) * sin(t_1));
		double k2t = (w_1 + ((h / 2.0) * k1w));
		double t_3 = (t_1 + (h * k2t));
		double k2w = ((-g / L) * sin((t_1 + ((h / 2.0) * w_1))));
		double w_2 = (w_1 + (h * k2w));
		double n_1 = (n + 1.0);
		t_1 = t_3;
		w_1 = w_2;
		n = n_1;
		test = (n < N);	}
	return t_1;
}

double ex36(double x0) {
	double x_1 = x0;
	double i = 0.0;
	int test = (i < 10.0);
	while (test) {
		double x_2 = (x_1 - ((((x_1 - (pow(x_1, 3.0) / 6.0)) + (pow(x_1, 5.0) / 120.0)) + (pow(x_1, 7.0) / 5040.0)) / (((1.0 - ((x_1 * x_1) / 2.0)) + (pow(x_1, 4.0) / 24.0)) + (pow(x_1, 6.0) / 720.0))));
		double i_1 = (i + 1.0);
		x_1 = x_2;
		i = i_1;
		test = (i < 10.0);	}
	return x_1;
}

