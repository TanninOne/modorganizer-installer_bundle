cmake_minimum_required(VERSION 3.16)

add_library(installer_bundle SHARED)
mo2_configure_plugin(installer_bundle WARNINGS OFF)
mo2_install_target(installer_bundle)
