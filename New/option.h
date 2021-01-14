#pragma once
#include "common.h"

/**
 * @file option.h
 * @author Viktor Lönnborg, Erik Sik
 * @date 17 Sep 2020
 * @brief TODO
 *
 * TODO
 *
 */


typedef struct ioopm_option ioopm_option_t;
struct ioopm_option
{
  bool success;
  elem_t value;
};


/// @brief Create a successful optional value
/// param v The payload that is successfully returned
ioopm_option_t ioopm_option_success(elem_t v);

/// @brief Create a failed optional value (with no payload)
ioopm_option_t ioopm_option_failure();

/// @brief Check if the optional value exists
bool ioopm_option_successful(ioopm_option_t o);

bool ioopm_option_unsuccessful(ioopm_option_t o);