#include "Bang/FPSChrono.h"

#include "Bang/Math.h"
#include "Bang/Time.h"
#include "Bang/Debug.h"

USING_NAMESPACE_BANG

void FPSChrono::MarkBegin()
{
    m_beginTimeSeconds = Time::GetNow_Seconds();
}

void FPSChrono::MarkEnd()
{
    while (m_latestDeltaTimes.Size() > GetMeanSamples() - 1)
    {
        m_latestDeltaTimes.PopBack();
    }

    m_latestDeltaTimes.PushFront( Time::GetNow_Seconds() - m_beginTimeSeconds );
}

void FPSChrono::SetMeanSamples(int meanSamples)
{
    m_meanSamples = Math::Max(meanSamples, 1);
}

double FPSChrono::GetMeanFPS() const
{
    if (m_latestDeltaTimes.IsEmpty()) { return 0.0; }

    return (1.0 / Math::Max(0.00001, GetMeanSeconds()));
}

double FPSChrono::GetMeanSeconds() const
{
    if (m_latestDeltaTimes.IsEmpty()) { return 0.0; }

    float meanDeltas = 0.0;
    for (float delta : m_latestDeltaTimes) { meanDeltas += delta; }
    meanDeltas /= m_latestDeltaTimes.Size();
    return meanDeltas;
}

int FPSChrono::GetMeanSamples() const
{
    return m_meanSamples;
}
