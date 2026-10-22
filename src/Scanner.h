#pragma once
#include "./process.h"
#include "ErrorReporter.h"
#include <any>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <print>





struct MemorySnapshot
{
	uintptr_t start;
	std::vector<uint8_t> bytes;
};
struct memoryBeforeValues
{
	uintptr_t memoryAddr;
	int value;
};
class Scanner
{
	private:
		Process proc;
		std::vector<MemorySnapshot> memorySnapshot;
		std::vector<uintptr_t> memoryAddrList;
		std::unordered_map<uintptr_t, int64_t> memoryValuesBefore;
		bool firstScan = false;
	public:
        bool fastScan = true;
	Scanner();

	

    template <typename T>
    size_t scanStep() const
    {
        if (fastScan)
            return sizeof(T);
        else
            return 1;
    }

	template <typename T>
	bool scanExact(T target)
	{
		firstScan = false;
		memoryAddrList.clear();
		T tempValue = target;
		if (!Scanner::newScan()) 
		{
			std::println("error 1");
			return false;
		}
		memoryValuesBefore.clear();
		std::vector<MemorySnapshot> snapshotBefore = Scanner::memorySnapshot;
		
		for (int i = 0; snapshotBefore.size() > i ; i++)
		{
			for (size_t j = 0; j + sizeof(T) <= snapshotBefore[i].bytes.size(); j += scanStep<T>())
			{
				std::memcpy(&target, snapshotBefore[i].bytes.data() + j,sizeof(target));
				uintptr_t memoryAddr = snapshotBefore[i].start + j;
				if (tempValue == target) 
				{
					Scanner::memoryAddrList.push_back(memoryAddr);
					saveMemoryValue(memoryAddr, target);
				}
			}
		
		}
		return true;
	}
	template<typename T>
	void scanUnknown()
	{
		reset();
		T tempValue;
		if (!Scanner::newScan()) 
		{
			std::println("error 1");
		}
		memoryValuesBefore.clear();
		const std::vector<MemorySnapshot>& snapshotBefore = Scanner::memorySnapshot;
		firstScan = true;
		
	}
	template <typename T>bool rescanExact(T target)
	{
		if (firstScan) return scanExact(target);
		T tempValue = target;
		std::vector<uintptr_t> newMemoryAddrList;
		Scanner::proc.attatch();
		for (int i = 0; Scanner::memoryAddrList.size() > i; i++) 
		{
			T value = readValue<T>(memoryAddrList[i]);
			if (tempValue == value) 
			{
				newMemoryAddrList.push_back(Scanner::memoryAddrList[i]);
		changeMemoryValueBefore(memoryAddrList[i], value);
			}
		}
		Scanner::proc.detatch();
		Scanner::memoryAddrList = newMemoryAddrList;
		return true;
	}
	
	template<typename T>
	bool rescanGreater()
	{
		if (firstScan)
		{
			T before;
			T after;
			std::vector<uintptr_t> newMemoryAddrList;
            proc.attatch();
			for(int i = 0; i < memorySnapshot.size(); i++)
			{
				std::vector<uint8_t> now = proc.readMemory(memorySnapshot[i].start, memorySnapshot[i].bytes.size());
				for (size_t j = 0; j + sizeof(T) <= now.size(); j += scanStep<T>())
				{
					std::memcpy(&before, memorySnapshot[i].bytes.data() + j, sizeof(before));
					std::memcpy(&after, now.data() + j, sizeof(after));
					uintptr_t memoryAddr = memorySnapshot[i].start + j;
					if (after > before)
					{
						newMemoryAddrList.push_back(memoryAddr);
						saveMemoryValue(memoryAddr, after);
					}
					
				}
			}
            proc.detatch();
			Scanner::memorySnapshot.clear();
			memoryAddrList = newMemoryAddrList;
			firstScan = false;
			return true;
		}

		std::vector<uintptr_t> newMemoryAddrList;
		Scanner::proc.attatch();
		std::vector<MemorySnapshot> snapshotBefore = Scanner::memorySnapshot;
		for (int i = 0; Scanner::memoryAddrList.size() > i; i++) 
		{
			T beforeValue = getMemoryValueBefore<T>(memoryAddrList[i]);
			T value = readValue<T>(memoryAddrList[i]);
			if (beforeValue < value) 
			{
				newMemoryAddrList.push_back(Scanner::memoryAddrList[i]);
				changeMemoryValueBefore(memoryAddrList[i], value);
			}
		}
		Scanner::proc.detatch();
		memoryAddrList = newMemoryAddrList;
		return true;
	}
	template<typename T>
	bool rescanLower()
	{
		if (firstScan)
		{
			T before;
			T after;
			std::vector<uintptr_t> newMemoryAddrList;
            proc.attatch();
			for(int i = 0; i < memorySnapshot.size(); i++)
			{
				std::vector<uint8_t> now = proc.readMemory(memorySnapshot[i].start, memorySnapshot[i].bytes.size());
				for (size_t j = 0; j + sizeof(T) <= now.size(); j += scanStep<T>())
				{
					std::memcpy(&before, memorySnapshot[i].bytes.data() + j, sizeof(before));
					std::memcpy(&after, now.data() + j, sizeof(after));
					uintptr_t memoryAddr = memorySnapshot[i].start + j;
					if (after < before)
					{
						newMemoryAddrList.push_back(memoryAddr);
						saveMemoryValue(memoryAddr, after);
					}
					
				}
			}
            proc.detatch();
			Scanner::memorySnapshot.clear();
			memoryAddrList = newMemoryAddrList;
			firstScan = false;
			return true;
		}
		std::vector<uintptr_t> newMemoryAddrList;
		Scanner::proc.attatch();
		std::vector<MemorySnapshot> snapshotBefore = Scanner::memorySnapshot;
		for (int i = 0; Scanner::memoryAddrList.size() > i; i++) 
		{
			T beforeValue = getMemoryValueBefore<T>(memoryAddrList[i]);
			T value = readValue<T>(memoryAddrList[i]);
			if (beforeValue > value) 
			{
				newMemoryAddrList.push_back(Scanner::memoryAddrList[i]);
				changeMemoryValueBefore(memoryAddrList[i], value);
			}
		}
		Scanner::proc.detatch();
		memoryAddrList = newMemoryAddrList;
		return true;
	}

	template<typename T>
	bool rescanSame()
	{
		if (firstScan)
		{
			T before;
			T after;
			std::vector<uintptr_t> newMemoryAddrList;
            proc.attatch();
			for(int i = 0; i < memorySnapshot.size(); i++)
			{
				std::vector<uint8_t> now = proc.readMemory(memorySnapshot[i].start, memorySnapshot[i].bytes.size());
				for (size_t j = 0; j + sizeof(T) <= now.size(); j += scanStep<T>())
				{
					std::memcpy(&before, memorySnapshot[i].bytes.data() + j, sizeof(before));
					std::memcpy(&after, now.data() + j, sizeof(after));
					uintptr_t memoryAddr = memorySnapshot[i].start + j;
					if (after == before)
					{
						newMemoryAddrList.push_back(memoryAddr);
						saveMemoryValue(memoryAddr, after);
					}
					
				}
			}
            proc.detatch();
			Scanner::memorySnapshot.clear();
			memoryAddrList = newMemoryAddrList;
			firstScan = false;
			return true;
		}

		std::vector<uintptr_t> newMemoryAddrList;
		Scanner::proc.attatch();
		for (int i = 0; Scanner::memoryAddrList.size() > i; i++) 
		{
			T beforeValue = getMemoryValueBefore<T>(memoryAddrList[i]);
			T value = readValue<T>(memoryAddrList[i]);
			if (beforeValue == value) 
			{
				newMemoryAddrList.push_back(Scanner::memoryAddrList[i]);
				changeMemoryValueBefore(memoryAddrList[i], value);
			}
		}
		Scanner::proc.detatch();
		memoryAddrList = newMemoryAddrList;
		return true;
	}
	template<typename T>
	bool rescanChanged()
	{
		if (firstScan)
		{
			T before;
			T after;
			std::vector<uintptr_t> newMemoryAddrList;
            proc.attatch();
			for(int i = 0; i < memorySnapshot.size(); i++)
			{
				std::vector<uint8_t> now = proc.readMemory(memorySnapshot[i].start, memorySnapshot[i].bytes.size());
				for (size_t j = 0; j + sizeof(T) <= now.size(); j += scanStep<T>())
				{
					std::memcpy(&before, memorySnapshot[i].bytes.data() + j, sizeof(before));
					std::memcpy(&after, now.data() + j, sizeof(after));
					uintptr_t memoryAddr = memorySnapshot[i].start + j;
					if (after != before)
					{
						newMemoryAddrList.push_back(memoryAddr);
						saveMemoryValue(memoryAddr, after);
					}
					
				}
			}
            proc.detatch();
			Scanner::memorySnapshot.clear();
			memoryAddrList = newMemoryAddrList;
			firstScan = false;
			return true;
		}
		std::vector<uintptr_t> newMemoryAddrList;
		Scanner::proc.attatch();
		for (int i = 0; Scanner::memoryAddrList.size() > i; i++) 
		{
			T beforeValue = getMemoryValueBefore<T>(memoryAddrList[i]);
			T value = readValue<T>(memoryAddrList[i]);
			if (beforeValue != value) 
			{
				newMemoryAddrList.push_back(Scanner::memoryAddrList[i]);
				changeMemoryValueBefore(memoryAddrList[i], value);
			}
		}
		Scanner::proc.detatch();
		memoryAddrList = newMemoryAddrList;
		return true;
	}



	template <typename T>
	void write(uintptr_t memoryAddr, T value)
	{
		std::vector<uint8_t> data(sizeof(T));
		std::memcpy(data.data(), &value, sizeof(T));
		Scanner::proc.attatch();
		proc.writeMemory(memoryAddr, data);
		Scanner::proc.detatch();
	}

	template <typename T>
	void saveMemoryValue(uintptr_t memoryAddr, T value)
	{
	    memoryValuesBefore[memoryAddr] = static_cast<int64_t>(value);
	}
	template <typename T>
T getMemoryValueBefore(uintptr_t memoryAddr)
	{
	    return static_cast<T>(memoryValuesBefore[memoryAddr]);
	}
	template <typename T>
	void changeMemoryValueBefore(uintptr_t memoryAddr, T value)
	{
	    memoryValuesBefore[memoryAddr] = static_cast<int64_t>(value);
	}

	template <typename T>
	void printScan(T target)
	{
		T value = target;
		Scanner::proc.attatch();
		for (int i = 0; Scanner::memoryAddrList.size() > i; i++) 
		{
			std::vector<uint8_t> value = Scanner::proc.readMemory(Scanner::memoryAddrList[i], sizeof(target));
			std::memcpy(&target,value.data(), sizeof(target));
			std::print("\n [{}] {}		{}",i,Scanner::memoryAddrList[i],target);
		}
		Scanner::proc.detatch();

	}
	template <typename T>
	T getMemoryValue(uintptr_t memoryAddr)
	{
		T target;
		Scanner::proc.attatch();
		std::vector<uint8_t> value = Scanner::proc.readMemory(memoryAddr, sizeof(T));
		Scanner::proc.detatch();
		std::memcpy(&target,value.data(),sizeof(target));
		return target;
	}
	template <typename T>
	T readValue(uintptr_t memoryAddr)
	{
		T target;
		std::vector<uint8_t> value = Scanner::proc.readMemory(memoryAddr, sizeof(T));
		std::memcpy(&target,value.data(),sizeof(target));
		return target;
	}

	bool newScan();
	std::vector<uintptr_t> getMemoryAddrList();
    std::vector<std::string> getProcessNames();
    void processConnect(std::string procName);
    void updateProcessNames();
    bool isStatic(uintptr_t memoryAddr);
    std::vector<uintptr_t> findPointersTo(uintptr_t memoryAddr);
    std::string findPath(uintptr_t memoryAddr);
    uintptr_t getBaseFromPath(std::string path);
    std::vector<MemorySnapshot> getMemorySnapshot();
    bool isValidAddress(uintptr_t memoryAddr);
    void refreshMaps() { proc.parceMaps(); }
    void reset();

};




