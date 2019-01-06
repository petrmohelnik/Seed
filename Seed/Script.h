#pragma once

class Script
{
public:
	virtual ~Script() = default;
	virtual void Start() {};
	virtual void OnFrameUpdate() {};
	virtual void End() {};
};