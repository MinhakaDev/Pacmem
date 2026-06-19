#pragma once
#include "./process.h"
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
	Scanner();
	

	template <typename T>
	bool scanExact(T target)
	{
		T tempValue = target;
		if (!Scanner::newScan()) 
		{
			std::println("error 1");
			return false;
		}
		std::vector<MemorySnapshot> snapshotBefore = Scanner::memorySnapshot;
		
		for (int i = 0; snapshotBefore.size() > i ; i++)
		{
			for (int j = 0; snapshotBefore[i].bytes.size() > j; j++) 
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
		T tempValue;
		if (!Scanner::newScan()) 
		{
			std::println("error 1");
		}
		const std::vector<MemorySnapshot>& snapshotBefore = Scanner::memorySnapshot;
		firstScan = true;
		
	}
	template <typename T>
		bool rescanExact(T target)
		{
			T tempValue = target;
			std::vector<uintptr_t> newMemoryAddrList;
			Scanner::proc.attatch();
		for (int i = 0; Scanner::memoryAddrList.size() > i; i++) 
		{
			T value = readValue<T>(memoryAddrList[i]);
			if (tempValue == value) 
			{
				newMemoryAddrList.push_back(Scanner::memoryAddrList[i]);
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
			std::vector<MemorySnapshot> snapshotBefore = Scanner::memorySnapshot;
			newScan();
			std::vector<MemorySnapshot>& newSnapshot = Scanner::memorySnapshot;
			T before;
			T after;
			std::vector<uintptr_t> newMemoryAddrList;
			for(int i = 0; i < snapshotBefore.size(); i++)
			{
				std::println("i = {} de {}",i,snapshotBefore.size());
				for (int j = 0; j < snapshotBefore[i].bytes.size(); j++ )
				{
					if (snapshotBefore[i].start == newSnapshot[i].start)
					{
						std::memcpy(&before, snapshotBefore[i].bytes.data() + j, sizeof(before));
						std::memcpy(&after, newSnapshot[i].bytes.data() + j, sizeof(after));
						uintptr_t memoryAddr = newSnapshot[i].start + j;
						if (after > before)
						{
							newMemoryAddrList.push_back(memoryAddr);
							saveMemoryValue(memoryAddr, after);
						}
					}
				}
			}
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
			std::vector<MemorySnapshot> snapshotBefore = Scanner::memorySnapshot;
			newScan();
			std::vector<MemorySnapshot>& newSnapshot = Scanner::memorySnapshot;
			T before;
			T after;
			std::vector<uintptr_t> newMemoryAddrList;
			for(int i = 0; i < snapshotBefore.size(); i++)
			{
				std::println("i = {} de {}",i,snapshotBefore.size());
				for (int j = 0; j < snapshotBefore[i].bytes.size(); j++ )
				{
					if (snapshotBefore[i].start == newSnapshot[i].start)
					{
						std::memcpy(&before, snapshotBefore[i].bytes.data() + j, sizeof(before));
						std::memcpy(&after, newSnapshot[i].bytes.data() + j, sizeof(after));
						uintptr_t memoryAddr = newSnapshot[i].start + j;
						if (after < before)
						{
							newMemoryAddrList.push_back(memoryAddr);
							saveMemoryValue(memoryAddr, after);
						}
					}
				}
			}
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
	void write(int index, T value)
	{
		std::vector<uint8_t> data(sizeof(T));
		std::memcpy(data.data(), &value, sizeof(T));
		Scanner::proc.attatch();
		proc.writeMemory(Scanner::memoryAddrList[index], data);
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
};





