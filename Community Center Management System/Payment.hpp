#pragma once

#include <string>

using namespace std;

class Payment
{
private:
  double amount;
  long card_number;
  int cvv;
  string expiry_date;

public:
  Payment(double amount, long card_number, int cvv, string expiry_date);
  ~Payment() = default;

  /**
   * Gets the amount of this Payment.
   */
  double get_amount() const;
  /**
   * Gets the card number of this Payment.
   */
  long get_card_number() const;
  /**
   * Gets the CVV of this Payment.
   */
  int get_cvv() const;
  /**
   * Gets the expiry date of this Payment.
   */
  string get_expiry_date() const;
};
