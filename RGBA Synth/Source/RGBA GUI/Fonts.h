/*
  ==============================================================================

	Fonts.cpp
	Created: 31 Jul 2023 12:30:48pm
	Author:  jcane

  ==============================================================================
*/
#pragma once

#include <JuceHeader.h>

namespace RGBAFonts
{
	static const juce::Typeface::Ptr parkLane{ juce::Typeface::createSystemTypefaceFor(BinaryData::Park_Lane_NF_ttf, BinaryData::Park_Lane_NF_ttfSize) };
	static const juce::Typeface::Ptr dymaxion{ juce::Typeface::createSystemTypefaceFor(BinaryData::DYMAXION_TTF, BinaryData::DYMAXION_TTFSize) };
	static const juce::Typeface::Ptr retro{ juce::Typeface::createSystemTypefaceFor(BinaryData::retro_ttf, BinaryData::retro_ttfSize) };
}
