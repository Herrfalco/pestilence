#include <stdio.h>
#include <stdint.h>
#include <sys/random.h>

#define	MASK	0b1010101010

int		junk_ops(int i, int j, int *ops, uint64_t rand) {
	uint64_t	useless = 0;
	int			usefull = 0;
	uint64_t	lol = 0;
	uint64_t	new_rand;

	for (int count = rand; count > 0; --count) {
		usefull -= lol;
		useless *= useless;
		getrandom(&new_rand, 8, GRND_RANDOM);
		usefull = *ops;
		if (useless == rand)
			useless += count + j - rand;
		else if (usefull + useless == new_rand % 12 && count != 1)
			continue;
		usefull /= rand;
		usefull -= j;
		lol = usefull;
		usefull += i;
		usefull -= rand;
		lol += i;
		useless -= i;
		lol -= rand;
		lol -= i;
		if (count == 1) {
			*ops = usefull;
			return (0);
		}
	}
	return (-1);
}

int		math_ops(int i, int j, int ops) {
	uint64_t	rand = 0;

	while (!rand) {
		getrandom(&rand, 8, GRND_RANDOM);
		rand %= 230;
	}
	ops += rand;
	ops -= i;
	ops += j;
	ops *= rand;
	junk_ops(i, j, &ops, rand);
	if (((ops & MASK) == (559 & MASK))
		|| ((ops & MASK) == (949 & MASK))
		|| ((ops & MASK) == (176 & MASK)))
		return (i + j);
	else if (((ops & MASK) == (920 & MASK))
		|| ((ops & MASK) == (503 & MASK))
		|| ((ops & MASK) == (843 & MASK)))
		return (i - j);
	else if (((ops & MASK) == (773 & MASK))
		|| ((ops & MASK) == (400 & MASK))
		|| ((ops & MASK) == (982 & MASK)))
		return (i * j);
	else
		return (-1);
}

int		main(void) {
	int		i = 10;
	int		j = 5;
	int		ret;

	ret = math_ops(i, j, 784);
	printf("%d\n", ret);

	return (0);
}
