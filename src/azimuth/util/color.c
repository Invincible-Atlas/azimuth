/*=============================================================================
| Copyright 2012 Matthew D. Steele <mdsteele@alum.mit.edu>                    |
|                                                                             |
| This file is part of Azimuth.                                               |
|                                                                             |
| Azimuth is free software: you can redistribute it and/or modify it under    |
| the terms of the GNU General Public License as published by the Free        |
| Software Foundation, either version 3 of the License, or (at your option)   |
| any later version.                                                          |
|                                                                             |
| Azimuth is distributed in the hope that it will be useful, but WITHOUT      |
| ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       |
| FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for   |
| more details.                                                               |
|                                                                             |
| You should have received a copy of the GNU General Public License along     |
| with Azimuth.  If not, see <http://www.gnu.org/licenses/>.                  |
=============================================================================*/

#include "azimuth/util/color.h"

#include "azimuth/util/misc.h"

/*===========================================================================*/

AZ_STATIC_ASSERT(sizeof(az_color_t) == sizeof(int32_t));

const az_color_t AZ_WHITE = {255, 255, 255, 255};
const az_color_t AZ_GRAY = {128, 128, 128, 255};
const az_color_t AZ_RED = {255, 0, 0, 255};
const az_color_t AZ_GREEN = {0, 255, 0, 255};
const az_color_t AZ_BLUE = {0, 0, 255, 255};
const az_color_t AZ_MAGENTA = {255, 0, 255, 255};
const az_color_t AZ_YELLOW = {255, 255, 0, 255};
const az_color_t AZ_CYAN = {0, 255, 255, 255};

/*===========================================================================*/
