/*
Copyright (C) 2012 Sebastian Herbord. All rights reserved.

This file is part of Mod Organizer.

Mod Organizer is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Mod Organizer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Mod Organizer.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "installerbundle.h"
#include "iinstallationmanager.h"

#include <QtPlugin>

using namespace MOBase;


InstallerBundle::InstallerBundle()
{
}


bool InstallerBundle::init(IOrganizer*)
{
  return true;
}

QString InstallerBundle::name() const
{
  return "Bundle Installer";
}

QString InstallerBundle::author() const
{
  return "Tannin";
}

QString InstallerBundle::description() const
{
  return tr("Proxy-Installer to handle bundles containing the actual mod archive");
}

VersionInfo InstallerBundle::version() const
{
  return VersionInfo(1, 1, 0, VersionInfo::RELEASE_FINAL);
}

bool InstallerBundle::isActive() const
{
  return true;
}

QList<PluginSetting> InstallerBundle::settings() const
{
  return QList<PluginSetting>();
}

unsigned int InstallerBundle::priority() const
{
  return 10;
}

bool InstallerBundle::isManualInstaller() const
{
  return false;
}

std::shared_ptr<const FileTreeEntry> InstallerBundle::findObject(std::shared_ptr<const IFileTree> tree) const
{
  // Check if we have an archive:
  // (Really truly we should use the supported extensions from the installation
  // manager here.)
  std::set<QString, FileNameComparator> supportedExtensions{ "7z", "zip", "rar" };
  if (tree->size() == 1 && tree->at(0)->isFile()) {
    auto file = tree->at(0);
    if (supportedExtensions.count(file->suffix()) > 0) {
      return file;
    }
  }

  // Look for a .fomod file:
  int nDirs = 0;
  for (auto entry : *tree) {
    if (entry->isFile()) {
      if (entry->compare(".fomod") == 0) {
        return entry;
      }
    }
    else {
      nDirs++;
    }
  }

  // Arguably this should also check the name of the directory we're looking at
  // is the same as the module name:
  if (nDirs != 1) {
    return nullptr;
  }

  // Retrieve the first folder which is the first item:
  return findObject(tree->at(0)->astree());
}

bool InstallerBundle::isArchiveSupported(std::shared_ptr<const IFileTree> tree) const
{
  return findObject(tree) != nullptr;
}

IPluginInstaller::EInstallResult InstallerBundle::install(
  GuessedValue<QString>& modName, std::shared_ptr<IFileTree>& tree, QString&, int &modId)
{
  // Find a valid "object":
  auto entry = findObject(tree);
  if (entry == nullptr) {
    return IPluginInstaller::RESULT_NOTATTEMPTED;
  }

  // Extract it:
  QString tempFile = manager()->extractFile(entry);
  IPluginInstaller::EInstallResult res = manager()->installArchive(modName, tempFile, modId);
  if (res == IPluginInstaller::RESULT_SUCCESS) {
    res = IPluginInstaller::RESULT_SUCCESSCANCEL;
  }
  return res;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(installerBundle, InstallerBundle)
#endif
