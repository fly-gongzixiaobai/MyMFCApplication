
#ifndef __YcBezierSpline_H__
#define __YcBezierSpline_H__

// INCLUDES -----------------------------------------------------------------------------

#include "YicSpline.h"

// --------------------------------------------------------------------------------------

#define YD_MAX_BEZIER_CONTROL_VALUE 33

// --------------------------------------------------------------------------------------

class YcBezierSpline : public YicSpline
{
public:
    YcBezierSpline();

    ~YcBezierSpline();

    // 设置输出样条值的数目
    void    SetSplineValuesCount(Yuint count);

    // 获得输出样条值的数目
    Yuint   GetSplineValuesCount() const;

    // 计算样条数值
    bool    BuildSpline(const void* ctrlValuesPtr, Yuint ctrlStride, Yuint ctrlCount,
        void* splineValuesPtr, Yuint splineStride) const;

protected:
    void    ClearPowT();

    void    BuildPowT();

    Yreal    GetValueT(Yint t, Yint p) const
    {
        return m_pow_t[YD_MAX_BEZIER_CONTROL_VALUE * t + p];
    }

protected:
    Yuint   m_valuesCount;
    Yreal* m_pow_t;

protected:
    static void    BuildYanghuiTriangle();
    static Yint m_yanghuiRowIndex[YD_MAX_BEZIER_CONTROL_VALUE];
    static Yint m_yanghuiTriangle[(YD_MAX_BEZIER_CONTROL_VALUE + 1) * YD_MAX_BEZIER_CONTROL_VALUE / 2];
};

// --------------------------------------------------------------------------------------

#endif