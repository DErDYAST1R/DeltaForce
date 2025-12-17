#pragma once

#include <Psapi.h>

struct objInf {
    uintptr_t entityaddy;
    uintptr_t playerstate;
    uintptr_t Mesh;
    uintptr_t playerpawn;
    std::string ClassName;
    uintptr_t BoneArray;
    bool isBot;
    std::string playername;
    ImColor TeamColor;
};
 
static std::mutex cacheMtx;
std::vector<objInf> get_entities = {};

enum class EMarkingItemType : uint8_t {
    None = 0,
    CommonItem = 1,
    LootingItem = 2,
    Container = 3,
    DyingTeammate = 4,
    DeadBody = 5,
    Weapon = 6,
    EngineerSonicTrap = 7,
    Vehicle = 8,
    TacticalEquip = 9,
    PendingDeathCharacter = 10,
    SectorAnchor = 11,
    MandelBrickInteractor = 12,
    MandelBrickDecipherDevice = 13,
    COUNT = 14,
    EMarkingItemType_MAX = 15
};

struct objItemInf {
    uintptr_t entityaddy;
    std::string ClassName;
    EMarkingItemType MarkingItemType;
};

static std::mutex cacheMtx2;
std::vector<objItemInf> get_items = {};


int processid;
uintptr_t process_base;
uintptr_t process_cr3;

#define DEVICE_NAME         L"\\Device\\MonoStraif64Device"
#define SYMBOLIC_LINK_NAME  L"\\??\\MonoStraif64Link"
#define USER_DEVICE_NAME    "\\\\.\\MonoStraif64Link"

#define IOCTL_PROCESS_REQUEST \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)

// standard response codes
#define SUCCESS            0x00000000
#define ERROR_INVALID_SIZE 0x00000001
#define ERROR_INVALID_DATA 0x00000002
#define ERROR_NO_MEMORY    0x00000003
#define ERROR_UNKNOWN      0x000000FF

#define MAX_DATA_SIZE   4096

// ---- Structures ----

typedef struct _REQUEST {
    ULONG requestId;
    ULONG dataLength;
    UCHAR buffer[MAX_DATA_SIZE];
} REQUEST, * P_REQUEST;

typedef struct _RESPONSE {
    ULONG requestId;
    ULONG responseCode;
    ULONG dataLength;
    UCHAR buffer[MAX_DATA_SIZE];
} RESPONSE, * P_RESPONSE;

typedef struct _REQUEST_DATA
{
    ULONG64 SecurityCode1;
    ULONG64 SecurityCode2;

    union
    {
        struct
        {
            ULONG64 processId;
            ULONG64 cr3Value;
            bool sizecheck;
        } Cr3Data;

        struct
        {
            ULONG64 Address;
            ULONG64 Buffer;
            ULONG Size;
            ULONGLONG return_size;

            BOOLEAN isWrite;
            BOOLEAN noncachedread;
        } ReadWriteMemoryData;

        struct
        {
            ULONG64 processId;
            ULONG64 baseadressvalue;
        } ProcessBaseData;

        struct
        {
            long MouseX;
            long MouseY;
            unsigned short MouseButtons;
        } MouseData;

        struct
        {
            uintptr_t window;
            unsigned int value;
        } WindowHideData;

        struct
        {
            ULONG64 PhysicalAddress;
            ULONG64 Buffer;
            ULONG Size;
            ULONGLONG return_size;
        } PhysicalReadData;

        struct
        {
            char kernelmodulename[256];
            ULONG64 kernelmodulebase;
        } KernelModuleData;

        struct
        {
            char modulename[256];
            ULONG64 modulebase;
            ULONGLONG ImageSize;
        } ModuleData;

        bool Result;
    };

} REQUEST_DATA, * PREQUEST_DATA;

typedef struct _SHARED_COMM_BUFFER {
    volatile LONG requestReady;
    volatile LONG responseReady;
    REQUEST request;
    RESPONSE response;

} SHARED_COMM_BUFFER, * PSHARED_COMM_BUFFER;

typedef enum _REQUEST_ID {
    RequestId_None = 0,
    RequestId_Test,
    RequestId_GetBase,
    RequestId_GetCr3_1,
    RequestId_GetCr3_2,
    RequestId_SetCr3,
    RequestId_ReadWrite,
    RequestId_HideWindow,
    RequestId_GetKernelModuleBase,
    RequestId_MouseMovements,
    RequestId_GetProcessModuleBase,

    RequestId_Spoof_MAC,
    RequestId_Spoof_DISK,
    RequestId_Spoof_GPU,
    RequestId_Spoof_BOOTINFO,
    RequestId_Spoof_FIRMWAREENTRY,
    RequestId_Spoof_ARP,
    RequestId_Spoof_SMBIOSNULL,
    RequestId_Spoof_TPMNULL,
} REQUEST_ID;

class Communication {
public:
    virtual ~Communication() = default;

    virtual bool initialize() = 0;
    virtual void cleanup() = 0;
    virtual bool sendData(const REQUEST* request, RESPONSE* response) = 0;
    virtual bool isActive() const = 0;
    virtual const char* getLastError() const = 0;
};

class SharedMemoryCommunication : public Communication {
public:
    HANDLE deviceHandle;
private:
    HANDLE sectionHandle;
    PSHARED_COMM_BUFFER sharedBuffer;
    char lastError[256];

    void setLastError(const char* error) {
        strncpy_s(lastError, sizeof(lastError), error, _TRUNCATE);
    }

public:
    SharedMemoryCommunication() : deviceHandle(INVALID_HANDLE_VALUE), sectionHandle(NULL), sharedBuffer(nullptr) {
        memset(lastError, 0, sizeof(lastError));
    }

    ~SharedMemoryCommunication() override {
        cleanup();
    }

    bool initialize() override {
        deviceHandle = CreateFileA(
            USER_DEVICE_NAME,
            GENERIC_READ | GENERIC_WRITE,
            0,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

        if (deviceHandle == INVALID_HANDLE_VALUE) {
            DWORD error = GetLastError();
            sprintf_s(lastError, "failed to open device: error code %lu", error);
            return false;
        }

        sectionHandle = OpenFileMappingW(
            FILE_MAP_ALL_ACCESS,
            FALSE,
            L"Global\\MonoStraif64Sec"
        );

        if (!sectionHandle) {
            DWORD error = GetLastError();
            sprintf_s(lastError, "failed to open section: error code %lu", error);
            CloseHandle(deviceHandle);
            deviceHandle = INVALID_HANDLE_VALUE;
            return false;
        }

        sharedBuffer = (PSHARED_COMM_BUFFER)MapViewOfFile(
            sectionHandle,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            sizeof(SHARED_COMM_BUFFER)
        );

        if (!sharedBuffer) {
            DWORD error = GetLastError();
            sprintf_s(lastError, "failed to map view: error code %lu", error);
            CloseHandle(sectionHandle);
            CloseHandle(deviceHandle);
            sectionHandle = NULL;
            deviceHandle = INVALID_HANDLE_VALUE;
            return false;
        }

        return true;
    }

    void cleanup() override {
        if (sharedBuffer) {
            UnmapViewOfFile(sharedBuffer);
            sharedBuffer = nullptr;
        }

        if (sectionHandle) {
            CloseHandle(sectionHandle);
            sectionHandle = NULL;
        }

        if (deviceHandle != INVALID_HANDLE_VALUE) {
            CloseHandle(deviceHandle);
            deviceHandle = INVALID_HANDLE_VALUE;
        }
    }

    bool sendData(const REQUEST* request, RESPONSE* response) override
    {
        if (!sharedBuffer || deviceHandle == INVALID_HANDLE_VALUE) {
            // setLastError("device not initialized");
            return false;
        }

        // FIX: validate request length against shared buffer and response buffer
        if (request->dataLength > sizeof(sharedBuffer->request.buffer) ||
            request->dataLength > sizeof(response->buffer)) {
            // setLastError("dataLength too big");
            return false;
        }

        // Write request into shared buffer
        sharedBuffer->request.requestId = request->requestId;
        sharedBuffer->request.dataLength = request->dataLength;
        memcpy(sharedBuffer->request.buffer, request->buffer, request->dataLength);

        // Handshake flags: reset response, mark request ready
        InterlockedExchange(&sharedBuffer->responseReady, 0);
        InterlockedExchange(&sharedBuffer->requestReady, 1);

        BYTE outBuf[sizeof(RESPONSE)] = {};
        DWORD bytesReturned = 0;

        BOOL result = DeviceIoControl(
            deviceHandle,
            IOCTL_PROCESS_REQUEST,
            nullptr,
            0,
            outBuf,
            sizeof(RESPONSE),
            &bytesReturned,
            nullptr
        );

        if (!result)
            return false;

        // FIX: use ULONGLONG for GetTickCount64, and keep your timeout loop
        const ULONGLONG timeoutMs = 5000;
        const ULONGLONG start = GetTickCount64();

        while (InterlockedCompareExchange(
            &sharedBuffer->responseReady, 0, 0) != 1)
        {
            if (GetTickCount64() - start > timeoutMs) {
                // printf("[UM] ERROR: Timeout waiting for response\n");
                return false;
            }
            Sleep(1);
        }

        // Response is ready ?copy it out
        response->requestId = sharedBuffer->response.requestId;
        response->responseCode = sharedBuffer->response.responseCode;
        response->dataLength = sharedBuffer->response.dataLength;

        // (Optional) sanity check:
        if (response->dataLength > sizeof(response->buffer)) {
            // BUG in driver / protocol; avoid overflow
            return false;
        }

        memcpy(response->buffer,
            sharedBuffer->response.buffer,
            response->dataLength);

        // Reset responseReady for next call
        InterlockedExchange(&sharedBuffer->responseReady, 0);

        return true;
    }

    bool isActive() const override {
        return deviceHandle != INVALID_HANDLE_VALUE && sharedBuffer != nullptr;
    }

    const char* getLastError() const override {
        return lastError;
    }
}; SharedMemoryCommunication* comm = new SharedMemoryCommunication();

void InitRequest(REQUEST& req, REQUEST_DATA& data, ULONG id)
{
    memset(&req, 0, sizeof(REQUEST));
    memset(&data, 0, sizeof(REQUEST_DATA));

    req.requestId = id;
    req.dataLength = sizeof(REQUEST_DATA);
}

bool ExtractResponse(REQUEST_DATA& out, const RESPONSE& resp)
{
    if (resp.responseCode != SUCCESS)
        return false;

    if (resp.dataLength < sizeof(REQUEST_DATA))
        return false;

    memcpy(&out, resp.buffer, sizeof(REQUEST_DATA));
    return true;
}

bool GetBaseAddress(ULONG pid, ULONG64& baseAddr)
{
    REQUEST req = { 0 };
    RESPONSE resp = { 0 };
    REQUEST_DATA data = { 0 };

    InitRequest(req, data, RequestId_GetBase);

    data.ProcessBaseData.processId = pid;

    memcpy(req.buffer, &data, sizeof(REQUEST_DATA));

    if (!comm->sendData(&req, &resp))
        return false;

    REQUEST_DATA returned = { 0 };

    if (!ExtractResponse(returned, resp))
        return false;

    baseAddr = returned.ProcessBaseData.baseadressvalue;
    return true;
}


bool ReadMemory(PVOID address, PVOID buffer, uint32_t size)
{
    if (!comm || !comm->isActive())
        return false;

    REQUEST req = { 0 };
    RESPONSE resp = { 0 };
    REQUEST_DATA reqData = { 0 };

    InitRequest(req, reqData, RequestId_ReadWrite);

    reqData.ReadWriteMemoryData.Address = (ULONG64)address;
    reqData.ReadWriteMemoryData.Buffer = (ULONG64)buffer;
    reqData.ReadWriteMemoryData.Size = size;
    reqData.ReadWriteMemoryData.isWrite = FALSE;
    reqData.ReadWriteMemoryData.noncachedread = TRUE;

    memcpy(req.buffer, &reqData, sizeof(REQUEST_DATA));

    if (!comm->sendData(&req, &resp))
        return false;

    REQUEST_DATA returned = { 0 };

    if (!ExtractResponse(returned, resp))
        return false;

    return (returned.ReadWriteMemoryData.return_size > 0);
}

bool WriteMemory(PVOID address, PVOID buffer, uint32_t size)
{
    REQUEST req = { 0 };
    RESPONSE resp = { 0 };
    REQUEST_DATA reqData = { 0 };

    InitRequest(req, reqData, RequestId_ReadWrite);

    reqData.ReadWriteMemoryData.Address = (ULONG64)address;
    reqData.ReadWriteMemoryData.Buffer = (ULONG64)buffer;
    reqData.ReadWriteMemoryData.Size = size;
    reqData.ReadWriteMemoryData.isWrite = TRUE;
    reqData.ReadWriteMemoryData.noncachedread = FALSE;

    memcpy(req.buffer, &reqData, sizeof(REQUEST_DATA));

    if (!comm->sendData(&req, &resp))
        return false;

    REQUEST_DATA returned = { 0 };
    if (!ExtractResponse(returned, resp))
        return false;

    return (returned.ReadWriteMemoryData.return_size == size);
}

bool GetProcessCr3(ULONG pid, ULONG64& cr3)
{
    REQUEST req = { 0 };
    RESPONSE resp = { 0 };
    REQUEST_DATA data = { 0 };

    InitRequest(req, data, RequestId_GetCr3_1);

    data.Cr3Data.processId = pid;

    memcpy(req.buffer, &data, sizeof(REQUEST_DATA));

    if (!comm->sendData(&req, &resp))
        return false;

    REQUEST_DATA returned;
    if (!ExtractResponse(returned, resp))
        return false;

    cr3 = returned.Cr3Data.cr3Value;
    return returned.Result;
}

bool GetProcessCr3_2(ULONG pid, ULONG64& cr3, bool sizecheck)
{
    REQUEST req = { 0 };
    RESPONSE resp = { 0 };
    REQUEST_DATA data = { 0 };

    InitRequest(req, data, RequestId_GetCr3_2);

    data.Cr3Data.processId = pid;
    data.Cr3Data.sizecheck = sizecheck;

    memcpy(req.buffer, &data, sizeof(REQUEST_DATA));

    if (!comm->sendData(&req, &resp))
        return false;

    REQUEST_DATA returned;
    if (!ExtractResponse(returned, resp))
        return false;

    cr3 = returned.Cr3Data.cr3Value;
    return returned.Result;
}

bool SetCr3(ULONG64 newCr3)
{
    REQUEST req = { 0 };
    RESPONSE resp = { 0 };
    REQUEST_DATA data = { 0 };

    InitRequest(req, data, RequestId_SetCr3);

    data.Cr3Data.cr3Value = newCr3;

    memcpy(req.buffer, &data, sizeof(REQUEST_DATA));

    if (!comm->sendData(&req, &resp))
        return false;

    REQUEST_DATA returned;
    if (!ExtractResponse(returned, resp))
        return false;

    return returned.Result;
}

bool GetKernelModuleBase(const char* name, ULONG64& base)
{
    REQUEST req = { 0 };
    RESPONSE resp = { 0 };
    REQUEST_DATA data = { 0 };

    InitRequest(req, data, RequestId_GetKernelModuleBase);

    strncpy(data.KernelModuleData.kernelmodulename, name, 255);

    memcpy(req.buffer, &data, sizeof(REQUEST_DATA));

    if (!comm->sendData(&req, &resp))
        return false;

    REQUEST_DATA ret;
    if (!ExtractResponse(ret, resp))
        return false;

    base = ret.KernelModuleData.kernelmodulebase;
    return ret.Result;
}

bool GetProcessModuleBase(ULONG pid, const char* name, ULONG64& baseOut, ULONG64& sizeOut)
{
    REQUEST req = { 0 };
    RESPONSE resp = { 0 };
    REQUEST_DATA data = { 0 };

    InitRequest(req, data, RequestId_GetProcessModuleBase);

    data.ModuleData.ImageSize = 0;
    strncpy(data.ModuleData.modulename, name, 255);

    memcpy(req.buffer, &data, sizeof(REQUEST_DATA));

    if (!comm->sendData(&req, &resp))
        return false;

    REQUEST_DATA ret;
    if (!ExtractResponse(ret, resp))
        return false;

    baseOut = ret.ModuleData.modulebase;
    sizeOut = ret.ModuleData.ImageSize;

    return ret.Result;
}

bool MouseMove(long x, long y, unsigned short buttons)
{
    REQUEST req = { 0 };
    RESPONSE resp = { 0 };
    REQUEST_DATA data = { 0 };

    InitRequest(req, data, RequestId_MouseMovements);

    data.MouseData.MouseX = x;
    data.MouseData.MouseY = y;
    data.MouseData.MouseButtons = buttons;

    memcpy(req.buffer, &data, sizeof(REQUEST_DATA));

    return comm->sendData(&req, &resp);
}

bool HideWindow(uintptr_t hwnd, unsigned int affinity)
{
    REQUEST req = { 0 };
    RESPONSE resp = { 0 };
    REQUEST_DATA data = { 0 };

    InitRequest(req, data, RequestId_HideWindow);

    data.WindowHideData.window = hwnd;
    data.WindowHideData.value = affinity;

    memcpy(req.buffer, &data, sizeof(REQUEST_DATA));

    return comm->sendData(&req, &resp);
}

INT32 find_process(LPCTSTR process_name) {
    PROCESSENTRY32 pt;
    HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pt.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hsnap, &pt)) {
        do {
            if (!lstrcmpi(pt.szExeFile, process_name)) {
                CloseHandle(hsnap);
                return pt.th32ProcessID;
            }
        } while (Process32Next(hsnap, &pt));
    }
    CloseHandle(hsnap);

    return { NULL };
}

bool is_valid(const uint64_t address)
{
    if (address == 0 ||
        address == 0xCCCCCCCCCCCCCCCC ||
        address == 0xFFFFFFFFFFFFFFFF)
        return false;

    if (address < 0x400000 || address > 0x7FFFFFFFFFFFFFFF)
        return false;

    return true;
}

template <typename T>
T read(uint64_t address) {
    T buffer{ };
    if (is_valid(address))
        ReadMemory((PVOID)address, &buffer, sizeof(T));
    return buffer;
}

template <typename T>
void write(uint64_t address, T buffer) {
    if (is_valid(address))
        WriteMemory((PVOID)address, &buffer, sizeof(T));
}

uint64_t decrypt_pointer_proper(uint64_t encrypted_ptr);

inline uintptr_t decrypt_pointer_simple(uintptr_t encrypted_ptr)
{
	if (wegame)
		return decrypt_pointer_proper(encrypted_ptr);
		//Drv->Write<int>(Drv->process_base + 0x93334EE, 0);//0x142219CC


	uintptr_t v31 = encrypted_ptr >> 4;
	encrypted_ptr = v31 & 0xFFFFFFFFFFFFFFF0ULL;
	return encrypted_ptr;
}

struct SectionInfo {
	uintptr_t start;
	size_t size;
};

bool GetTextSectionInfo(uint64_t baseAddress, SectionInfo& textSection) {
	// Read DOS header
	IMAGE_DOS_HEADER dosHeader;
	if (!ReadMemory((PVOID)baseAddress, &dosHeader, sizeof(dosHeader)) || dosHeader.e_magic != IMAGE_DOS_SIGNATURE) {
		return false;
	}

	// Read NT headers signature
	uint32_t peSignature;
	uint64_t ntHeadersAddr = baseAddress + dosHeader.e_lfanew;
	if (!ReadMemory((PVOID)ntHeadersAddr, &peSignature, sizeof(peSignature)) || peSignature != IMAGE_NT_SIGNATURE) {
		return false;
	}

	// Read file header
	IMAGE_FILE_HEADER fileHeader;
	if (!ReadMemory((PVOID)(ntHeadersAddr + 4), &fileHeader, sizeof(fileHeader))) {
		return false;
	}

	// Locate section headers
	uint64_t sectionHeaderAddr = ntHeadersAddr + 4 + sizeof(fileHeader) + fileHeader.SizeOfOptionalHeader;

	// Iterate through sections to find .text
	for (int i = 0; i < fileHeader.NumberOfSections; i++) {
		IMAGE_SECTION_HEADER sectionHeader;
		if (!ReadMemory((PVOID)(sectionHeaderAddr + (i * sizeof(sectionHeader))), &sectionHeader, sizeof(sectionHeader))) {
			continue;
		}

		if (strncmp((const char*)sectionHeader.Name, ".text", 5) == 0) {
			textSection.start = baseAddress + sectionHeader.VirtualAddress;
			textSection.size = sectionHeader.Misc.VirtualSize;
			return true;
		}
	}

	return false;
}

bool PatternToBytes(const std::string& pattern, std::vector<uint8_t>& bytes, std::vector<bool>& mask) {
	bytes.clear();
	mask.clear();

	std::string currentByte;
	for (char ch : pattern) {
		if (ch == ' ') {
			if (!currentByte.empty()) {
				if (currentByte == "?") {
					bytes.push_back(0);
					mask.push_back(false);
				}
				else if (currentByte.length() == 2 &&
					isxdigit(currentByte[0]) && isxdigit(currentByte[1])) {
					bytes.push_back(static_cast<uint8_t>(std::stoul(currentByte, nullptr, 16)));
					mask.push_back(true);
				}
				else {
					return false;
				}
				currentByte.clear();
			}
		}
		else {
			currentByte += ch;
		}
	}

	if (!currentByte.empty()) {
		if (currentByte == "?") {
			bytes.push_back(0);
			mask.push_back(false);
		}
		else if (currentByte.length() == 2 &&
			isxdigit(currentByte[0]) && isxdigit(currentByte[1])) {
			bytes.push_back(static_cast<uint8_t>(std::stoul(currentByte, nullptr, 16)));
			mask.push_back(true);
		}
		else {
			return false;
		}
	}

	return !bytes.empty();
}

uintptr_t ScanTextSection(const std::string& pattern, uintptr_t* outRva = nullptr) {
	// Get .text section information
	SectionInfo textSection;
	if (!GetTextSectionInfo(process_base, textSection)) {
		std::cerr << "Failed to locate .text section" << std::endl;
		return 0;
	}

	// Convert pattern to bytes
	std::vector<uint8_t> patternBytes;
	std::vector<bool> mask;
	if (!PatternToBytes(pattern, patternBytes, mask)) {
		std::cerr << "Invalid pattern format!" << std::endl;
		return 0;
	}

	const size_t patternSize = patternBytes.size();
	if (patternSize == 0 || patternSize > textSection.size) {
		return 0;
	}

	// Scan in chunks for efficiency
	const size_t chunkSize = 4096;
	std::vector<uint8_t> chunk(chunkSize + patternSize - 1);

	for (size_t i = 0; i < textSection.size - patternSize; i += chunkSize - patternSize + 1) {
		size_t bytesToRead = min(chunkSize + patternSize - 1, textSection.size - i);

		if (!ReadMemory((PVOID)(textSection.start + i), chunk.data(), bytesToRead)) {
			continue;
		}

		for (size_t j = 0; j <= bytesToRead - patternSize; j++) {
			bool match = true;
			for (size_t k = 0; k < patternSize; k++) {
				if (mask[k] && chunk[j + k] != patternBytes[k]) {
					match = false;
					break;
				}
			}

			if (match) {
				uintptr_t foundAddress = textSection.start + i + j;
				if (outRva) {
					*outRva = foundAddress - process_base;
				}
				return foundAddress;
			}
		}
	}

	return 0;
}

struct DwordResult {
	uintptr_t instruction_address; // Address where pattern was found
	uintptr_t dword_address;      // The DWORD address used in instruction
	uintptr_t dword_rva;          // RVA of the DWORD
};

DwordResult ScanAndExtractDword(const std::string& pattern, size_t offset = 0) {
	DwordResult result{ 0, 0, 0 };

	SectionInfo textSection;
	if (!GetTextSectionInfo(process_base, textSection)) {
		std::cerr << "Failed to locate .text section" << std::endl;
		return result;
	}

	std::vector<uint8_t> patternBytes;
	std::vector<bool> mask;
	if (!PatternToBytes(pattern, patternBytes, mask)) {
		std::cerr << "Invalid pattern format!" << std::endl;
		return result;
	}

	const size_t patternSize = patternBytes.size();
	if (patternSize == 0 || patternSize > textSection.size) {
		return result;
	}

	const size_t chunkSize = 4096;
	std::vector<uint8_t> chunk(chunkSize + patternSize - 1);

	for (size_t i = 0; i < textSection.size - patternSize; i += chunkSize - patternSize + 1) {
		size_t bytesToRead = min(chunkSize + patternSize - 1, textSection.size - i);

		if (!ReadMemory((PVOID)(textSection.start + i), chunk.data(), bytesToRead)) {
			continue;
		}

		for (size_t j = 0; j <= bytesToRead - patternSize; j++) {
			bool match = true;
			for (size_t k = 0; k < patternSize; k++) {
				if (mask[k] && chunk[j + k] != patternBytes[k]) {
					match = false;
					break;
				}
			}

			if (match) {
				result.instruction_address = textSection.start + i + j;

				// 2. Extract the DWORD address from the instruction
				uintptr_t rip = result.instruction_address + offset + 4; // +4 for x64 RIP-relative
				int32_t rip_offset;

				if (!ReadMemory((PVOID)(result.instruction_address + offset), &rip_offset, sizeof(rip_offset))) {
					std::cerr << "Failed to read RIP offset at 0x"
						<< std::hex << (result.instruction_address + offset) << std::endl;
					return { 0, 0, 0 };
				}

				// Calculate absolute address of the DWORD
				result.dword_address = rip + rip_offset;
				result.dword_rva = result.dword_address - process_base;
				return result;
			}
		}
	}

	return result;
}

void getoffsetsfrompatter()
{
	DwordResult Gworldpattern = ScanAndExtractDword("48 8B 35 ? ? ? ? EB ? 48 8B 0D ? ? ? ? 0F B6 05 ? ? ? ? 48 89 4D ? 34 ? 80 F1", 3);
	DwordResult Fnamepattern = ScanAndExtractDword("48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8B F0 C6 05 ? ? ? ? ? C6 45", 3);
	DwordResult localplayerpattern = ScanAndExtractDword("48 89 1D ? ? ? ? F2 0F 11 05 ? ? ? ? C7 05", 3);
	//DwordResult BitTest1scan = ScanAndExtractDword("8B 05 ? ? ? ? 0F A3 C8 73 ? 83 3D ? ? ? ? ? 75 ? 44 8B C9 48 8D 55 ? 48 8D 0D ? ? ? ? 41 B8 ? ? ? ? E8 ? ? ? ? EB ? 66 89 4D ? 4C 8D 45 ? 48 8D 4D ? C6 45 ? ? BA ? ? ? ? FF 15 ? ? ? ? 48 F7 45 ? ? ? ? ? 48 8D 9F", 3);
	//DwordResult BitTest1scan = ScanAndExtractDword("8B 05 ? ? ? ? 83 E2 ?", 3);

    //printf("BitTest1scan:     0x%llx\n", BitTest1scan.dword_rva);
   // printf("BitTest1scan:     0x%llx\n", BitTest1scan.dword_address);
   // printf("BitTest1scan:     0x%llx\n", BitTest1scan.instruction_address);
	printf("Gworldpattern:     0x%llx\n", Gworldpattern.dword_rva);
	printf("Fnamepattern:     0x%llx\n", Fnamepattern.dword_rva);
	printf("localplayerpattern:     0x%llx\n", localplayerpattern.dword_rva);

	Offsets::GWorld = Gworldpattern.dword_rva;
	Offsets::GNames = Fnamepattern.dword_rva;
	Offsets::localplayerscan = localplayerpattern.dword_address;
	//Offsets::BitTest1scan = BitTest1scan.dword_rva;

	//uintptr_t gword = process.Read<uintptr_t>(process.process_base + offsets::GWorld);
	//printf("alternative_localplayers:     0x%llx\n", alternative_localplayers);
	//printf("BitTest1scan:     0x%llx\n", Offsets::BitTest1scan);

}

std::string findProcessPath_NoOpenProcess(const char* procName)
{
	std::string path;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe{ sizeof(PROCESSENTRY32) };

	if (Process32First(snapshot, &pe)) {
		do {
			if (!std::strcmp(procName, pe.szExeFile)) {
				HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pe.th32ProcessID);
				if (hProcess) {
					char buffer[MAX_PATH];
					DWORD size = sizeof(buffer);
					if (QueryFullProcessImageNameA(hProcess, 0, buffer, &size)) {
						path = buffer;
					}
					CloseHandle(hProcess);
				}
				break;
			}
		} while (Process32Next(snapshot, &pe));
	}

	CloseHandle(snapshot);
	return path;
}

#include <filesystem>

bool deleteFileTwoUp(const std::string& exePath, const std::string& relativeFile)
{
	namespace fs = std::filesystem;

	fs::path exeDir = fs::path(exePath).parent_path();    // e.g. ...\Binaries\Win64
	fs::path target = exeDir / "../.." / relativeFile;    // go 2 up, then to target
	target = fs::weakly_canonical(target);                // normalize safely

	std::cout << "Target file path: " << target << "\n";

	if (!fs::exists(target)) {
		std::cerr << "File does not exist.\n";
		return false;
	}

	if (fs::equivalent(target, exePath)) {
		std::cerr << "Refusing to delete the EXE itself.\n";
		return false;
	}

	std::error_code ec;
	if (!fs::remove(target, ec)) {
		std::cerr << "Failed to delete: " << ec.message() << "\n";
		return false;
	}

	std::cout << "Deleted: " << target << "\n";
	return true;
}