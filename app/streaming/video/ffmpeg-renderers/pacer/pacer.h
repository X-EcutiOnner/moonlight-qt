#pragma once

#include "../../decoder.h"
#include "../renderer.h"

#include <QQueue>

class IVsyncSource {
public:
    virtual ~IVsyncSource() {}
    virtual bool initialize(SDL_Window* window, int displayFps) = 0;
};

class Pacer
{
public:
    Pacer(IFFmpegRenderer* renderer, PVIDEO_STATS videoStats);

    ~Pacer();

    void submitFrame(AVFrame* frame);

    bool initialize(SDL_Window* window, int maxVideoFps, bool enablePacing);

    void vsyncCallback(int timeUntilNextVsyncMillis);

    bool isUsingFrameQueue();

private:
    void addRenderTimeToHistory(int renderTime);

    void renderFrame(AVFrame* frame);

    QQueue<AVFrame*> m_FrameQueue;
    QQueue<int> m_FrameQueueHistory;
    QQueue<int> m_RenderTimeHistory;
    SDL_SpinLock m_FrameQueueLock;

    IVsyncSource* m_VsyncSource;
    IFFmpegRenderer* m_VsyncRenderer;
    int m_MaxVideoFps;
    int m_DisplayFps;
    PVIDEO_STATS m_VideoStats;
};
