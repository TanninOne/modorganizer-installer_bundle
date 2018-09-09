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
  return VersionInfo(1, 0, 1, VersionInfo::RELEASE_FINAL);
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

bool InstallerBundle::findObject(DirectoryTree const *tree) const
{
  for (;;) {
    DirectoryTree::const_leaf_iterator fileIter = tree->leafsBegin();

    //Really truly we should use the supported extensions from the installation
    //manager here
    if (tree->numNodes() == 0 && tree->numLeafs() == 1 &&
        (fileIter->getName().endsWith(".7z") ||
         fileIter->getName().endsWith(".zip") ||
         fileIter->getName().endsWith(".rar"))) {
      // nested archive
      m_found_dir = tree;
      m_found_leaf = fileIter;
      return true;
    }

    for (; fileIter != tree->leafsEnd(); ++fileIter) {
      if (fileIter->getName().endsWith(".fomod")) {
        m_found_dir = tree;
        m_found_leaf = fileIter;
        return true;
      }
    }

    //Arguably this should also check the name of the directory we're looking at
    //is the same as the module name
    if (tree->numNodes() != 1) {
      return false;
    }
    tree = *(tree->nodesBegin());
  }
}

bool InstallerBundle::isArchiveSupported(const DirectoryTree &tree) const
{
  return findObject(&tree);
}

IPluginInstaller::EInstallResult InstallerBundle::install(GuessedValue<QString> &modName,
                                                          DirectoryTree &tree,
                                                          QString&, int &modId)
{
  if (! findObject(&tree)) {
    return IPluginInstaller::RESULT_NOTATTEMPTED;
  }
  FileNameString name = m_found_dir->getFullPath(&(*m_found_leaf));
  QString tempFile = manager()->extractFile(name.toQString());
  IPluginInstaller::EInstallResult res = manager()->installArchive(modName, tempFile, modId);
  if (res == IPluginInstaller::RESULT_SUCCESS) {
    res = IPluginInstaller::RESULT_SUCCESSCANCEL;
  }
  return res;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(installerBundle, InstallerBundle)
#endif
