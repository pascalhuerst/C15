#include "Ribbon.h"
#include <glib.h>
#include <glib.h>
#include <typeinfo>
#include <math.h>
#include "device-settings/DebugLevel.h"
#include <thread>

Ribbon::Ribbon()
{
}

Ribbon::~Ribbon()
{
  resetLEDs();
}

void Ribbon::syncBBBB()
{
  for(int i = 0; i < NUM_LEDS_PER_RIBBON; i++)
  {
    m_leds[i].syncBBBB();
  }
}

void Ribbon::initLEDs()
{
  for(int i = 0; i < NUM_LEDS_PER_RIBBON; i++)
  {
    int id = posToLedID(i);
    m_leds[i].setID(id);
  }

  resetLEDs();
}

void Ribbon::setLEDState(int ledPos, char state)
{
  m_leds[ledPos].setState(state);
  // debugTrace();
}

void Ribbon::debugTrace()
{
#if _DEVELOPMENT_PC && 1
  Glib::ustring dbg;

  for(int i = 0; i < NUM_LEDS_PER_RIBBON; i++)
  {
    char s = m_leds[i].getState ();
    if(s == 0)
    dbg += '-';
    else if(s == 1)
    dbg += '.';
    else if(s == 2)
    dbg += 'o';
    else if(s == 3)
    dbg += 'O';
  }

  DebugLevel::warning("Ribbon:", typeid(*this).name(), dbg.c_str());
#endif
}

void Ribbon::resetLEDs()
{
  for(int i = 0; i < NUM_LEDS_PER_RIBBON; i++)
    setLEDState(i, 0);

  // debugTrace();
}

void Ribbon::setLEDsForValueUniPolar(tDisplayValue paramVal)
{
  int numRepresentableStates = NUM_LEDS_PER_RIBBON * 3 + 1;
  int paramValIdx = lround(paramVal * numRepresentableStates);
  setLEDsUniPolar(paramValIdx);
}

void Ribbon::setLEDsUniPolar(int paramValIdx)
{
  int incomingParamVal = paramValIdx;

  for(int i = 0; i < NUM_LEDS_PER_RIBBON; i++)
  {
    int brightness = 0;

    for(int b = 0; b < 3; b++)
    {
      paramValIdx -= 1;

      if(paramValIdx > 0)
        brightness++;
    }

    if(i == 0 && incomingParamVal > 0)
      brightness = std::max(brightness, 1);

    setLEDState(i, brightness);
  }
}

char Ribbon::handleCenter(tDisplayValue v) const
{
  if(v == 0.0)
    return 0;

  return 3;
}

char Ribbon::getLEDStateForBipolarValue(int led, tDisplayValue v) const
{
  constexpr auto center = NUM_LEDS_PER_RIBBON / 2;

  if(led == center)
    return handleCenter(v);

  bool ledIsOnLeftHalf = led < center;
  bool valueIsOnLeftHalf = v < 0.0;

  if(ledIsOnLeftHalf != valueIsOnLeftHalf)
    return 0;

  constexpr auto valueStepPerLed = 1.0 / center;
  constexpr auto valueStepPerLedBrightnessStep = valueStepPerLed / 3;
  const auto relativeAbsoluteDistanceToMiddle = std::abs(led - center) / (double) center;
  const auto absoluteValue = std::abs(v);

  if(relativeAbsoluteDistanceToMiddle <= absoluteValue)
    return 3;

  if(relativeAbsoluteDistanceToMiddle <= absoluteValue + valueStepPerLedBrightnessStep)
    return 2;

  if(relativeAbsoluteDistanceToMiddle <= absoluteValue + 2 * valueStepPerLedBrightnessStep)
    return 1;

  return 0;
}

void Ribbon::setLEDsForValueBiPolar(tDisplayValue paramValue)
{
  for(int i = 0; i < NUM_LEDS_PER_RIBBON; i++)
  {
    setLEDState(i, getLEDStateForBipolarValue(i, paramValue));
  }
}
