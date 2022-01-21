#include <stdio.h>
#include <map>
#include <vector>
#include <string>
#include <capstone/capstone.h>
#include "loader/loader.h"

// check if is actually a jmp or call instruction
bool is_cs_jmp_ins(cs_insn *ins);

// disassemble one instruction and save it
int save_jmps(Section *text, uint64_t root, std::map<std::string,
	      std::vector<uint64_t> > *jmps, csh dis);

// return the cs_mode, 32bit or 64bit
cs_mode automode(unsigned bits);

// find indirect jmp or call instructions in the .text section through the opcode
int find_jmps(Binary *bin);

  
int
main(int argc, char *argv[])
{
  Binary bin;
  std::string fname;

  if(argc < 2) {
    printf("Usage: %s <binary>\n", argv[0]);
    return 1;
  }

  fname.assign(argv[1]);
  if(load_binary(fname, &bin, Binary::BIN_TYPE_AUTO) < 0) {
    return 1;
  }

  if(find_jmps(&bin) < 0) {
    return 1;
  }

  unload_binary(&bin);

  return 0;
}


bool
is_cs_jmp_ins(cs_insn *ins)
{
  switch(ins->id) {
  case X86_INS_JMP:
  case X86_INS_CALL:
  case X86_INS_LJMP:
    return true;
  default:
    return false;
  }
}

int
save_jmps(Section *text, uint64_t root, 
                     std::map<std::string, std::vector<uint64_t> > *jmps, 
                     csh dis)
{
  size_t n;
  const uint8_t *pc;
  uint64_t offset;
  std::string jmp_str;
  cs_insn *cs_ins; 

  cs_ins = cs_malloc(dis);
  if(!cs_ins) {
    fprintf(stderr, "Out of memory\n");
    return -1;
  }

  offset = root - text->vma;
  pc = text->bytes + offset;
  n = text->size - offset;
  cs_disasm_iter(dis, &pc, &n, &root, cs_ins);

  if(is_cs_jmp_ins(cs_ins)) {
    jmp_str = "";
    jmp_str += std::string(cs_ins->mnemonic)
      + " " + std::string(cs_ins->op_str);
    (*jmps)[jmp_str].push_back(root-2);
  }
  
  cs_free(cs_ins, 1);

  return 0;
}

cs_mode automode(unsigned bits){
  cs_mode ret;

  if(bits==32) {
    ret = CS_MODE_32;
  } else {
    ret = CS_MODE_64;
  }
  return ret;
}

int
find_jmps(Binary *bin)
{
  csh dis;
  Section *text;
  std::map<std::string, std::vector<uint64_t> > jmps;
  
  const uint8_t x86_opc_jmp = 0xff;

  text = bin->get_text_section();
  if(!text) {
    fprintf(stderr, "Nothing to disassemble\n");
    return 0;
  }

  if(cs_open(CS_ARCH_X86, automode(bin->bits), &dis) != CS_ERR_OK) {
    fprintf(stderr, "Failed to open Capstone\n");
    return -1;
  }
  cs_option(dis, CS_OPT_DETAIL, CS_OPT_ON);

  for(size_t i = 0; i < (text->size); i++) {
    if(text->bytes[i] == x86_opc_jmp) {
      if(save_jmps(text, text->vma+i, &jmps, dis) < 0) {
	break;
      }
    }
  }

  for(auto &kv: jmps) {
    printf("%s\t[ ", kv.first.c_str());
    for(auto addr: kv.second) {
      printf("0x%jx ", addr);
    }
    printf("]\n");
  }
  
  cs_close(&dis);
  
  return 0;
}


