#pragma once

#include <memory>

#include <utilities/time.hpp>

class Entity;

namespace google
{
namespace protobuf
{
class Message;
}
}

using Message = google::protobuf::Message;

class EntityComponent
{
	friend class EntityFactory;

public:
	EntityComponent() = default;
	virtual ~EntityComponent() = default;

	virtual bool initialize(const std::shared_ptr<Message>& data) = 0;

	virtual void postInitialize() {}
	virtual void update(const Milliseconds /*delta*/) {}

	virtual const char* name() const = 0;

protected:
	std::shared_ptr<Entity> _owner;

private:
	void setOwner(std::shared_ptr<Entity> owner) { this->_owner = owner; }
};
