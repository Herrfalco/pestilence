/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_count.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 16:45:55 by fcadet            #+#    #+#             */
/*   Updated: 2022/08/04 13:27:27 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/syscall.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>

#define	BUFF_SZ		1024

void	get_full_path(char *s1, char *s2, uint8_t *buff) {
	for (; *s1; ++s1, ++buff)
		*buff = *s1;
	for (; *s2; ++s2, ++buff)
		*buff = *s2;
	*buff = '\0';
}

char	*get_name(char *line, uint64_t size) {
	uint64_t		i;

	for (; size && *line != ' ' && *line != '\t'; ++line, --size);
	for (; size && (*line == ' ' || *line == '\t'); ++line, --size);
	for (i = 0; i < size; ++i) {
		if (line[i] == '\n') {
			line[i] = '\0';
			break;
		}
	}
	return (i == size ? NULL : line);
}

int		str_n_cmp(char *s1, char *s2, int n) {
	for (; *s1 && *s1 == *s2 && --n; ++s1, ++s2);
	return (*s1 - *s2);
}

int			main(void) {
	uint8_t		dirs[BUFF_SZ], path[BUFF_SZ], line[BUFF_SZ];
	char		root_path[] = "/proc/", excl_proc[] = "excl_proc.sh";
	char		*name;
	int64_t		dir_count, read_ret;
	uint16_t	ent_sz;
	uint8_t		*ent_ptr;
	int			proc, status;

	if ((proc = open(root_path, O_RDONLY)) < 0)
		return (1);
	while((dir_count = syscall(SYS_getdents, proc, dirs, BUFF_SZ)) > 0) {
		for (ent_ptr = dirs; dir_count; ent_sz = *(uint16_t *)(ent_ptr + 16),
				dir_count -= ent_sz, ent_ptr += ent_sz) {
			get_full_path(root_path, (char *)(ent_ptr + 18), path);
			get_full_path((char *)path, "/status", path);
			if ((status = open((char *)path, O_RDONLY)) < 0)
				continue;
			if ((read_ret = read(status, line, BUFF_SZ)) < 1
					|| !(name = get_name((char *)line, read_ret))
					|| str_n_cmp(name, excl_proc, 15)) {
				close(status);
				continue;
			}
			printf("%s is running !\n", excl_proc);
			close(status);
			close(proc);
			return (0);
		}
	}
	printf("%s isn't running !\n", excl_proc);
	close(proc);
	return (0);
}
