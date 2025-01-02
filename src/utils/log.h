/*
 *  Copyright (C) 2022 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#ifndef INPUTSTREAM_TEST_BUILD
#include <kodi/AddonBase.h>
#else
#include "kodi/tools/StringUtils.h"
#include <iostream>
#endif

#include <format>
#include <utility>

// To keep in sync with SSDLogLevel on SSD_dll.h
enum LogLevel
{
  LOGDEBUG,
  LOGINFO,
  LOGWARNING,
  LOGERROR,
  LOGFATAL
};

namespace LOG
{

template<typename... Args>
inline void Log(const LogLevel level, std::format_string<Args...> format, Args&&... args)
{
  char buffer[16 * 1024]; // 16k is the maximum kodi logs

  const std::format_to_n_result result =
      std::format_to_n(std::begin(buffer), sizeof(buffer) - 1, format, std::forward<Args>(args)...);
  *result.out = '\0';

#ifndef INPUTSTREAM_TEST_BUILD
  ADDON_LOG addonLevel;

  switch (level)
  {
    case LogLevel::LOGFATAL:
      addonLevel = ADDON_LOG::ADDON_LOG_FATAL;
      break;
    case LogLevel::LOGERROR:
      addonLevel = ADDON_LOG::ADDON_LOG_ERROR;
      break;
    case LogLevel::LOGWARNING:
      addonLevel = ADDON_LOG::ADDON_LOG_WARNING;
      break;
    case LogLevel::LOGINFO:
      addonLevel = ADDON_LOG::ADDON_LOG_INFO;
      break;
    default:
      addonLevel = ADDON_LOG::ADDON_LOG_DEBUG;
  }

  kodi::Log(addonLevel, "%s", buffer);
#else
  switch (level)
  {
  case LogLevel::LOGFATAL:
    std::cout << "[ LOG-FATAL ] " << buffer << std::endl;
    break;
  case LogLevel::LOGERROR:
    std::cout << "[ LOG-ERROR ] " << buffer << std::endl;
    break;
    /*
  case LogLevel::LOGWARNING:
    std::cout << "[ LOG-WARN  ] " << buffer << std::endl;
    break;
  case LogLevel::LOGINFO:
    std::cout << "[ LOG-INFO  ] " << buffer << std::endl;
    break;
  case LogLevel::LOGDEBUG:
    std::cout << "[ LOG-DEBUG ] " << buffer << std::endl;
*/
  default:
    break;
  }
#endif
}

#define LogF(level, format, ...) Log((level), ("{}: " format), __FUNCTION__, ##__VA_ARGS__)

} // namespace LOG
