#include "RenderQueue.h"
#include "Renderer.h"

RenderQueue::RenderQueue()
{
}

void RenderQueue::Add(Renderer* renderer, int index)
{
    queue.push_back({renderer, index});
}
