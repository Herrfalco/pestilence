/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatcher.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/07 12:04:07 by fcadet            #+#    #+#             */
/*   Updated: 2022/08/29 00:13:52 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void		fn_00(void) { printf("function 00\n"); }
void		fn_01(void) { printf("function 01\n"); }
void		fn_02(void) { printf("function 02\n"); }
void		fn_03(void) { printf("function 03\n"); }
void		fn_04(void) { printf("function 04\n"); }
void		fn_05(void) { printf("function 05\n"); }
void		fn_06(void) { printf("function 06\n"); }
void		fn_07(void) { printf("function 07\n"); }
void		fn_08(void) { printf("function 08\n"); }
void		fn_09(void) { printf("function 09\n"); }
void		fn_10(void) { printf("function 10\n"); }
void		fn_11(void) { printf("function 11\n"); }
void		fn_12(void) { printf("function 12\n"); }
void		fn_13(void) { printf("function 13\n"); }
void		fn_14(void) { printf("function 14\n"); }
void		fn_15(void) { printf("function 15\n"); }
void		fn_16(void) { printf("function 16\n"); }
void		fn_17(void) { printf("function 17\n"); }
void		fn_18(void) { printf("function 18\n"); }
void		fn_19(void) { printf("function 19\n"); }
void		fn_20(void) { printf("function 20\n"); }
void		fn_21(void) { printf("function 21\n"); }
void		fn_22(void) { printf("function 22\n"); }
void		fn_23(void) { printf("function 23\n"); }
void		fn_24(void) { printf("function 24\n"); }
void		fn_25(void) { printf("function 25\n"); }
void		fn_26(void) { printf("function 26\n"); }
void		fn_27(void) { printf("function 27\n"); }
void		fn_28(void) { printf("function 28\n"); }
void		fn_29(void) { printf("function 29\n"); }
void		fn_30(void) { printf("function 30\n"); }
void		fn_31(void) { printf("function 31\n"); }

/*
00111 01111 00110 01011 01001 10000 11101 11000
01000 00000 01101 00011 11100 00001 01010 10111
10001 00101 01100 11010 11111 01110 11011 00010
10110 00100 11001 10011 10101 10010 10100 11110
*/

uint8_t		keys[] = {	0b10111000, 0b11000011, 0b10110100, 0b11001100, 0b00111011,
						0b01010111, 0b00000101, 0b00111110, 0b00011010, 0b01000000,
						0b01100010, 0b10111011, 0b10101111, 0b01011001, 0b10001001,
						0b10011110, 0b11001010, 0b00111010, 0b00110011, 0b10110001 };

int			checker(uint64_t code, uint64_t targ) {
	uint8_t				*k_ptr = (uint8_t *)keys;
	uint64_t			buff;
	uint64_t			line, col;
	
	for (line = 0, buff = *(uint64_t *)k_ptr << 24;
			line < 4;
			++line, k_ptr += 5, buff = *(uint64_t *)k_ptr << 24) {
		for (col = 0; col < 8; ++col) {
			if ((code >> (59 - col * 5) & 0b11111) == (buff >> (59 - col * 5) & 0b11111)
					&& targ != line * 8 + col)
				return (-1);
		}
	}
	return (0);
}

uint64_t	code_gen(uint64_t fn_nb) {
	int				rand_file = open("/dev/urandom", O_RDONLY);
	uint64_t		result, line = fn_nb / 8, col = fn_nb % 8,
					mask = (uint64_t)0b11111 << (59 - col * 5),
					buff = *(uint64_t *)(keys + line * 5) << 24;

	while (42) {
		if (rand_file < 0 || read(rand_file, &result, sizeof(uint64_t)) < 0)
			return (0);
		result &= ~mask;
		result |= buff & mask;
		if (!checker(result, fn_nb))
			break;
	}
	return (result);
}

void		call_fn(uint64_t line, uint64_t col) {
	static		void (*fn[])(void) = {	fn_00, fn_01, fn_02, fn_03, fn_04, fn_05, fn_06, fn_07,
										fn_08, fn_09, fn_10, fn_11, fn_12, fn_13, fn_14, fn_15,
										fn_16, fn_17, fn_18, fn_19, fn_20, fn_21, fn_22, fn_23,
										fn_24, fn_25, fn_26, fn_27, fn_28, fn_29, fn_30, fn_31 };

	fn[line * 8 + col]();
}

void		dispatcher(uint64_t code) {
	uint8_t				*k_ptr = (uint8_t *)keys;
	uint64_t			buff;
	uint64_t			line, col;
	
	for (line = 0, buff = *(uint64_t *)k_ptr << 24;
			line < 4;
			++line, k_ptr += 5, buff = *(uint64_t *)k_ptr << 24) {
		for (col = 0; col < 8; ++col)
			if ((code >> (59 - col * 5) & 0b11111) == (buff >> (59 - col * 5) & 0b11111))
				call_fn(line, col);
	}
}

int				main(void) {
	uint64_t		code;

	for (int i = 0; i < 32; ++i) {
		code = code_gen(i);
		printf("code: %ld -> ", code);
		dispatcher(code_gen(i));
	}
	return (0);
}
