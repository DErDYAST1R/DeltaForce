#pragma once

/* ------------------------------------------------------------------------- */
/* THE DECRYPTION NEEDS TO BE UPDATED TO THE NEW ONE INCLUDED IN THE .README */
/* ------------------------------------------------------------------------- */
 
#include <Zydis/Zydis.h>
#include <unicorn/unicorn.h>

#pragma comment(lib, "Zydis.lib")
#pragma comment(lib, "unicorn.lib")

uc_engine* uc = nullptr;

constexpr uint64_t STACK_BASE = 0x10000000;
constexpr uint64_t STACK_SIZE = 0x100000;
constexpr uint64_t DATA_BASE = 0x20000000;
constexpr uint64_t DATA_SIZE = 0x100000;

void hook_mem_invalid(uc_engine* uc, uc_mem_type type, uint64_t address, int size, int64_t value, void* user_data) {
    //std::cout << "Memory access error at 0x" << std::hex << address << " size: " << size << " type: " << type << std::dec << std::endl;

    uint64_t aligned_addr = address & ~0xFFF;
    uint64_t map_size = 0x1000;

    uc_err err = uc_mem_map(uc, aligned_addr, map_size, UC_PROT_ALL);
    if (err == UC_ERR_OK || err == UC_ERR_MAP) {
        std::vector<uint8_t> buffer(map_size, 0);

        if (ReadMemory((PVOID)aligned_addr, buffer.data(), map_size)) {
            uc_mem_write(uc, aligned_addr, buffer.data(), map_size);
            //std::cout << "Successfully mapped and read remote memory at 0x" << std::hex << aligned_addr << std::dec << std::endl;
        }
        else {
           // std::cout << "Failed to read remote memory, using zeroed memory" << std::endl;
            uc_mem_write(uc, aligned_addr, buffer.data(), map_size);
        }
    }
    else {
       // std::cerr << "Failed to map memory: " << uc_strerror(err) << std::endl;
    }
}

void hook_code(uc_engine* uc, uint64_t address, uint32_t size, void* user_data) {
   // std::cout << "Executing at 0x" << std::hex << address << std::dec << std::endl;
}

bool InitializeUnicorn() {
    uc_err err = uc_open(UC_ARCH_X86, UC_MODE_64, &uc);
    if (err != UC_ERR_OK) {
     //   std::cerr << "Failed to initialize Unicorn: " << uc_strerror(err) << std::endl;
        return false;
    }

    err = uc_mem_map(uc, STACK_BASE, STACK_SIZE, UC_PROT_READ | UC_PROT_WRITE);
    if (err != UC_ERR_OK) {
    //    std::cerr << "Failed to map stack: " << uc_strerror(err) << std::endl;
        return false;
    }

    err = uc_mem_map(uc, DATA_BASE, DATA_SIZE, UC_PROT_READ | UC_PROT_WRITE);
    if (err != UC_ERR_OK) {
     //   std::cerr << "Failed to map data: " << uc_strerror(err) << std::endl;
        return false;
    }

    uc_hook hook;
    uc_hook_add(uc, &hook, UC_HOOK_MEM_READ_UNMAPPED | UC_HOOK_MEM_WRITE_UNMAPPED |
        UC_HOOK_MEM_FETCH_UNMAPPED, (void*)hook_mem_invalid, nullptr, 1, 0);

    return true;
}

void CleanupUnicorn() {
    if (uc) {
        uc_close(uc);
        uc = nullptr;
    }
}

bool EmulateFunction(uint64_t functionAddress, uintptr_t* data, uintptr_t length, uintptr_t offset) {
   // std::cout << "\n=== Starting Emulation at 0x" << std::hex << functionAddress << std::dec << " ===" << std::endl;

    uint64_t code_base = functionAddress & ~0xFFF;
    uint64_t code_size = 0x100000;

    uc_err err = uc_mem_map(uc, code_base, code_size, UC_PROT_ALL);
    if (err != UC_ERR_OK && err != UC_ERR_MAP) {
        //std::cerr << "Failed to map code memory: " << uc_strerror(err) << std::endl;
        return false;
    }

    std::vector<uint8_t> code_buffer(code_size);
    if (!ReadMemory((PVOID)code_base, code_buffer.data(), code_size)) {
       // std::cerr << "Failed to read code from remote process" << std::endl;
        return false;
    }

    err = uc_mem_write(uc, code_base, code_buffer.data(), code_size);
    if (err != UC_ERR_OK) {
        //std::cerr << "Failed to write code: " << uc_strerror(err) << std::endl;
        return false;
    }

    uint64_t dataAddr = DATA_BASE + 0x1000;
    err = uc_mem_write(uc, dataAddr, data, length);
    if (err != UC_ERR_OK) {
       // std::cerr << "Failed to write data: " << uc_strerror(err) << std::endl;
        return false;
    }

    uint64_t rsp = STACK_BASE + STACK_SIZE - 0x1000;
    uint64_t offsetAddr = DATA_BASE + 0x2000;

    err = uc_mem_write(uc, offsetAddr, &offset, sizeof(offset));
    if (err != UC_ERR_OK) {
        //std::cerr << "Failed to write offset: " << uc_strerror(err) << std::endl;
        return false;
    }

    uc_reg_write(uc, UC_X86_REG_RCX, &dataAddr);
    uc_reg_write(uc, UC_X86_REG_RDX, &length);
    uc_reg_write(uc, UC_X86_REG_R8, &offsetAddr);
    uc_reg_write(uc, UC_X86_REG_RSP, &rsp);
    uc_reg_write(uc, UC_X86_REG_RBP, &rsp);

   // std::cout << "Starting emulation:" << std::endl;
   // std::cout << "  Function: 0x" << std::hex << functionAddress << std::dec << std::endl;
   // std::cout << "  RCX (data): 0x" << std::hex << dataAddr << std::dec << std::endl;
   // std::cout << "  RDX (length): 0x" << std::hex << length << std::dec << std::endl;
    //std::cout << "  R8 (offset ptr): 0x" << std::hex << offsetAddr << " (value: " << offset << ")" << std::dec << std::endl;

    err = uc_emu_start(uc, functionAddress, 0, 0, 100000);
    if (err != UC_ERR_OK) {
      //  std::cerr << "Emulation failed: " << uc_strerror(err) << std::endl;

        uint64_t rip;
        uc_reg_read(uc, UC_X86_REG_RIP, &rip);
       // std::cerr << "Failed at RIP: 0x" << std::hex << rip << std::dec << std::endl;

        return false;
    }

    err = uc_mem_read(uc, dataAddr, data, length);
    if (err != UC_ERR_OK) {
       // std::cerr << "Failed to read result: " << uc_strerror(err) << std::endl;
        return false;
    }

   // std::cout << "=== Emulation completed ===" << std::endl;
    return true;
}

bool safe_emulate_decrypt_with_key(uint64_t functionAddress, uint64_t keyAddress, uintptr_t* data, uintptr_t length, uintptr_t offset) {
        uint64_t code_base = functionAddress & ~0xFFF;
        uint64_t code_size = 0x100000;

        uc_err err = uc_mem_map(uc, code_base, code_size, UC_PROT_ALL);
        if (err != UC_ERR_OK && err != UC_ERR_MAP) {
          //  std::cerr << "Failed to map code memory: " << uc_strerror(err) << std::endl;
            return false;
        }

        std::vector<uint8_t> code_buffer(code_size);
        if (!ReadMemory((PVOID)code_base, code_buffer.data(), code_size)) {
         //   std::cerr << "Failed to read code from remote process" << std::endl;
            return false;
        }

        err = uc_mem_write(uc, code_base, code_buffer.data(), code_size);
        if (err != UC_ERR_OK) {
         //   std::cerr << "Failed to write code: " << uc_strerror(err) << std::endl;
            return false;
        }

        uint64_t dataAddr = DATA_BASE + 0x1000;
        err = uc_mem_write(uc, dataAddr, data, length);
        if (err != UC_ERR_OK) {
        //    std::cerr << "Failed to write data: " << uc_strerror(err) << std::endl;
            return false;
        }

        uint64_t offsetAddr = DATA_BASE + 0x2000;
        err = uc_mem_write(uc, offsetAddr, &offset, sizeof(offset));
        if (err != UC_ERR_OK) {
           // std::cerr << "Failed to write offset: " << uc_strerror(err) << std::endl;
            return false;
        }

        uint64_t rsp = STACK_BASE + STACK_SIZE - 0x1000;

        uc_reg_write(uc, UC_X86_REG_RCX, &keyAddress);
        uc_reg_write(uc, UC_X86_REG_RDX, &dataAddr);
        uc_reg_write(uc, UC_X86_REG_R8, &length);
        uc_reg_write(uc, UC_X86_REG_R9, &offset);
        uc_reg_write(uc, UC_X86_REG_RSP, &rsp);
        uc_reg_write(uc, UC_X86_REG_RBP, &rsp);

        err = uc_emu_start(uc, functionAddress, 0, 0, 100000);

        if (err != UC_ERR_OK) {

          //  std::cerr << "Emulation failed: " << uc_strerror(err) << std::endl;

            uint64_t rip;
            uc_reg_read(uc, UC_X86_REG_RIP, &rip);
          //  std::cerr << "Failed at RIP: 0x" << std::hex << rip << std::dec << std::endl;

            return false;
        }

        err = uc_mem_read(uc, dataAddr, data, length);
        if (err != UC_ERR_OK) {
          //  std::cerr << "Failed to read result: " << uc_strerror(err) << std::endl;
            return false;
        }

        return true;
}

typedef void(__fastcall* DecryptFunc)(void*, uintptr_t*, uintptr_t, uintptr_t);
typedef void(__fastcall* DefaultDecryptFunc)(uintptr_t*, uintptr_t, uintptr_t*);

bool safe_emulate_decrypt_default(uint64_t functionAddress, uintptr_t* data, uintptr_t length, uintptr_t* offset) {
    __try {
        return EmulateFunction(functionAddress, data, length, *offset);
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
      //  std::cerr << "Exception during default emulation" << std::endl;
        return false;
    }
}

uint64_t decrypt_pointer_proper(uint64_t encrypted_ptr) {
    if (!wegame)
    {
        return decrypt_pointer_simple(encrypted_ptr);
    }

    uintptr_t PersistentLevel = encrypted_ptr;
    uintptr_t v17 = PersistentLevel >> 4;

    if (v17) {
        uintptr_t v19 = read<uintptr_t>(0x154350830);
        uintptr_t v20 = PersistentLevel & 0xF;
        bool v9 = (v19 >> v20) & 1;
        uintptr_t v34 = PersistentLevel >> 4;

        if (v9) {
            bool success = false;

            switch (read<uintptr_t>(0x1543507E8)) {
            case 1:
                success = safe_emulate_decrypt_with_key(0x14099D820, 0x15435AB98, &v34, 4, (unsigned int)v20);
                break;
            case 2:
                success = safe_emulate_decrypt_with_key(0x140973130, 0x15435ABA8, &v34, 4, (unsigned int)v20);
                break;
            case 3:
                success = safe_emulate_decrypt_with_key(0x1409813D0, 0x15435ABB8, &v34, 4, (unsigned int)v20);
                break;
            case 4:
                success = safe_emulate_decrypt_with_key(0x14098F3A0, 0x15435ABC8, &v34, 4, (unsigned int)v20);
                break;
            default: {
                uintptr_t v32 = v20;
                uint64_t default_func = read<uintptr_t>(0x15432F5A8);
                success = safe_emulate_decrypt_default(default_func, &v34, 4, &v32);
                break;
            }
            }

            if (!success) {
              //  std::cerr << "Decryption failed, returning original value" << std::endl;
                return v17;
            }

            uintptr_t v18 = v34 & 0xFFFFFFFFFFFFFFF0ULL;
            return v18;
        }
    }

    return v17;
}