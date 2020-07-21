#pragma once
#include "parkassist/gles/Color.h"

namespace nb {

class NB_API Brush
{
public:

protected:
	Brush() = default;
	virtual ~Brush() = default;
};

class NB_API SolidColorBrush : public Brush
{
public:
	SolidColorBrush(const Color &color);

	void setColor(const Color &color);
	const Color &color() const;

private:
	Color m_color;
};

class NB_API GradientStop
{
public:
	Color color;
	float offset;
};

class NB_API GradientBrush : public Brush
{
public:
	GradientBrush(const std::vector<GradientStop> &gradientStops);

	const std::vector<GradientStop> &gradientStops() const;

private:
	std::vector<GradientStop> m_gradientStops;
};

class NB_API LinearGradientBrush : public GradientBrush
{
public:
	LinearGradientBrush(const std::vector<GradientStop> &gradientStops);

};

}