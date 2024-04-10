#include <common.h>
#include <elf.h>

#define SYM_NUM 200

typedef struct{
    char name[128];
    Elf64_Addr	value;
    Elf64_Xword	 size;
}Symbols;

int func_cnt;
Symbols sym[SYM_NUM];

void ftrace_init(const char* elf_file){
    //printf("%s\n", elf_file);
    if (elf_file == NULL) assert(0);
    FILE *fp;
    fp = fopen(elf_file, "rb");
    if (fp == NULL) assert(0);

    //read the elf head
    Elf32_Ehdr elf_hd;
    if (fread(&elf_hd, sizeof(Elf32_Ehdr), 1, fp) <= 0) 
        assert(0);

    //e_ident[0,3] = 0x7f, 'E', 'L', 'F' => ELF_FILE
    if (elf_hd.e_ident[0] != 0x7f || 
        strncmp((char*)(elf_hd.e_ident+1), "ELF", 3)) 
        assert(0);

    //move to Section header table file offset
    fseek(fp, elf_hd.e_shoff, SEEK_SET);

    Elf32_Shdr sec_hd;
    //find the string table
    char* string_table = NULL;
    for (int i = 0; i < elf_hd.e_shnum; ++i){
        if (fread(&sec_hd, sizeof(Elf32_Shdr), 1, fp) <= 0)
            assert(0);
        if (sec_hd.sh_type == SHT_STRTAB){
            string_table = (char *)malloc(sec_hd.sh_size);
            fseek(fp, sec_hd.sh_offset, SEEK_SET);
            if (fread(string_table, sec_hd.sh_size, 1, fp) <= 0)
                assert(0);
            break;
        }
    }
    
    //move to Section header table file offset
    fseek(fp, elf_hd.e_shoff, SEEK_SET);

    //find the symbol table
    func_cnt = 0;
    for (int i = 0; i < elf_hd.e_shnum; ++i){
        if (fread(&sec_hd, sizeof(Elf32_Shdr), 1, fp) <= 0)
            assert(0);
        if (sec_hd.sh_type == SHT_SYMTAB){
            fseek(fp, sec_hd.sh_offset, SEEK_SET);
            
            Elf32_Sym symbol_table;
            int cnt = sec_hd.sh_size / sec_hd.sh_entsize;

            for (int j = 0; j < cnt; ++j){
                if (fread(&symbol_table, sec_hd.sh_entsize, 1, fp) <= 0)
                    assert(0);
                if (ELF32_ST_TYPE(symbol_table.st_info) != STT_FUNC)
                    continue;
                
                const char *name = string_table + symbol_table.st_name;
                sym[func_cnt].value = symbol_table.st_value;
                sym[func_cnt].size = symbol_table.st_size;
                strncpy(sym[func_cnt].name, name, sizeof(sym[func_cnt].name) - 1);
                func_cnt++;    
            }
            break;
        }
    }
    fclose(fp);
}

int tab_cnt = -1;
void ftrace_call(vaddr_t pc, vaddr_t dnpc){
    for (int i = 0; i < func_cnt; ++i){
        if (dnpc >= sym[i].value && dnpc < sym[i].size + sym[i].value){
            tab_cnt++;
            printf("0x%08x pc:", pc);
            for (int j = 0; j < tab_cnt; ++j) printf("  ");
            printf("call [%s@0x%08x]\n", sym[i].name, dnpc);
            break;
        }       
    }
}


void ftrace_ret(vaddr_t pc){
    for (int i = 0; i < func_cnt; ++i){
        if (pc >= sym[i].value && pc < sym[i].size + sym[i].value){
            printf("0x%08x pc:", pc);
            for (int j = 0; j < tab_cnt; ++j) printf("  ");
            printf("ret  [%s]\n", sym[i].name);
            tab_cnt--;
            break;
        }       
    }
}