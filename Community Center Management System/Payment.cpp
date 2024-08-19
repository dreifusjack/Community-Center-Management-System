#include "Payment.hpp"
#include <string>

using namespace std;

Payment::Payment(double amount, long card_number, int cvv, string expiry_date)
    : amount(amount), card_number(card_number), cvv(cvv), expiry_date(expiry_date) {}

double Payment::get_amount() const { return amount; }

long Payment::get_card_number() const { return card_number; }

int Payment::get_cvv() const { return cvv; }

string Payment::get_expiry_date() const { return expiry_date; }