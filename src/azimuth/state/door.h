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

#pragma once
#ifndef AZIMUTH_STATE_DOOR_H_
#define AZIMUTH_STATE_DOOR_H_

#include <stdbool.h>

#include "azimuth/state/player.h" // for az_damage_flags_t and az_room_key_t
#include "azimuth/util/vector.h"

/*===========================================================================*/

// The number of different door kinds there are, not counting AZ_DOOR_NOTHING:
#define AZ_NUM_DOOR_KINDS 7

typedef enum {
  AZ_DOOR_NOTHING = 0,
  AZ_DOOR_NORMAL, // openable by any weapon
  AZ_DOOR_LOCKED, // cannot be opened
  AZ_DOOR_ROCKET, // openable by a rocket of any kind
  AZ_DOOR_HYPER_ROCKET, // openable only by a hyper rocket
  AZ_DOOR_BOMB, // openable by a bomb of any kind
  AZ_DOOR_MEGA_BOMB, // openable only by a mega bomb
  AZ_DOOR_PASSAGE // no door per se; just a passage that leads to another room
} az_door_kind_t;

// Determine whether damage of the given kind will open a door of the given
// kind.  The door kind must not be AZ_DOOR_NOTHING.
bool az_can_open_door(az_door_kind_t door_kind, az_damage_flags_t damage_kind);

/*===========================================================================*/

typedef struct {
  az_door_kind_t kind; // if AZ_DOOR_NOTHING, this door is not present
  az_vector_t position;
  double angle;
  az_room_key_t destination;
  bool is_open; // is the door open/opening, or is it closed/closing
  double openness; // 0.0 = fully closed, 1.0 = fully open
} az_door_t;

extern const double AZ_DOOR_BOUNDING_RADIUS;

// Determine if a ray, travelling delta from start, will hit the exterior of
// the door (this is possible only for closed doors).  If it does, stores the
// intersection point in *point_out (if point_out is non-NULL) and the normal
// vector in *normal_out (if normal_out is non-NULL).
bool az_ray_hits_door_outside(
    const az_door_t *door, az_vector_t start, az_vector_t delta,
    az_vector_t *point_out, az_vector_t *normal_out);

// Determine if a circle with the given radius, travelling delta from start,
// will hit the exterior of the door (this is possible only for closed doors).
// If it does, the function stores in *pos_out the earliest position of the
// circle at which it touches the door (if pos_out is non-NULL) and in
// *impact_out the point of intersection (if impact_out is non-NULL).
bool az_circle_hits_door_outside(
    const az_door_t *door, double radius, az_vector_t start, az_vector_t delta,
    az_vector_t *pos_out, az_vector_t *impact_out);

// Determine if a ray, travelling delta from start, will hit the interior of
// the door (this is possible only for open doors).  If it does, stores the
// intersection point in *point_out (if point_out is non-NULL) and the normal
// vector in *normal_out (if normal_out is non-NULL).
bool az_ray_hits_door_inside(
    const az_door_t *door, az_vector_t start, az_vector_t delta,
    az_vector_t *point_out, az_vector_t *normal_out);

// Determine if a circle with the given radius, travelling delta from start,
// will hit the interior of the door (this is possible only for open doors).
// If it does, the function stores in *pos_out the earliest position of the
// circle at which it touches the door (if pos_out is non-NULL) and in
// *impact_out the point of intersection (if impact_out is non-NULL).
bool az_circle_hits_door_inside(
    const az_door_t *door, double radius, az_vector_t start, az_vector_t delta,
    az_vector_t *pos_out, az_vector_t *impact_out);

/*===========================================================================*/

#endif // AZIMUTH_STATE_DOOR_H_