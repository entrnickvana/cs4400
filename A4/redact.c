#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <elf.h>
#include "decode.h"

#define DEBUG 0
#ifdef DEBUG
# define DEBUG_PRINT(x) printf x
#else
# define DEBUG_PRINT(x) do {} while (0)
#endif

/*
  Given the in-memory ELF header pointer as `ehdr` and a section
  header pointer as `shdr`, returns a pointer to the memory that
  contains the in-memory content of the section 
*/

#define AT_SEC(ehdr, shdr) ((void *)(ehdr) + (shdr)->sh_offset)
#define ELF64_R_SYM_1(r_info) ((r_info) >> 32)
#define ELF64_ST_TYPE1(st_info) ((st_info) & 0xf)


static int get_secrecy_level(const char *s); /* foward declaration */

// ADDED FUNCTIONS
int p_shdrs(Elf64_Ehdr*  ehdr, char* name, int on_off);
void p_syms(Elf64_Ehdr* ehdr);
char* p_sym(Elf64_Ehdr* ehdr, int indx, int on_off);
Elf64_Shdr *section_by_index(Elf64_Ehdr *ehdr, int idx);
Elf64_Shdr *section_by_name(Elf64_Ehdr *ehdr, char *name);
unsigned char* get_code_addr(Elf64_Ehdr *ehdr, int sym_indx);
Elf64_Sym* get_sym_arr(Elf64_Ehdr *ehdr);
int get_sym_arr_count(Elf64_Ehdr *ehdr);
void print_decode(int sym_indx);
void fix(Elf64_Ehdr *ehdr);
Elf64_Sym* sym_by_index(Elf64_Ehdr* ehdr, int index, char* nameOfSymSection);
int print_ins_info(Elf64_Ehdr* ehdr, instruction_t* ins);
void print_mach_code(unsigned char* code_ptr, instruction_t ins);
void print_rela(Elf64_Ehdr *ehdr);
int rela_idx(Elf64_Ehdr *ehdr, Elf64_Addr code_addr, int plt);

// ADDED FUNCTIONS

/*************************************************************/

void redact(Elf64_Ehdr *ehdr) 
{
  p_shdrs(ehdr, NULL, 1);
  fix(ehdr);

}

void print_rela_str(Elf64_Ehdr *ehdr, int k)
{
  int m = -1;
  Elf64_Shdr *rela_dyn_shdr = section_by_name(ehdr, ".rela.dyn");
  Elf64_Rela *relas = AT_SEC(ehdr, rela_dyn_shdr);

  m = ELF64_R_SYM(relas[k].r_info);
  p_sym(ehdr, k, 1);
}

Elf64_Sym* get_sym_arr(Elf64_Ehdr *ehdr)
{
    Elf64_Shdr* dynsym_shdr = section_by_name(ehdr, ".dynsym");
    Elf64_Sym* syms = AT_SEC(ehdr, dynsym_shdr);
    return syms;
}

void print_rela(Elf64_Ehdr *ehdr)
{

  printf("\n\n\n+++++++++++++++++++++++   RELAS   ++++++++++++++++++++++++++++\n\n");
  Elf64_Shdr *rela_dyn_shdr = section_by_name(ehdr, ".rela.dyn");
  Elf64_Rela *relas = AT_SEC(ehdr, rela_dyn_shdr);
  int i, count = rela_dyn_shdr->sh_size / sizeof(Elf64_Rela);
  for (i = 0; i < count; i++) {
    printf("%d\n", ELF64_R_SYM_1(relas[i].r_info));
  }
  printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n\n");  
}

int rela_idx(Elf64_Ehdr *ehdr, Elf64_Addr code_addr, int plt)
{

  Elf64_Shdr *rela_dyn_shdr;
  if(plt == 0)  rela_dyn_shdr = section_by_name(ehdr, ".rela.dyn");
  else          rela_dyn_shdr = section_by_name(ehdr, ".rela.plt");

  Elf64_Rela *relas = AT_SEC(ehdr, rela_dyn_shdr);
  int k, count = rela_dyn_shdr->sh_size / sizeof(Elf64_Rela);
  Elf64_Addr rela_addr = 0;
  int var_addr_idx = -1;
  for (k = 0; k < count; k++) {
    rela_addr = relas[k].r_offset;
    if(code_addr == rela_addr)
    { printf("\n\n\n\n---------------------------------\nMATCHED ADDRESS FOR VAR, IDX K: %d \n\n\n", k); var_addr_idx = k;}
    else
    { 
      if(0) { printf("[ UNMATCHED ] K:%d \n\tcode_addr: %d \n\trela_addr: %d \n", k, code_addr, rela_addr);}
      if(0) { printf("\tDIFF: \t\t%d\n", code_addr - rela_addr);                                           }
    }
  }
  return var_addr_idx;
}

int get_sym_arr_count(Elf64_Ehdr *ehdr)
{
    Elf64_Shdr *dynsym_shdr = section_by_name(ehdr, ".dynsym");
    char *strs = AT_SEC(ehdr, section_by_name(ehdr, ".dynstr"));
    int count = dynsym_shdr->sh_size / sizeof(Elf64_Sym);
    return count;
}


unsigned char* get_code_addr(Elf64_Ehdr *ehdr, int sym_indx)
{
  Elf64_Sym* syms = get_sym_arr(ehdr);
  Elf64_Shdr* shdrs = (void*)ehdr + ehdr->e_shoff;
  
  int j = syms[sym_indx].st_shndx;
  unsigned char* code = AT_SEC(ehdr, shdrs + j) + (syms[sym_indx].st_value - shdrs[j].sh_addr);
  return code;
}

void fix(Elf64_Ehdr *ehdr)
{
    p_syms(ehdr);
    print_rela(ehdr);
    Elf64_Sym* syms = get_sym_arr(ehdr);
    Elf64_Sym curr_sym;
    unsigned char* jmp_code = NULL;
    Elf64_Shdr *plt_shdr = section_by_name(ehdr, ".plt");
    int m, count = get_sym_arr_count(ehdr);
    instruction_t ins;
    instruction_t prev_ins;
    instruction_t prev_func;
    unsigned char* code_ptr;       // in memory, red
    Elf64_Addr code_addr;
    Elf64_Addr code_addr2 = NULL;
    int sec_lvl;
    int k = 0;
    int level = 0;
    int plt_indx = p_shdrs(ehdr, ".plt", 0);
    int text_indx = p_shdrs(ehdr, ".text", 0);
    Elf64_Shdr *shdrs = (void*)ehdr+ehdr->e_shoff;

    for(m = 0; m < count; ++m){
        if(ELF64_ST_TYPE1(syms[m].st_info) == STT_FUNC && syms[m].st_size > 0){
          printf("\n_____________________________ ENTERING FUNCTION:  %s _______________________________\n", p_sym(ehdr, m, 0));

          sec_lvl = get_secrecy_level(p_sym(ehdr, m, 0));      
          printf("m: %d\t sec: %d\n\n", m, sec_lvl);      
  
          code_ptr = get_code_addr(ehdr, m);       // in memory, red
          code_addr = syms[m].st_value;            // at runtime, green 
  
          do{
              prev_ins = ins;
              decode(&ins, code_ptr, code_addr); 
              print_ins_info(ehdr, &ins);
  
              if(ins.op == 0)       
              { 
                printf("OP: 0\n");
                code_addr = ins.mov_addr_to_reg.addr;
                k = rela_idx(ehdr, code_addr, 0); 

                if(k >= 0){
                  printf("((((((((((((((  MATCH  ))))))))))))))))))))\n\n");
                  print_rela_str(ehdr, k);
                  printf("((((((((((((((  MATCH  ))))))))))))))))))))\n\n\n\n");                
                }
                else
                  printf(" ))))))))))) NO MATCH ((((((((((\n");

              }
              else if(ins.op == 1){   //  JMP_TO_ADDR_OP
                printf("OP: 1\n");                

                prev_func = prev_ins;
                code_addr = ins.jmp_to_addr.addr;
                code_ptr = AT_SEC(ehdr, shdrs + plt_indx) + (code_addr - section_by_name(ehdr, ".rela.plt")->sh_addr);
                decode(&ins, code_ptr, code_addr);
                print_ins_info(ehdr, &ins);                

                k = rela_idx(ehdr, code_addr, 1); 

                if(k > -1)
                  printf("<<<<<<<<<<<<<<<<<<<<<<<<<<< K: %d\n", k);
                else        
                  printf("@@@ NO K MATCH FOUND @@@\n");

              }
              else if(ins.op == 2){   //  MAYBE_JMP_TO_ADDR_OP
                printf("OP: 2\n");

                code_addr = ins.maybe_jmp_to_addr.addr;
                code_ptr = AT_SEC(ehdr, shdrs + text_indx) + (code_addr - section_by_name(ehdr, ".text")->sh_addr);
                decode(&ins, code_ptr, code_addr);
                print_ins_info(ehdr, &ins);                

              }
              else if(ins.op == 3){   //  RET_OP
                printf("OP: 3\n");

              }
              else if(ins.op == 4){   //  OTHER_OP
                printf("OP: 4\n");                

              }

              code_ptr = code_ptr + ins.length;
              code_addr+= ins.length;

          }while(ins.op != 3);
        
      } else {}

    }// END FOR M

}

void print_mach_code(unsigned char* code_ptr, instruction_t ins)
{
  int i;
  for(i = 0; i < ins.length; ++i)
    printf("%02hhx ", *(code_ptr + i));
  printf("\n");

}

int print_ins_info(Elf64_Ehdr* ehdr, instruction_t* ins)
{
  printf("\nTHE DECODED INSTRUCTION WAS INTERPRETED AS:\n--------------------------\n");
  switch(ins->op)
  {
    case 0: printf("OP:\tMOV_ADDR_TO_REG_OP\t\tLENGTH: %d\t\tADDR: %llu\n\n", ins->length, (long long unsigned int)ins->mov_addr_to_reg.addr );break;
    case 1: printf("OP:\tJMP_TO_ADDR_OP\t\tLENGTH: %d\t\tADDR: %llu\n\n", ins->length, (long long unsigned int)ins->jmp_to_addr.addr);break;
    case 2: printf("OP:\tMAYBE_JMP_TO_ADDR_OP\t\tLENGTH: %d\t\tADDR: %llu\n\n", ins->length,(long long unsigned int) ins->maybe_jmp_to_addr.addr );break;
    case 3: printf("OP:\tRET_OP\t\tLENGTH: %d\t\tADDR: NA\n\n", ins->length );break;
    case 4: printf("OP:\tOTHER_OP\t\tLENGTH: %d\t\tADDR: NA\n\n", ins->length );break;
  }

  return 0;
}

void p_syms(Elf64_Ehdr* ehdr)
{

  DEBUG_PRINT(("------------------------------DYN SYMBOL NAMES------------------------\n"));
    Elf64_Shdr *dynsym_shdr = section_by_name(ehdr, ".dynsym");
    Elf64_Sym *syms = AT_SEC(ehdr, dynsym_shdr);
    char *strs = AT_SEC(ehdr, section_by_name(ehdr, ".dynstr"));
    int i, count = dynsym_shdr->sh_size / sizeof(Elf64_Sym);
    for (i = 0; i < count; i++) 
      if( ELF64_ST_TYPE1(syms[i].st_info) == STT_FUNC)
        printf("FUNC:\t\t\t %d: \t%s\n", i, strs + syms[i].st_name);
      else
        printf("NON FUNC X\t\t %d: \t%s\n", i, strs + syms[i].st_name);        

  DEBUG_PRINT(("------------------------------DYN SYMBOL NAMES------------------------\n\n\n"));  
  DEBUG_PRINT(("------------------------------FUNC SYMBOL ONLY W/ SIZE > 0 ------------------------\n\n\n"));  
    for (i = 0; i < count; i++) 
      if( ELF64_ST_TYPE1(syms[i].st_info) == STT_FUNC && syms[i].st_size > 0)
        printf("FUNC:\t\t\t %d: \t%s \t\t\t SIZE: %d\n", i, strs + syms[i].st_name, syms[i].st_size);
  DEBUG_PRINT(("------------------------------FUNC SYMBOL ONLY W/ SIZE > 0 ------------------------\n\n\n"));  


  DEBUG_PRINT(("------------------------------SYMBOL NAMES------------------------\n"));
  
    Elf64_Shdr *sym_shdr = section_by_name(ehdr, ".symtab");
    syms = AT_SEC(ehdr, sym_shdr);
    strs = AT_SEC(ehdr, section_by_name(ehdr, ".strtab"));

    count = sym_shdr->sh_size / sizeof(Elf64_Sym);
    for (i = 0; i < count; i++) 
     printf("%d: %s\n", i, strs + syms[i].st_name);
   DEBUG_PRINT(("-------------------------------SYMBOL NAMES------------------------\n\n\n"));

}

char* p_sym(Elf64_Ehdr* ehdr, int indx, int on_off)
{

    Elf64_Shdr *dynsym_shdr = section_by_name(ehdr, ".dynsym");
    Elf64_Sym *syms = AT_SEC(ehdr, dynsym_shdr);
    const char *strs = AT_SEC(ehdr, section_by_name(ehdr, ".dynstr"));

    if(on_off)  { printf("-->  %s\n", strs + syms[indx].st_name);   }
    return (const char*)(strs + syms[indx].st_name);
}


int p_shdrs(Elf64_Ehdr*  ehdr, char* name, int on_off)
{
  int indx = -1;

  DEBUG_PRINT(("------------------------------SECTION HDR NAMES------------------------\n"));
  Elf64_Shdr *shdrs = (void*)ehdr+ehdr->e_shoff;
  char *strs = (void*)ehdr+shdrs[ehdr->e_shstrndx].sh_offset;
  int i;
  for (i = 0; i < ehdr->e_shnum; i++){
    if(on_off)
      printf("index: %d\t\t%s\n",i,  strs + shdrs[i].sh_name);

    if(name != NULL)
      if( strcmp((const char*)name, (const char*)(strs + shdrs[i].sh_name)) == 0)
        indx = i;
  } 

  DEBUG_PRINT(("------------------------------SECTION HDR NAMES------------------------\n"));

  return indx;
}


Elf64_Shdr *section_by_index(Elf64_Ehdr *ehdr, int idx)
{
  Elf64_Shdr *shdrs = (void*)ehdr+ehdr->e_shoff;
  return &shdrs[idx];
}

Elf64_Shdr *section_by_name(Elf64_Ehdr *ehdr, char *nm)
{
  Elf64_Shdr *shdrs = (void*)ehdr+ehdr->e_shoff;
  char *strs = (void*)ehdr+shdrs[ehdr->e_shstrndx].sh_offset;
  int i;
  for(i = 0; i < ehdr->e_shnum; i++)
    if(strcmp(strs + shdrs[i].sh_name, nm) == 0)
      return &shdrs[i];
  return NULL;
}

/*************************************************************/

static int get_secrecy_level(const char *s) {
  int level = 0;
  int len = strlen(s);
  while (len && (s[len-1] >= '0') && (s[len-1] <= '9')) {
    level = (level * 10) + (s[len-1] - '0');
    --len;
  }
  return level;
}

static void fail(char *reason, int err_code) {
  fprintf(stderr, "%s (%d)\n", reason, err_code);
  exit(1);
}

static void check_for_shared_object(Elf64_Ehdr *ehdr) {
  if ((ehdr->e_ident[EI_MAG0] != ELFMAG0)
      || (ehdr->e_ident[EI_MAG1] != ELFMAG1)
      || (ehdr->e_ident[EI_MAG2] != ELFMAG2)
      || (ehdr->e_ident[EI_MAG3] != ELFMAG3))
    fail("not an ELF file", 0);

  if (ehdr->e_ident[EI_CLASS] != ELFCLASS64)
    fail("not a 64-bit ELF file", 0);
  
  if (ehdr->e_type != ET_DYN)
    fail("not a shared-object file", 0);
}

int main(int argc, char **argv) {
  int fd_in, fd_out;
  size_t len;
  void *p_in, *p_out;
  Elf64_Ehdr *ehdr;

  if (argc != 3)
    fail("expected two file names on the command line", 0);

  /* Open the shared-library input file */
  fd_in = open(argv[1], O_RDONLY);
  if (fd_in == -1)
    fail("could not open input file", errno);

  /* Find out how big the input file is: */
  len = lseek(fd_in, 0, SEEK_END);

  /* Map the input file into memory: */
  p_in = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd_in, 0);
  if (p_in == (void*)-1)
    fail("mmap input failed", errno);

  /* Since the ELF file starts with an ELF header, the in-memory image
     can be cast to a `Elf64_Ehdr *` to inspect it: */
  ehdr = (Elf64_Ehdr *)p_in;

  /* Check that we have the right kind of file: */
  check_for_shared_object(ehdr);

  /* Open the shared-library output file and set its size: */
  fd_out = open(argv[2], O_RDWR | O_CREAT, 0777);
  if (fd_out == -1)
    fail("could not open output file", errno);
  if (ftruncate(fd_out, len))
    fail("could not set output file file", errno);

  /* Map the output file into memory: */
  p_out = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd_out, 0);
  if (p_out == (void*)-1)
    fail("mmap output failed", errno);

  /* Copy input file to output file: */
  memcpy(p_out, p_in, len);

  /* Close input */
  if (munmap(p_in, len))
    fail("munmap input failed", errno);    
  if (close(fd_in))
    fail("close input failed", errno);

  ehdr = (Elf64_Ehdr *)p_out;

  redact(ehdr);

  if (munmap(p_out, len))
    fail("munmap input failed", errno);    
  if (close(fd_out))
    fail("close input failed", errno);

  return 0;
}

