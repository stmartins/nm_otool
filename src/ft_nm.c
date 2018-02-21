#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "../libft/libft.h"
#include <mach-o/loader.h>
#include <mach-o/nlist.h>

int     is_in_alpha_order(char *str, char *cmp)
{
  int len_str;
  int len_cmp;
  int i;

  i = 0;
  len_str = ft_strlen(str);
  len_cmp = ft_strlen(cmp);
  // printf("str %s %s\n", str, cmp);


  while (i < len_str >= len_cmp ? len_cmp : len_str)
  {
    // printf("%c %c\n", str[i], cmp[i]);
    if (str[i] > cmp[i])
      return (1);
    else if (str[i] < cmp[i])
      return (0);
    i++;
  }
  return (0);
}

void	  tab_alpha_order(int *order, struct nlist_64 *array, char *strtab, int last)
{
  int t = 0;
  char *str;
  char *cmp;

  cmp = ft_strdup(strtab + array[last].n_un.n_strx);

  // printf("t=>%d last=>%d\n", order[t], order[last]);

  while (t < last)
  {
    str = ft_strdup(strtab + array[order[t]].n_un.n_strx);
    // printf("str %s %s\n", str, cmp);
    if (is_in_alpha_order(str, cmp))
    {
      ft_swap(&order[t], &order[last]);
      cmp = ft_strdup(strtab + array[order[last]].n_un.n_strx);
      int i = -1;
      // while (++i < 17)
      //   printf("[%d]", order[i]);
      //   printf("\n");
      // printf("tab_order %d\n", order[t]);
      // t = 0;
    }
    t++;
  }
}

int     check_tab_doubl(char *str, char *strtab, struct nlist_64 *array, int *tab, int lim)
{
  int   i;

  i = 0;
  while (i < lim)
  {
    if (!ft_strcmp(str, strtab + array[tab[i]].n_un.n_strx))
      return (1);
    i++;
  }
  return (0);
}
void    print_output(int nsyms, int symoff, int stroff, void *ptr)
{
  int i;
  // int run;
  char *strtab;
  struct nlist_64 *array;
  char type[nsyms];
  char *str;
  int  al_order[nsyms];

  array = ptr + symoff;
  strtab = ptr + stroff;
  i =-1;
  while (++i < nsyms)
    al_order[i] = i;
  i = -1;
  while (++i < nsyms)
  {
    printf("%s\n",array[i].n_desc);

    type[i] = array[i].n_type;
    if ((type[i] & N_STAB))
      type[i] = '-';
    else
    {
      if ((type[i] & N_TYPE) == N_UNDF)
      {
        type[i] = 'u';
        if ((array[i].n_value) != 0)
          type[i] = 'c';
      }
      else if ((type[i] & N_TYPE) == N_PBUD)
         type[i] = 'u';
      else if ((type[i] & N_TYPE) == N_ABS)
        type[i] = 'a';
      else if ((type[i] & N_TYPE) == N_SECT)
      {
        type[i] = '@';
      }
    }
    // printf("\n=====struct nlist_64=======\n ");
    // int ti = -1;
    // while (++ti < nsyms)
    //   printf("[%d]", al_order[ti]);
    if (i > 0)
      tab_alpha_order(al_order, array, strtab, i);
    // printf(" n_sect %hhu\n ",array[i].n_sect);
    // printf("n_desc %hu\n ",array[i].n_desc);
  }
  i = -1;
  while (++i < nsyms)
  {
    // printf("entre\n");
    // run = 0;
    str = ft_strdup(strtab + array[al_order[i]].n_un.n_strx);
    // printf("assigne str\n");
    // while (run < i)
    // {
    //   printf("%d\n", run);
    //   if (!(ft_strcmp(strtab + array[al_order[i]].n_un.n_strx, strtab + array[al_order[run]].n_un.n_strx)))
    //   run++;
    // }
    while (i < nsyms  && (!ft_strcmp("",strtab + array[al_order[i]].n_un.n_strx) || str[0] == '/' || (str[0] != '_'  && !(str[0] == 'd' && str[1] == 'y'))))
    {
      // printf("%d sur %d\n", i, nsyms);
      i++;
      if (i < nsyms)
        str = ft_strdup(strtab + array[al_order[i]].n_un.n_strx);
      else if (str)
        free(str);
      // printf("apres\n");
    }
    if (i >= nsyms)
      break;
    if (!check_tab_doubl(strtab + array[al_order[i]].n_un.n_strx, strtab, array, al_order, i))
    {
      if ((array[al_order[i]].n_value))
        printf("0000000%llx", array[al_order[i]].n_value);
      else
        printf("                ");

      if (type[al_order[i]])
        printf(" %c ", type[al_order[i]]);
      else
        printf(" %d ", array[al_order[i]].n_type);
      printf("%s\n",strtab + array[al_order[i]].n_un.n_strx);
    }
    // printf("sort\n");
    if (str)
      free(str);
  }
    // printf("[%s]\n", strtab + array[al_order[i]].n_un.n_strx);
    // printf("\n");
}


void handle_64(void *ptr)
{
  struct mach_header_64 *header;
  struct load_command       *lc;
  struct symtab_command   *sym;
  int   i;
  int ncmds;
  void *limit;

  i = 0;
  header = (struct mach_header_64 *)ptr;
  ncmds = header->ncmds;
  lc = ptr + sizeof(*header);

  // printf("\n====struct mach_header_64=====\n ncmds %d\n cpusubtype %d\n cputype %d\n filetype %d\n flags %d\n magic %d\n reserved %u\n sizeofcmds %d\n================================================\n", ncmds, header->cpusubtype, header->cputype, header->filetype, header->flags, header->magic, header->reserved, header->sizeofcmds);

  while (i++ < ncmds)
  {
      // printf("\n=====struct load_command=======\n cmd %d\n cmdsize %d\n================================================\n", lc->cmd, lc->cmdsize);
    if (lc->cmd == LC_SYMTAB)
    {
      sym = (struct symtab_command *)lc;
      // printf("\n====struct symtab_command=====\n cmdsize=%d\n symoff=%d\n nsyms=%d\n stroff=%d\n strsize=%d\n================================================\n\n",sym->cmdsize,  sym->symoff, sym->nsyms, sym->stroff,  sym->strsize);
      print_output(sym->nsyms, sym->symoff, sym->stroff, ptr);
      break ;
    }
    lc = (void *)lc + lc->cmdsize;
  }
}

void  nm(void *ptr)
{
  int   magic_number;

  magic_number = *(int *)ptr;
  if (magic_number == MH_MAGIC_64 || magic_number == MH_CIGAM_64)
    handle_64(ptr);
  else
    printf("Not a 64 bit binary\n");
}

int main(int ac, char **av)
{
  int fd;
  int i;
  char  *ptr;
  struct stat buff;

// printf("[]%s[]\n", SECT_TEXT);
  i = 0;
  if (ac < 2)
    return (printf("Nombre d'arguments insuffisant\n"));
  while (++i < ac)
  {
    if (ac > 2)
      printf("\n%s:\n", av[i]);
    if ((fd = open(av[i], O_RDONLY)) < 0)
      return (printf("Open error\n"));
    if (fstat(fd, &buff) < 0)
      return (printf("fstat error\n"));
    // printf("\n====struct stat=====\n st_dev  %d\n", buff.st_dev);
    // printf(" st_ino  %llu\n", buff.st_ino);
    // printf(" st_mode  %d\n", buff.st_mode);
    // printf(" st_nlink %d\n", buff.st_nlink);
    // printf(" st_uid  %d\n", buff.st_uid);
    // printf(" st_gid  %d\n", buff.st_gid);
    // printf(" st_rdev %d\n", buff.st_rdev);
    // printf(" st_size %lld\n================================================\n", buff.st_size);

    if ((ptr = mmap(0, buff.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
      return (printf("mmap error\n"));
    nm(ptr);
    if (munmap(ptr, buff.st_size) < 0)
      return (printf("munmap error\n"));
  }
  return (0);
}
