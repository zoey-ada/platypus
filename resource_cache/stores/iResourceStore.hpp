#pragma once

#include <string>

class IResourceStore
{
public:
	virtual ~IResourceStore() = default;

	[[nodiscard]] virtual std::string identifier() const = 0;
	[[nodiscard]] virtual bool open() = 0;
	[[nodiscard]] virtual uint64_t getResourceSize(const std::string& name) const = 0;
	[[nodiscard]] virtual uint8_t* getResource(const std::string& name) = 0;
	[[nodiscard]] virtual bool getResource(const std::string& name, uint8_t* buffer) = 0;
	[[nodiscard]] virtual uint64_t getResourceCount() const = 0;
	[[nodiscard]] virtual int64_t getResourceIndex(const std::string& name) const = 0;
	[[nodiscard]] virtual std::string getResourceName(const int64_t index) const = 0;
};
