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

#include "azimuth/tick/baddie_oth.h"

#include <assert.h>
#include <math.h>

#include "azimuth/state/baddie.h"
#include "azimuth/state/sound.h"
#include "azimuth/state/space.h"
#include "azimuth/tick/baddie_util.h"
#include "azimuth/util/random.h"
#include "azimuth/util/vector.h"

/*===========================================================================*/

void az_tick_bad_oth_crab_1(
    az_space_state_t *state, az_baddie_t *baddie, double time) {
  assert(baddie->kind == AZ_BAD_OTH_CRAB_1);
  az_fly_towards_ship(state, baddie, time,
                      2.0, 40.0, 100.0, 20.0, 100.0, 100.0);
  if (baddie->cooldown <= 0.0 &&
      az_ship_within_angle(state, baddie, 0, AZ_DEG2RAD(6)) &&
      az_can_see_ship(state, baddie)) {
    if (az_fire_baddie_projectile(state, baddie, AZ_PROJ_OTH_ROCKET,
                                  15.0, 0.0, 0.0) != NULL) {
      az_play_sound(&state->soundboard, AZ_SND_FIRE_OTH_ROCKET);
      baddie->cooldown = 2.0;
    }
  }
}

void az_tick_bad_oth_crab_2(
    az_space_state_t *state, az_baddie_t *baddie, double time) {
  assert(baddie->kind == AZ_BAD_OTH_CRAB_2);
  az_fly_towards_ship(state, baddie, time,
                      2.0, 100.0, 200.0, 50.0, 100.0, 100.0);
  baddie->param = fmax(0.0, baddie->param - time);
  if (baddie->cooldown <= 0.0 &&
      az_ship_within_angle(state, baddie, 0, AZ_DEG2RAD(6))) {
    if (az_fire_baddie_projectile(state, baddie, AZ_PROJ_OTH_ROCKET,
                                  15.0, 0.0, 0.0) != NULL) {
      az_play_sound(&state->soundboard, AZ_SND_FIRE_OTH_ROCKET);
      baddie->cooldown = 1.5;
    }
  }
  if (baddie->param <= 0.0 &&
      az_ship_within_angle(state, baddie, 0, AZ_DEG2RAD(8)) &&
      az_can_see_ship(state, baddie)) {
    for (int i = -1; i <= 1; i += 2) {
      az_fire_baddie_projectile(state, baddie, AZ_PROJ_OTH_SPRAY,
                                15.0, AZ_DEG2RAD(14 * i), 0.0);
    }
    az_play_sound(&state->soundboard, AZ_SND_FIRE_OTH_SPRAY);
    baddie->param = 0.9;
  }
}

void az_tick_bad_oth_crawler(
    az_space_state_t *state, az_baddie_t *baddie, double time) {
  assert(baddie->kind == AZ_BAD_OTH_CRAWLER);
  az_crawl_around(state, baddie, time, true, 3.0, 40.0, 100.0);
  if (baddie->cooldown <= 0.0 && az_can_see_ship(state, baddie)) {
    az_vector_t rel_impact;
    if (az_lead_target(az_vsub(state->ship.position, baddie->position),
                       state->ship.velocity, 1000.0, &rel_impact)) {
      az_fire_baddie_projectile(
          state, baddie, AZ_PROJ_OTH_MINIROCKET,
          0.0, az_vtheta(rel_impact) - baddie->angle, 0.0);
      az_play_sound(&state->soundboard, AZ_SND_FIRE_OTH_ROCKET);
      baddie->cooldown = az_random(1.0, 2.0);
    }
  }
}

void az_tick_bad_oth_orb_1(
    az_space_state_t *state, az_baddie_t *baddie, double time) {
  assert(baddie->kind == AZ_BAD_OTH_ORB_1);
  az_drift_towards_ship(state, baddie, time, 80, 20, 100);
  if (baddie->cooldown <= 0.0 && az_ship_in_range(state, baddie, 500)) {
    for (int i = 0; i < 360; i += 20) {
      az_fire_baddie_projectile(
          state, baddie, AZ_PROJ_OTH_SPRAY,
          baddie->data->main_body.bounding_radius, AZ_DEG2RAD(i), 0.0);
    }
    az_play_sound(&state->soundboard, AZ_SND_FIRE_OTH_SPRAY);
    baddie->cooldown = 2.0;
  }
}

void az_tick_bad_oth_orb_2(
    az_space_state_t *state, az_baddie_t *baddie, double time) {
  assert(baddie->kind == AZ_BAD_OTH_ORB_2);
  az_drift_towards_ship(state, baddie, time, 80, 20, 100);
  if (baddie->cooldown <= 0.0 && az_ship_in_range(state, baddie, 500) &&
      az_can_see_ship(state, baddie)) {
    az_fire_baddie_projectile(state, baddie, AZ_PROJ_OTH_HOMING,
                              baddie->data->main_body.bounding_radius,
                              az_random(-AZ_PI, AZ_PI), 0.0);
    baddie->cooldown = 0.1;
  }
}

void az_tick_bad_oth_razor(
    az_space_state_t *state, az_baddie_t *baddie, double time) {
  assert(baddie->kind == AZ_BAD_OTH_RAZOR);
  if (baddie->state == 0) {
    baddie->velocity = az_vpolar(az_random(300, 500), baddie->angle);
    baddie->state = 2;
  } else if (baddie->state == 1) {
    baddie->velocity = az_vpolar(300, baddie->angle);
    baddie->state = 3;
  }
  if (baddie->state == 2) {
    az_drift_towards_ship(state, baddie, time, 400, 500, 100);
    baddie->angle = az_mod2pi(baddie->angle + AZ_DEG2RAD(180) * time);
  } else if (baddie->state == 3) {
    baddie->velocity = az_vwithlen(baddie->velocity, 300.0);
    baddie->angle = az_mod2pi(baddie->angle - AZ_DEG2RAD(180) * time);
  } else baddie->state = 0;
}

/*===========================================================================*/

void az_tick_bad_oth_snapdragon(
    az_space_state_t *state, az_baddie_t *baddie, double time) {
  assert(baddie->kind == AZ_BAD_OTH_SNAPDRAGON);
  az_fly_towards_ship(state, baddie, time,
                      5.0, 300.0, 300.0, 200.0, 0.0, 100.0);
  if (baddie->cooldown <= 0.0) {
    const bool crazy = (baddie->health <= 0.15 * baddie->data->max_health);
    if (crazy) baddie->state = 0;
    // The Snapdragon proceeds in a 9-state cycle; each state continues to the
    // next until it loops back around to zero.
    switch (baddie->state) {
      // States 0, 1, 2, 4, and 6: Fire an Oth Rocket.
      case 0: case 1: case 2: case 4: case 6:
        if (crazy || az_ship_within_angle(state, baddie, 0, AZ_DEG2RAD(6))) {
          az_projectile_t *proj = az_fire_baddie_projectile(
              state, baddie, AZ_PROJ_OTH_ROCKET, 30.0, 0.0, 0.0);
          if (proj != NULL) {
            proj->power = (crazy ? 0.3 : 0.5);
            az_play_sound(&state->soundboard, AZ_SND_FIRE_OTH_ROCKET);
            baddie->cooldown = (crazy ? 0.75 : 2.0);
            ++baddie->state;
          }
        }
        break;
      // States 3 and 7: Fire a spray of homing projectiles.
      case 3: case 7:
        for (int i = 0; i < 360; i += 15) {
          az_fire_baddie_projectile(
              state, baddie, AZ_PROJ_OTH_SPRAY,
              baddie->data->main_body.bounding_radius, AZ_DEG2RAD(i), 0.0);
        }
        az_play_sound(&state->soundboard, AZ_SND_FIRE_OTH_SPRAY);
        baddie->cooldown = 2.0;
        ++baddie->state;
        break;
      // State 5: Launch three Oth Razors that home in on the ship.
      case 5:
        for (int i = -1; i <= 1; ++i) {
          az_baddie_t *razor = az_add_baddie(
              state, AZ_BAD_OTH_RAZOR, baddie->position,
              baddie->angle + AZ_PI + i * AZ_DEG2RAD(45));
          if (razor == NULL) break;
        }
        az_play_sound(&state->soundboard, AZ_SND_LAUNCH_OTH_RAZORS);
        baddie->cooldown = az_random(2.0, 4.0);
        ++baddie->state;
        break;
      // State 8: Launch four Oth Razors that bounce aimlessly.
      case 8:
        for (int i = 0; i < 4; ++i) {
          az_baddie_t *razor = az_add_baddie(
              state, AZ_BAD_OTH_RAZOR, baddie->position,
              baddie->angle + AZ_DEG2RAD(45) + i * AZ_DEG2RAD(90));
          if (razor != NULL) razor->state = 1;
          else break;
        }
        az_play_sound(&state->soundboard, AZ_SND_LAUNCH_OTH_RAZORS);
        baddie->cooldown = az_random(2.0, 4.0);
        baddie->state = 0;
        break;
      default:
        baddie->state = 0;
        break;
    }
  }
}

/*===========================================================================*/

void az_tick_bad_oth_gunship(
    az_space_state_t *state, az_baddie_t *baddie, double time) {
  assert(baddie->kind == AZ_BAD_OTH_GUNSHIP);
  const double hurt = 1.0 - baddie->health / baddie->data->max_health;
  switch (baddie->state) {
    // State 0: Intro.
    case 0:
      baddie->cooldown = 10.0;
      baddie->state = 3; // TODO
      break;
    // State 1: Flee from ship.
    case 1: {
      // Out of all marker nodes the Oth Gunship can see, pick the one farthest
      // from the ship.
      double best_dist = 0.0;
      az_vector_t target = baddie->position;
      AZ_ARRAY_LOOP(node, state->nodes) {
        if (node->kind != AZ_NODE_MARKER) continue;
        if (az_baddie_has_clear_path_to_position(state, baddie,
                                                 node->position)) {
          const double dist = az_vdist(node->position, state->ship.position);
          if (dist > best_dist) {
            best_dist = dist;
            target = node->position;
          }
        }
      }
      // If we've reached the target position (or if the cooldown expires), go
      // to state 2.  Otherwise, fly towards the target position.
      if (baddie->cooldown <= 0.0 ||
          az_vwithin(baddie->position, target, 50.0)) {
        if (!az_ship_in_range(state, baddie, 800)) {
          baddie->cooldown = 3.0;
          baddie->state = 3;
        } else baddie->state = 2;
      } else {
        az_fly_towards_position(state, baddie, time, target,
                                5.0, 300, 300, 200, 100);
      }
    } break;
    // State 2: Pursue the ship.
    case 2: {
      az_fly_towards_ship(state, baddie, time, 5.0, 300, 300, 200, 200, 100);
      if (az_ship_in_range(state, baddie, 300)) {
        baddie->state = 5;
      }
    } break;
    // State 3: Line up for C-plus dash.
    case 3: {
      baddie->velocity = AZ_VZERO;
      az_vector_t rel_impact;
      if (az_lead_target(az_vsub(state->ship.position, baddie->position),
                         state->ship.velocity, 1000.0, &rel_impact)) {
        const double goal_theta = az_vtheta(rel_impact);
        baddie->angle =
          az_angle_towards(baddie->angle, AZ_DEG2RAD(360) * time, goal_theta);
        if (fabs(az_mod2pi(baddie->angle - goal_theta)) <= AZ_DEG2RAD(1) &&
            az_baddie_has_clear_path_to_position(
                state, baddie, az_vadd(baddie->position, rel_impact))) {
          baddie->velocity = az_vpolar(1000.0, baddie->angle);
          baddie->state = 4;
          break;
        }
      }
      if (baddie->cooldown <= 0.0) baddie->state = 2;
    } break;
    // State 4: Use C-plus drive.
    case 4:
      if (fabs(az_mod2pi(az_vtheta(baddie->velocity) -
                         baddie->angle)) > AZ_DEG2RAD(5)) {
        baddie->state = 1;
      } else {
        // TODO: do another az_lead_target, and steer towards new position
        az_particle_t *particle;
        if (az_insert_particle(state, &particle)) {
          particle->kind = AZ_PAR_EMBER;
          particle->color = (az_color_t){64, 255, 64, 255};
          particle->position =
            az_vadd(baddie->position, az_vpolar(-15.0, baddie->angle));
          particle->velocity = AZ_VZERO;
          particle->lifetime = 0.3;
          particle->param1 = 20;
        }
      }
      break;
    // State 5: Dogfight.
    case 5: {
      az_fly_towards_ship(state, baddie, time, 5.0, 300, 300, 200, 200, 100);
      if (baddie->cooldown <= 0.0 &&
          az_ship_within_angle(state, baddie, 0, AZ_DEG2RAD(3)) &&
          az_can_see_ship(state, baddie)) {
        az_fire_baddie_projectile(state, baddie, AZ_PROJ_GUN_NORMAL,
                                  20.0, 0.0, 0.0);
        az_play_sound(&state->soundboard, AZ_SND_FIRE_GUN_NORMAL);
        baddie->cooldown = 0.1;
        if (az_random(0, 1) < 0.1) {
          baddie->state = 6 + az_randint(0, az_imin(3, (int)(4.0 * hurt)));
        }
      }
    } break;
    // State 6: Fire a charged triple shot.
    case 6:
      az_fly_towards_ship(state, baddie, time, 5.0, 300, 300, 200, 200, 100);
      if (baddie->cooldown <= 0.0 &&
          az_ship_within_angle(state, baddie, 0, AZ_DEG2RAD(8)) &&
          az_can_see_ship(state, baddie)) {
        for (int i = -1; i <= 1; ++i) {
          az_fire_baddie_projectile(state, baddie, AZ_PROJ_GUN_CHARGED_TRIPLE,
                                    20.0, 0.0, i * AZ_DEG2RAD(10));
        }
        az_play_sound(&state->soundboard, AZ_SND_FIRE_GUN_NORMAL);
        baddie->cooldown = 6.0;
        baddie->state = 1;
      }
      break;
    // State 7: Fire a hyper rocket.
    case 7:
      az_fly_towards_ship(state, baddie, time, 5.0, 300, 300, 200, 200, 100);
      if (baddie->cooldown <= 0.0 &&
          az_ship_within_angle(state, baddie, 0, AZ_DEG2RAD(3)) &&
          az_can_see_ship(state, baddie)) {
        az_fire_baddie_projectile(state, baddie, AZ_PROJ_HYPER_ROCKET,
                                  20.0, 0.0, 0.0);
        az_play_sound(&state->soundboard, AZ_SND_FIRE_HYPER_ROCKET);
        baddie->cooldown = 6.0;
        baddie->state = 1;
      }
      break;
    // State 8: Fire a charged homing shot.
    case 8:
      az_fly_towards_ship(state, baddie, time, 5.0, 300, 300, 200, 200, 100);
      if (baddie->cooldown <= 0.0 && az_can_see_ship(state, baddie)) {
        for (int i = 0; i < 4; ++i) {
          az_fire_baddie_projectile(
              state, baddie, AZ_PROJ_GUN_CHARGED_HOMING,
              20.0, 0.0, AZ_DEG2RAD(45) + i * AZ_DEG2RAD(90));
        }
        az_play_sound(&state->soundboard, AZ_SND_FIRE_GUN_NORMAL);
        baddie->cooldown = 6.0;
        baddie->state = 1;
      }
      break;
    // State 9: Fire a missile barrage.
    case 9:
      az_fly_towards_ship(state, baddie, time, 5.0, 300, 300, 200, 200, 100);
      if (baddie->cooldown <= 0.0 &&
          az_ship_within_angle(state, baddie, 0, AZ_DEG2RAD(8)) &&
          az_can_see_ship(state, baddie)) {
        az_fire_baddie_projectile(state, baddie, AZ_PROJ_MISSILE_BARRAGE,
                                  20.0, 0.0, 0.0);
        baddie->cooldown = 6.0;
        baddie->state = 1;
      }
      break;
    default:
      baddie->state = 0;
      break;
  }
}

/*===========================================================================*/