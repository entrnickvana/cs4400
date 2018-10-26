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
  Author:     Nick Elliott
  UID:        U0682219
*/

/*
  Given the in-memory ELF header pointer as `ehdr` and a section
  header pointer as `shdr`, returns a pointer to the memory that
  contains the in-memory content of the section 
*/

#define AT_SEC(ehdr, shdr) ((void *)(ehdr) + (shdr)->sh_offset)
#define ELF64_R_SYM_1(r_info) ((r_info) >> 32)
#define ELF64_ST_TYPE1(st_info) ((st_info) & 0xf)

static int get_secrecy_level(const char *s); /* foward declaration */

int p_shdrs(Elf64_Ehdr*  ehdr, char* name, int on_off);
void p_syms(Elf64_Ehdr* ehdr);
char* p_sym(Elf64_Ehdr* ehdr, int indx, int on_off);
Elf64_Shdr *section_by_index(Elf64_Ehdr *ehdr, int idx);
Elf64_Shdr *section_by_name(Elf64_Ehdr *ehdr, char *name);
unsigned char* get_code_addr(Elf64_Ehdr *ehdr, int sym_indx);
Elf64_Sym* get_sym_arr(Elf64_Ehdr *ehdr);
int get_sym_arr_count(Elf64_Ehdr *ehdr);
void print_decode(int sym_indx);
Elf64_Sym* sym_by_index(Elf64_Ehdr* ehdr, int index, char* nameOfSymSection);
int print_ins_info(Elf64_Ehdr* ehdr, instruction_t* ins, unsigned char* code_ptr);
void print_rela(Elf64_Ehdr *ehdr);
int rela_idx(Elf64_Ehdr *ehdr, Elf64_Addr code_addr, int plt);
unsigned char* p_sect_addr(Elf64_Ehdr *ehdr);
char* print_rela_str(Elf64_Ehdr *ehdr, int k);

void mov_addr_to_reg_func(Elf64_Ehdr *ehdr, instruction_t* ins, unsigned char* code_ptr, Elf64_Addr code_addr, int clr_sec_lvl);
void jmp_to_addr_func(Elf64_Ehdr *ehdr, instruction_t* ins, unsigned char* code_ptr, Elf64_Addr code_addr, int clr_sec_lvl);
void maybe_jmp_func(Elf64_Ehdr *ehdr, instruction_t* ins, unsigned char* code_ptr, Elf64_Addr code_addr, int clr_sec_lvl);
void other_op_func(Elf64_Ehdr *ehdr, instruction_t* ins, unsigned char* code_ptr, Elf64_Addr code_addr, int clr_sec_lvl);

void fix_all(Elf64_Ehdr *ehdr);
void fix(Elf64_Ehdr *ehdr, instruction_t* ins, unsigned char* code_ptr, Elf64_Addr code_addr, int clr_sec_lvl);
int p_lvl = 1;

void redact(Elf64_Ehdr *ehdr) 
{
  p_shdrs(ehdr, NULL, 1);
  p_sect_addr(ehdr);
  p_syms(ehdr);
  fix_all(ehdr);
}

char* print_rela_str(Elf64_Ehdr *ehdr, int k)
{
  return p_sym(ehdr, k, 1);
}

/* gets the array of symbols from .dynsym section */
Elf64_Sym* get_sym_arr(Elf64_Ehdr *ehdr)
{
    Elf64_Shdr* dynsym_shdr = section_by_name(ehdr, ".dynsym");
    Elf64_Sym* syms = AT_SEC(ehdr, dynsym_shdr);
    return syms;
}

/* Print rela information */
void print_rela(Elf64_Ehdr *ehdr)
{
  printf("\n\n\n+++++++++++++++++++++++   RELAS   ++++++++++++++++++++++++++++\n\n");
  Elf64_Shdr *rela_dyn_shdr = section_by_name(ehdr, ".rela.dyn");
  Elf64_Rela *relas = AT_SEC(ehdr, rela_dyn_shdr);
  int i, count = rela_dyn_shdr->sh_size / sizeof(Elf64_Rela);
  for (i = 0; i < count; i++) {
    printf("%d\n", (int)ELF64_R_SYM_1(relas[i].r_info));
  }
  printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n\n");  
}

/*  Gets relavant index corresponding to a runtime address 'code_addr'.  int plt parameter specifies rela.plt from rela.dyn. */
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
    { printf("\nMATCHED ADDRESS FOR VAR, IDX K: %d \n", k); var_addr_idx = k;}
    else
    { 
      if(0) { printf("[ UNMATCHED ] K:%d \n\tcode_addr: %d \n\trela_addr: %d \n", k, (int)code_addr, (int)rela_addr);}
      if(0) { printf("\tDIFF: \t\t%d\n", (int)(code_addr - rela_addr));                                           }
    }
  }
  return var_addr_idx;
}

int get_sym_arr_count(Elf64_Ehdr *ehdr)
{
    Elf64_Shdr *dynsym_shdr = section_by_name(ehdr, ".dynsym");
    int count = dynsym_shdr->sh_size / sizeof(Elf64_Sym);
    return count;
}

/*  Calculates the memory addr of the machine code for the text seciont, this is accomplished using st_index from symbol fields*/
unsigned char* get_code_addr(Elf64_Ehdr *ehdr, int sym_indx)
{
  Elf64_Sym* syms = get_sym_arr(ehdr);
  Elf64_Shdr* shdrs = (void*)ehdr + ehdr->e_shoff;
  
  int j = syms[sym_indx].st_shndx;
  unsigned char* code = AT_SEC(ehdr, shdrs + j) + (syms[sym_indx].st_value - shdrs[j].sh_addr);
  return code;
}

/* printing addresses for debugging */
unsigned char* p_sect_addr(Elf64_Ehdr *ehdr)
{
  Elf64_Shdr *shdrs = (void*)ehdr+ehdr->e_shoff;
  int i;
  for (i = 0; i < ehdr->e_shnum; i++) 
    printf("IDX: %d\t\tRUNTIME ADDR:  %d\t\t MEM OFFSTE: %d\t\n", i, (int)shdrs[i].sh_addr, (int)shdrs[i].sh_offset);
  return NULL;
}

/*
  fix_all is the largest of a set of handlers for redact
  fix_all iterates over each symbol, selects only function symbols of size > 0 
  and then calls fix which is recursive.
*/
void fix_all(Elf64_Ehdr *ehdr)
{
  int m, clr_sec_lvl, count = get_sym_arr_count(ehdr);  
  Elf64_Sym* syms = get_sym_arr(ehdr);
  instruction_t ins;
  
  unsigned char* code_ptr; 
  Elf64_Addr code_addr;

  for(m = 0; m < count; ++m){                                                   /* Iterate through each symbol*/
      clr_sec_lvl = get_secrecy_level(p_sym(ehdr, m, 0));          
      printf("\n\n\n\n\n\n\n______________________________ ENTERING SYMBOL M: %d _______________________________\n", m);
      printf("FUNCTION NAME: %s\t FUNC SEC LEVEL:\t%d\n", p_sym(ehdr, m, 0),clr_sec_lvl);
  
      code_ptr = get_code_addr(ehdr, m);   // in memory, red
      code_addr = syms[m].st_value;        // at runtime, green 
  
      if(ELF64_ST_TYPE1(syms[m].st_info) == STT_FUNC && syms[m].st_size > 0)    /* Check each symbol for size and type*/
        fix(ehdr, &ins, code_ptr, code_addr, clr_sec_lvl);
  }        
}

/*
  fix directs each new recursive call traversing the machine code of a function symbol.  The two recursive base
  cases are a jump_to_addr_func and return.  
*/
void fix(Elf64_Ehdr *ehdr, instruction_t* ins, unsigned char* code_ptr, Elf64_Addr code_addr, int clr_sec_lvl)
{
      decode(ins, code_ptr, code_addr);
      print_ins_info(ehdr, ins, code_ptr);
  
      if     (ins->op == 0) { mov_addr_to_reg_func(ehdr, ins, code_ptr, code_addr, clr_sec_lvl);   }
      else if(ins->op == 1) { jmp_to_addr_func(ehdr, ins, code_ptr, code_addr, clr_sec_lvl);       }
      else if(ins->op == 2) { maybe_jmp_func(ehdr, ins, code_ptr, code_addr, clr_sec_lvl);         }
      else if(ins->op == 3) {  /* DO NOTHING, JUST RETURN*/                                        }
      else if(ins->op == 4) { other_op_func(ehdr, ins, code_ptr, code_addr, clr_sec_lvl);          }
}

void mov_addr_to_reg_func(Elf64_Ehdr *ehdr, instruction_t* ins, unsigned char* code_ptr, Elf64_Addr code_addr, int clr_sec_lvl)
{

  Elf64_Shdr *rela_dyn_shdr = section_by_name(ehdr, ".rela.dyn");
  Elf64_Rela *relas = AT_SEC(ehdr, rela_dyn_shdr);
  char* var_str = NULL;
  int var_lvl, k, m;
  
  k = rela_idx(ehdr, ins->mov_addr_to_reg.addr, 0); 
  m = ELF64_R_SYM(relas[k].r_info);
  if(k >= 0){
    printf("VAR MATCH !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"); 
    var_str = print_rela_str(ehdr, m);
    var_lvl = get_secrecy_level(p_sym(ehdr, m, 0));
    printf("CALLER LEVEL: %d\tCALLEE LEVEL: %d", clr_sec_lvl, var_lvl);
    if(clr_sec_lvl >= var_lvl) 
      printf("\tPERMITTED VARIABLE ACCESS\n");
    else{
      printf("\tILLEGAL VARIABLE ACCESS\n");
      replace_with_crash(code_ptr, ins);
    }
  }    
  
  code_ptr+= ins->length;
  code_addr+= ins->length;        
  fix(ehdr, ins, code_ptr, code_addr, clr_sec_lvl); 
}

void jmp_to_addr_func(Elf64_Ehdr *ehdr, instruction_t* ins, unsigned char* code_ptr, Elf64_Addr code_addr, int clr_sec_lvl)
{
  instruction_t new_ins;
  Elf64_Shdr *rela_plt_shdr = section_by_name(ehdr, ".rela.plt");
  Elf64_Rela *relas = AT_SEC(ehdr, rela_plt_shdr);
  Elf64_Shdr *shdrs = (void*)ehdr+ehdr->e_shoff;

  int j = p_shdrs(ehdr, ".plt", 0);

  unsigned char* new_code = AT_SEC(ehdr, shdrs + j) + (ins->jmp_to_addr.addr - shdrs[j].sh_addr);

  printf("SECOND DECODE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

  decode(&new_ins, new_code, ins->jmp_to_addr.addr);
  print_ins_info(ehdr, &new_ins, new_code);
  char* callee_func_str = NULL;
  int callee_func_lvl;

  int k = -1;
  k = rela_idx(ehdr, new_ins.jmp_to_addr.addr, 1); 
  int m = -1;

  m = ELF64_R_SYM(relas[k].r_info);
  printf("M = %d\n", m);

  if(k > -1){
    callee_func_str = print_rela_str(ehdr, m);
    callee_func_lvl = get_secrecy_level(p_sym(ehdr, m, 0));
    printf("CALLER LEVEL: %d\tCALLEE LEVEL: %d", clr_sec_lvl, callee_func_lvl);
    if(clr_sec_lvl >= callee_func_lvl) 
      printf("\tPERMITTED FUNCTION ACCESS\n");
    else{
      printf("\tILLEGAL FUNCTION ACCESS\n");
      replace_with_crash(code_ptr, ins);
    }
  }
  else
    printf("@@@ NO K MATCH FOUND @@@\n");
}

void maybe_jmp_func(Elf64_Ehdr *ehdr, instruction_t* ins, unsigned char* code_ptr, Elf64_Addr code_addr, int clr_sec_lvl)
{
  instruction_t branch_ins;
  Elf64_Shdr *shdrs = (void*)ehdr+ehdr->e_shoff;
  int j = p_shdrs(ehdr, ".text", 0);
  unsigned char* branch_code = AT_SEC(ehdr, shdrs + j) + (ins->maybe_jmp_to_addr.addr - shdrs[j].sh_addr);
  instruction_t temp_ins = *ins;

  code_ptr+= ins->length;
  code_addr+= ins->length;    

  printf("ENTERING NOT BRANCH   ////////////////////////////////\n");      
  fix(ehdr, ins, code_ptr, code_addr, clr_sec_lvl);
   
  printf("ENTERING BRANCH       ////////////////////////////////\n");
  fix(ehdr, &branch_ins, branch_code, temp_ins.maybe_jmp_to_addr.addr, clr_sec_lvl);
  printf("EXITING BRANCH       ////////////////////////////////\n");
}

void other_op_func(Elf64_Ehdr *ehdr, instruction_t* ins, unsigned char* code_ptr, Elf64_Addr code_addr, int clr_sec_lvl)
{
  code_ptr+= ins->length;
  code_addr+= ins->length;        
  fix(ehdr, ins, code_ptr, code_addr, clr_sec_lvl);
}

int print_ins_info(Elf64_Ehdr* ehdr, instruction_t* ins, unsigned char* code_ptr)
{
  printf("\nINTERPRETED INSTRUCTION -------------------------------------------------------\n");
  int l = ins->length;
  switch(ins->op)
  {
    case 0: printf("CODE PTR: %p\tOP:\tMOV_ADDR_TO_REG_OP\t\tLENGTH: %d\t\tADDR: %llu\n",    code_ptr, l,  (long long unsigned int)ins->mov_addr_to_reg.addr );  break;
    case 1: printf("CODE PTR: %p\tOP:\tJMP_TO_ADDR_OP\t\t\tLENGTH: %d\t\tADDR: %llu\n",      code_ptr, l,  (long long unsigned int)ins->jmp_to_addr.addr);       break;
    case 2: printf("CODE PTR: %p\tOP:\tMAYBE_JMP_TO_ADDR_OP\t\tLENGTH: %d\t\tADDR: %llu\n",  code_ptr, l,  (long long unsigned int)ins->maybe_jmp_to_addr.addr );break;
    case 3: printf("CODE PTR: %p\tOP:\tRET_OP\t\t\t\tLENGTH: %d\t\tADDR: NA\n",              code_ptr, l );break;
    case 4: printf("CODE PTR: %p\tOP:\tOTHER_OP\t\t\tLENGTH: %d\t\tADDR: NA\n",              code_ptr, l );break;
  }

  int i=0;
  unsigned char* ptr = code_ptr;
  for (; i<ins->length; i++)
    printf("%.2x ", ptr[i]);
  printf("\n");

  return 0;
}

/*  Print all symbols in .dynsym  */
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
        printf("FUNC:\t\t\t %d: \t%s \t\t\t SIZE: %d\n", i, strs + syms[i].st_name, (int)syms[i].st_size);
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

/* Print symbol using index in .dynsym */
char* p_sym(Elf64_Ehdr* ehdr, int indx, int on_off)
{

    Elf64_Shdr *dynsym_shdr = section_by_name(ehdr, ".dynsym");
    Elf64_Sym *syms = AT_SEC(ehdr, dynsym_shdr);
    const char *strs = AT_SEC(ehdr, section_by_name(ehdr, ".dynstr"));

    if(on_off)  { printf("-->  %s\n", strs + syms[indx].st_name);   }
    return strs + syms[indx].st_name;
}

/* Print all section headers */
int p_shdrs(Elf64_Ehdr*  ehdr, char* name, int on_off)
{
  int indx = -1;

  if(on_off) DEBUG_PRINT(("------------------------------SECTION HDR NAMES------------------------\n"));
  Elf64_Shdr *shdrs = (void*)ehdr+ehdr->e_shoff;
  char *strs = (void*)ehdr+shdrs[ehdr->e_shstrndx].sh_offset;
  int i;
  for (i = 0; i < ehdr->e_shnum; i++){                                            /* check each section header name for match */
    if(on_off)
      printf("index: %d\t\t%s\n",i,  strs + shdrs[i].sh_name);
    if(name != NULL)
      if( strcmp((const char*)name, (const char*)(strs + shdrs[i].sh_name)) == 0) /* compare input str to match with section name */
        indx = i;
  } 

  if(on_off) DEBUG_PRINT(("------------------------------SECTION HDR NAMES------------------------\n"));

  return indx;
}

/* get reference to section name by index */
Elf64_Shdr *section_by_index(Elf64_Ehdr *ehdr, int idx)
{
  Elf64_Shdr *shdrs = (void*)ehdr+ehdr->e_shoff;
  return &shdrs[idx];
}

/* get section reference given the corresponding str name */
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

