#ifndef FPSCHRONO_H
#define FPSCHRONO_H

#include "Bang/List.h"
#include "Bang/ChronoGL.h"

NAMESPACE_BANG_BEGIN

class FPSChrono
{
public:
    FPSChrono() = default;
    ~FPSChrono() = default;

    void MarkBegin();
    void MarkEnd();

    void SetMeanSamples(int meanSamples);

    double GetMeanFPS() const;
    double GetMeanSeconds() const;
    int GetMeanSamples() const;

private:
    List<double> m_latestDeltaTimes;
    double m_beginTimeSeconds = 0.0;
    int m_meanSamples = 30;
};

NAMESPACE_BANG_END

#endif // FPSCHRONO_H

