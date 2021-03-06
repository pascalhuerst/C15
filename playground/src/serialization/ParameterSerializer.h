#pragma once

#include "Serializer.h"

class Parameter;

class ParameterSerializer : public Serializer
{
  public:
    ParameterSerializer (Parameter *param);

    static Glib::ustring getTagName ();

  private:
    void writeTagContent (Writer &writer) const;
    void readTagContent (Reader &reader) const;

    void tryLoadModulateableParameter (Reader& reader) const;
    void tryLoadMacroControlParameter (Reader& reader) const;
    void tryLoadHardwareSourceParameter (Reader &reader) const;
    void loadModulationAmount (Reader& reader, const Glib::ustring& text) const;

    void tryWriteModulateableParameter (Writer& writer) const;
    void tryWriteMacroControlParameter (Writer& writer) const;
    void tryWriteHardwareSourceParameter (Writer &writer) const;

    Parameter *m_param = nullptr;
};
