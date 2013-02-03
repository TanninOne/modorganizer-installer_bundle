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
  return VersionInfo(1, 0, 0, VersionInfo::RELEASE_FINAL);
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

bool InstallerBundle::isArchiveSupported(const DirectoryTree &tree) const
{
  for (DirectoryTree::const_leaf_iterator fileIter = tree.leafsBegin();
       fileIter != tree.leafsEnd(); ++fileIter) {
    if (fileIter->getName().endsWith(".fomod", Qt::CaseInsensitive)) {
      return true;
    }
  }
  return false;
}

IPluginInstaller::EInstallResult InstallerBundle::install(QString &modName, DirectoryTree &tree)
{
  for (DirectoryTree::const_leaf_iterator fileIter = tree.leafsBegin();
       fileIter != tree.leafsEnd(); ++fileIter) {
    if (fileIter->getName().endsWith(".fomod", Qt::CaseInsensitive)) {
      QString tempFile = manager()->extractFile(fileIter->getName());
      return manager()->installArchive(modName, tempFile);
    }
  }
  return IPluginInstaller::RESULT_NOTATTEMPTED;
}


Q_EXPORT_PLUGIN2(installerBundle, InstallerBundle)
