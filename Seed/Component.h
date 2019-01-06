#pragma once
class Object;

class Component
{
public:
	Component();
	virtual ~Component() = default;

	void SetObject(std::weak_ptr<Object> parentObject);
	std::shared_ptr<Object> GetObject();

private:
	std::weak_ptr<Object> object;
};
