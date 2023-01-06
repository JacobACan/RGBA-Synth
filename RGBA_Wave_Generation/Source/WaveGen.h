/*
  ==============================================================================

    WaveGen.h
    Created: 6 Jan 2023 10:48:58am
    Author:  13308

  ==============================================================================
*/

#pragma once

class WaveGen
{
public:
    /** Returns the square wave sample level for a corresponding angle*/
    static double sqr(double angle);

    /** Returns the sawtooth wave sample level for a corresponding angle*/
    static double swt(double angle);

    /** Returns the saw wave sample level for a corresponding angle*/
    static double saw(double angle);
};