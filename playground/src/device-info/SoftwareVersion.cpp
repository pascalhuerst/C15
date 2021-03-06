#include "SoftwareVersion.h"
#include <xml/Writer.h>
#include <xml/Attribute.h>
#include <device-settings/DebugLevel.h>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

SoftwareVersion::SoftwareVersion (DeviceInformation *parent) :
    DeviceInformationItem (parent),
    m_versionString (TOSTRING(LAST_COMMIT_TIME))
{
}

void SoftwareVersion::writeDocument (Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  writer.writeTextElement ("software-version", m_versionString);
}

Glib::ustring SoftwareVersion::get () const
{
  return m_versionString;
}
