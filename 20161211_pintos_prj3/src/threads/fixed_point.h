#define F (1<<14)

int int_to_fp(int n) {
	return n * F;
}

int round_toward_zero(int x){
	return x / F;
}

int round_to_nearest(int x) {
	if (x >= 0)
		return (x + F / 2) / F;
	else
		return (x - F / 2) / F;
}

int fp_add_fp(int x, int y) {
	return x + y;
}

int fp_sub_fp(int x, int y) {
	return x - y;
}

int fp_add_int(int x, int n) {
	return x + n * F;
}

int fp_sub_int(int x, int n) {
	return x - n * F;
}

int fp_mul_fp(int x, int y) {
	return ((int64_t)x * y) / F;
}

int fp_mul_int(int x, int n) {
	return x * n;
}

int fp_div_fp(int x, int y) {
	return ((int64_t)x * F) / y;
}

int fp_div_int(int x, int n) {
	return x / n;
}


