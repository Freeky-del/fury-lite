<<<<<<< HEAD
#pragma once
#include "globals.h"

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)

typedef struct _MEM_PARAM {
	void* buffer_address;
	uintptr_t address;
	ULONGLONG size;
	ULONG pid;
	ULONG64 base_address;
	BOOLEAN req_base = FALSE;
	BOOLEAN write = FALSE;
	BOOLEAN read = FALSE;
	char module_name[128];
	void* output;
	NTSTATUS status;
} MEM_PARAM;

typedef struct HandleDisposer {
	using pointer = HANDLE;
	void operator()(HANDLE handle) const {
		if (handle != NULL && handle != INVALID_HANDLE_VALUE) {
			CloseHandle(handle);
		}
	}
};

using unique_handle = std::unique_ptr<HANDLE, HandleDisposer>;

namespace memory {
	using NtHookFunc = uint64_t(__stdcall*)(MEM_PARAM*);

	inline bool call_hook(MEM_PARAM* instr) {
		static NtHookFunc func = nullptr;

		if (!func) {
			HMODULE win32u = LoadLibraryA("win32u.dll");
			if (!win32u) {
				MessageBoxA(0, "[-] Impossibile caricare win32u.dll", "Errore", MB_ICONERROR);
				return false;
			}

			func = reinterpret_cast<NtHookFunc>(
				GetProcAddress(win32u, "NtDxgkGetTrackedWorkloadStatistics")
				);

			//if (!func) {
			//	MessageBoxA(0, "[-] Funzione hook non trovata!", "Errore", MB_ICONERROR);
			//	return false;
			//}
		}

		NTSTATUS status = func(instr);
		return status >= 0;  // STATUS_SUCCESS == 0
	}

	inline std::uint32_t get_process_id(std::string_view process_name) {
		PROCESSENTRY32 processentry;
		const unique_handle snapshot_handle(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL));

		if (snapshot_handle.get() == INVALID_HANDLE_VALUE) return NULL;

		processentry.dwSize = sizeof(PROCESSENTRY32);

		while (Process32Next(snapshot_handle.get(), &processentry) == TRUE) {
			if (process_name.compare(processentry.szExeFile) == NULL) {
				return processentry.th32ProcessID;
			}
		}
		return NULL;
	}

	inline ULONG64 get_module_base_address(const char* module_name) {
		MEM_PARAM instr{};
		instr.pid = globals::pid;
		instr.req_base = TRUE;

		strncpy_s(instr.module_name, module_name, sizeof(instr.module_name) - 1);
		instr.module_name[sizeof(instr.module_name) - 1] = '\0';

		call_hook(&instr);

		//if (!instr.base_address) {
		//	MessageBoxA(0, "[-] Kernel did not return base address.", "Errore", MB_OK);
		//}

		return instr.base_address;
	}

	inline bool write_memory(UINT_PTR write_address, const void* source_data, SIZE_T write_size) {
		if (!source_data || write_size == 0) return false;

		void* local_buffer = VirtualAlloc(NULL, write_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		if (!local_buffer) return false;

		memcpy(local_buffer, source_data, write_size);

		MEM_PARAM instr{};
		instr.write = TRUE;
		instr.base_address = write_address;
		instr.pid = globals::pid;
		instr.buffer_address = local_buffer;
		instr.size = write_size;

		bool result = call_hook(&instr);
		VirtualFree(local_buffer, 0, MEM_RELEASE);

		return result;
	}

	template <class T>
	T read(UINT_PTR read_address) {
		T response{};

		MEM_PARAM instr{};
		instr.read = TRUE;
		instr.pid = globals::pid;
		instr.size = sizeof(T);
		instr.address = read_address;
		instr.output = &response;

		if (!call_hook(&instr) || instr.status != STATUS_SUCCESS) {
			//MessageBoxA(NULL, "Hook Fallito", "Errore", MB_ICONERROR);
			memset(&response, 0, sizeof(T));
		}

		return response;
	}

	template <typename T>
	inline bool read_array(uintptr_t address, T* buffer, size_t count) {
		if (!buffer || count == 0) return false;

		MEM_PARAM instr{};
		instr.read = TRUE;
		instr.pid = globals::pid;
		instr.size = sizeof(T) * count;
		instr.address = address;
		instr.output = buffer;

		bool success = call_hook(&instr);

		// Se vuoi fare logging in debug
#ifdef _DEBUG
		if (!success) {
			std::cerr << "ReadArray failed: address=0x" << std::hex << address << ", size=" << std::dec << instructions.size << std::endl;
		}
#endif

		return success;
	}

	template <typename S>
	inline bool write(UINT_PTR write_address, const S& value) {
		return write_memory(write_address, &value, sizeof(S));
	}
}
=======
#pragma once
#include "globals.h"

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)

typedef struct _MEM_PARAM {
	void* buffer_address;
	uintptr_t address;
	ULONGLONG size;
	ULONG pid;
	ULONG64 base_address;
	BOOLEAN req_base = FALSE;
	BOOLEAN write = FALSE;
	BOOLEAN read = FALSE;
	char module_name[128];
	void* output;
	NTSTATUS status;
} MEM_PARAM;

typedef struct HandleDisposer {
	using pointer = HANDLE;
	void operator()(HANDLE handle) const {
		if (handle != NULL && handle != INVALID_HANDLE_VALUE) {
			CloseHandle(handle);
		}
	}
};

using unique_handle = std::unique_ptr<HANDLE, HandleDisposer>;

namespace memory {
	using NtHookFunc = uint64_t(__stdcall*)(MEM_PARAM*);

	inline bool call_hook(MEM_PARAM* instr) {
		static NtHookFunc func = nullptr;

		if (!func) {
			HMODULE win32u = LoadLibraryA("win32u.dll");
			if (!win32u) {
				MessageBoxA(0, "[-] Impossibile caricare win32u.dll", "Errore", MB_ICONERROR);
				return false;
			}

			func = reinterpret_cast<NtHookFunc>(
				GetProcAddress(win32u, "NtDxgkGetTrackedWorkloadStatistics")
				);

			//if (!func) {
			//	MessageBoxA(0, "[-] Funzione hook non trovata!", "Errore", MB_ICONERROR);
			//	return false;
			//}
		}

		NTSTATUS status = func(instr);
		return status >= 0;  // STATUS_SUCCESS == 0
	}

	inline std::uint32_t get_process_id(std::string_view process_name) {
		PROCESSENTRY32 processentry;
		const unique_handle snapshot_handle(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL));

		if (snapshot_handle.get() == INVALID_HANDLE_VALUE) return NULL;

		processentry.dwSize = sizeof(PROCESSENTRY32);

		while (Process32Next(snapshot_handle.get(), &processentry) == TRUE) {
			if (process_name.compare(processentry.szExeFile) == NULL) {
				return processentry.th32ProcessID;
			}
		}
		return NULL;
	}

	inline ULONG64 get_module_base_address(const char* module_name) {
		MEM_PARAM instr{};
		instr.pid = globals::pid;
		instr.req_base = TRUE;

		strncpy_s(instr.module_name, module_name, sizeof(instr.module_name) - 1);
		instr.module_name[sizeof(instr.module_name) - 1] = '\0';

		call_hook(&instr);

		//if (!instr.base_address) {
		//	MessageBoxA(0, "[-] Nessuna base address restituita dal kernel.", "Errore", MB_OK);
		//}

		return instr.base_address;
	}

	inline bool write_memory(UINT_PTR write_address, const void* source_data, SIZE_T write_size) {
		if (!source_data || write_size == 0) return false;

		void* local_buffer = VirtualAlloc(NULL, write_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		if (!local_buffer) return false;

		memcpy(local_buffer, source_data, write_size);

		MEM_PARAM instr{};
		instr.write = TRUE;
		instr.base_address = write_address;
		instr.pid = globals::pid;
		instr.buffer_address = local_buffer;
		instr.size = write_size;

		bool result = call_hook(&instr);
		VirtualFree(local_buffer, 0, MEM_RELEASE);

		return result;
	}

	template <class T>
	T read(UINT_PTR read_address) {
		T response{};

		MEM_PARAM instr{};
		instr.read = TRUE;
		instr.pid = globals::pid;
		instr.size = sizeof(T);
		instr.address = read_address;
		instr.output = &response;

		if (!call_hook(&instr) || instr.status != STATUS_SUCCESS) {
			//MessageBoxA(NULL, "Hook Fallito", "Errore", MB_ICONERROR);
			memset(&response, 0, sizeof(T));
		}

		return response;
	}

	template <typename T>
	inline bool read_array(uintptr_t address, T* buffer, size_t count) {
		if (!buffer || count == 0) return false;

		MEM_PARAM instr{};
		instr.read = TRUE;
		instr.pid = globals::pid;
		instr.size = sizeof(T) * count;
		instr.address = address;
		instr.output = buffer;

		bool success = call_hook(&instr);

#ifdef _DEBUG
		if (!success) {
			std::cerr << "ReadArray failed: address=0x" << std::hex << address << ", size=" << std::dec << instructions.size << std::endl;
		}
#endif

		return success;
	}

	template <typename S>
	inline bool write(UINT_PTR write_address, const S& value) {
		return write_memory(write_address, &value, sizeof(S));
	}

}
>>>>>>> 72f0e4004c77e605edd9bfe0f871a88bae23b5fb
