#include "User.hpp"

User::User(const string &username, const string &password) : username(username), password(password), balance(0) {}

string User::get_username() const
{
  return username;
}

string User::get_password() const
{
  return password;
}

void User::claim_balance()
{
  if (balance > 0)
  {
    cout << "Press 1 to claim your balance. Balance: $" << balance << endl;
    int option;
    cin >> option;
    if (option == 1)
    {
      cout << "Balance claimed!" << endl;
      balance = 0;
    }
    else
    {
      cout << "Balance not claimed." << endl;
    }
  }
  else
  {
    cout << "No balance to claim." << endl;
  }
}

void User::add_to_balance(const double &amount)
{
  balance += amount;
}

void User::subtract_from_balance(const double &amount)
{
  balance -= amount;
}

bool User::operator==(const User &user) const
{
  return username == user.username;
}