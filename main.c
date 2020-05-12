#include <gmp.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>

typedef uint64_t u64;

#define logb(a, b) ((log(a)) / (log(b)))

/* approx -log10(pi^n/n!) */
static double sin_precision_for_n(u64 n) {
	return -(n * logb(3.2, 10) - 1/log(10) * n * (log(n) - 1));
}

/* n we should go up to for number of decimal digits */
static u64 n_for_precision(u64 digits) {
	double want = (double)digits;
	double lo = 0.0;
	double hi = want;
	double epsilon = 1.0;
	while (hi > lo + epsilon) {
		double mid = (lo + hi) * 0.5;
		double prec = sin_precision_for_n(mid);
		if (prec < want) {
			lo = mid;
		} else {
			hi = mid;
		}
	}
	return (u64)(hi + 20.0); /* add 20 to be safe */
}

int main(int argc, char **argv) {
	u64 original_digits, digits;
	u64 sin_terms;
	mpf_t pi, sin_pi, term, last_pi, desired_error;
	u64 i;

	original_digits = argc < 2 ? 100 : (u64)atol(argv[1]);
	digits = original_digits < 10 ? 10 : original_digits;
	sin_terms = n_for_precision(digits);

	mpf_set_default_prec(digits < 100 ? 333 : (digits * 10) / 3);
	mpf_init(pi);
	mpf_init(sin_pi);
	mpf_init(term);
	mpf_init(desired_error);
	mpf_init(last_pi);

	mpf_set_ui(pi, 3);
	mpf_set_ui(desired_error, 1);
	mpf_div_ui(desired_error, desired_error, 10);
	mpf_pow_ui(desired_error, desired_error, digits);

	for (i = 0; ; ++i) {
		unsigned j;
		mpf_set(last_pi, pi);
		mpf_set(sin_pi, pi);
		mpf_set(term, pi);
		mpf_add(sin_pi, sin_pi, term);
		/* pi is now actually -pi^2 */
		mpf_mul(pi, pi, pi);
		mpf_neg(pi, pi);
		for (j = 3; j < sin_terms; j += 2) {
			mpf_div_ui(term, term, j * (j-1));
			mpf_mul(term, term, pi);
			mpf_add(sin_pi, sin_pi, term);
		}
		mpf_set(pi, sin_pi);
		mpf_sub(last_pi, pi, last_pi);
		/* last_pi is now delta */
		mpf_abs(last_pi, last_pi);
		gmp_fprintf(stderr, "Iteration: %9lu. Delta: %Fe\n",1+(unsigned long)i, last_pi);
		if (mpf_cmp(last_pi, desired_error) < 0)
			break;
	}
	gmp_printf("%.*Ff\n", original_digits, pi);


	return 0;
}
