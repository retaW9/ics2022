#include <common.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <elf.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>

extern uint64_t g_nr_guest_inst;

char *StringTable;
char *strtab;
int fd;
uint8_t sym_shdr = 0;
uint8_t str_shdr = 0;
uint8_t *elf_mem;
struct stat st;
/* the elf header */
Elf64_Ehdr *ehdr;
/* Section header */
Elf64_Shdr *shdr;
/* Symbol table */
Elf64_Sym *sym;

void init_ftrace(const char *elf)
{
#if 0
  char *StringTable;

  int fd, i, j;
  uint8_t sym_shdr = 0;
  uint8_t *elf_mem;
  struct stat st;
  /* the elf header */
  Elf64_Ehdr *ehdr;
  /* Section header */
  Elf64_Shdr *shdr;
  /* Symbol table */
  Elf64_Sym *sym;
#endif

  int i, j; 

  if ((fd = open(elf, O_RDONLY)) < 0)
  {
    perror("open failed \n");
    exit(-1);
  }

  if (fstat(fd, &st) < 0)
  {
    perror("fstat failed \n");
    exit(-1);
  }

  /* map the file to the adderss of men */
  elf_mem = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (elf_mem == MAP_FAILED)
  {
    perror("mmap failed! \n");
    exit(-1);
  }

  /* get the ELF header address */
  ehdr = (Elf64_Ehdr *)elf_mem;

  /* get the elf setion header table address*/
  // shdr = (Elf64_Shdr *)(&elf_mem[ehdr->e_shoff]);
  shdr = (Elf64_Shdr *)(elf_mem + ehdr->e_shoff);

  printf("ELF header:\n\n");
  printf("%-36s 0x%lx\n", " Program Entry point:", ehdr->e_entry);
  // printf("%-36s %d (bytes)\n", " Size of program header:", ehdr->e_phentsize);
  // printf("%-36s %d\n", " Number of Program headers:", ehdr->e_phnum);
  printf("%-36s %d (bytes)\n", " Size of section header:", ehdr->e_shentsize);
  printf("%-36s %d\n", " Number of Section headers:", ehdr->e_shnum);
  printf("%-36s %d\n\n", " Section header string table index:", ehdr->e_shstrndx);

  /* get the section header string table address*/
  StringTable = (char *)&elf_mem[shdr[ehdr->e_shstrndx].sh_offset];

  printf("Section header list: \n\n");
  for (i = 1; i < ehdr->e_shnum; i++)
  {
    printf("%-26s 0x%lx\n", &StringTable[shdr[i].sh_name], shdr[i].sh_addr);
    if (strcmp(&StringTable[shdr[i].sh_name], ".symtab") == 0)
    {
      sym_shdr = i;
      // printf("location = %d\n", sym_shdr);
    }
    if (strcmp(&StringTable[shdr[i].sh_name], ".strtab") == 0)
    {
      str_shdr = i;
      // printf("location = %d\n", sym_shdr);
    }    
  }

  /* get the symbol table address */
  sym = (Elf64_Sym *)(&elf_mem[shdr[sym_shdr].sh_offset]);
  strtab = (char *)&elf_mem[shdr[str_shdr].sh_offset];
  printf("Synbol tabel list: \n\n");
  printf("Name\t\tSize\t\tValue\t \n\n");
  // printf("num =%ld", (shdr[sym_shdr].sh_size) / (shdr[sym_shdr].sh_entsize));

  for (j = 1; j < (shdr[sym_shdr].sh_size) / (shdr[sym_shdr].sh_entsize); j++)
  {
    /* Type is FUNC */
    if ((sym[j].st_info & 0x0f) == STT_FUNC)
    {
      printf("%s\t\t %ld\t\t0x%lx\t\n", strtab + sym[j].st_name , sym[j].st_size, sym[j].st_value);
    }
  }
}
