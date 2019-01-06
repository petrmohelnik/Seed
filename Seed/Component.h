#pragma once

class Object;

class Component
{
public:
	virtual ~Component() = default;

	std::shared_ptr<Object> GetObject();

protected:
    friend class Object;
	void SetObject(std::weak_ptr<Object> parentObject);

private:
	std::weak_ptr<Object> object;
};
