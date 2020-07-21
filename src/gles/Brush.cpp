#include "parkassist/gles/Brush.h"

using namespace nb;

SolidColorBrush::SolidColorBrush(const Color & color)
	: m_color(color)
{
}

void SolidColorBrush::setColor(const Color & color)
{
	m_color = color;
}

const Color & SolidColorBrush::color() const
{
	return m_color;
}

GradientBrush::GradientBrush(const std::vector<GradientStop>& gradientStops)
	: m_gradientStops(gradientStops)
{
}

const std::vector<GradientStop>& GradientBrush::gradientStops() const
{
	return m_gradientStops;
}

LinearGradientBrush::LinearGradientBrush(const std::vector<GradientStop>& gradientStops)
	: GradientBrush(gradientStops)
{
}
