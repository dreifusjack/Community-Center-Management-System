#pragma once

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Facility;

class User
{
private:
  string username; // must be unique
  string password;
  double balance;

public:
  User(const string &username, const string &password);
  virtual ~User() = default;

  string get_username() const;
  string get_password() const;
  /**
   * Prompts the user to claim their balance if they have any.
   */
  void claim_balance();
  /**
   * Increments this User's current balance.
   */
  void add_to_balance(const double &amount);

  /**
   * Decrements this User's current balance.
   */
  void subtract_from_balance(const double &amount);

  // overloaded == operator
  bool operator==(const User &user) const;

  /**
   * Displays the menu options for the User.
   */
  virtual void display_menu() = 0;
  /**
   * Handles menu input for the User.
   */
  virtual void handle_menu_input(Facility &facility) = 0;
  virtual bool has_overbooked(const int &hours) = 0;
};