#pragma once

#include <string>

using namespace std;

// Helper functions for User prompting
namespace prompt
{
  /**
   * Prompts the user for a valid date input MM/DD/YYYY.
   */
  string get_user_date_input();

  /**
   * Prompts the user for a valid time input (8-22).
   */
  string get_user_time_input();
}
