#ifndef PAYMENT_H
#define PAYMENT_H

#include "common.h"
#include "event_booking.h"

struct Payment {
	string paymentId;       // e.g., UUID or incremental ID
	string eventName;       // Link to Event by name
	string payerName;       // Optional: guest name or payer reference
	int amountCents = 0;    // Store in cents to avoid float errors
	string currency;        // e.g., "USD"
	string method;          // "cash", "bank_transfer", etc.
	string status;          // "succeeded", "failed", "refunded"
	string createdAt;       // ISO-like timestamp string
};

// File used to persist payments
const string PAYMENTS_FILE = "payments.txt";

// Simple pricing defaults for invoice flow (MYR)
const int DEFAULT_EVENT_FEE_CENTS = 10000;    // RM100.00
const int DEFAULT_PER_GUEST_FEE_CENTS = 1000; // RM10.00 per guest

// Core APIs (no globals)
void paymentMenu(const vector<Event>& events, const vector<Guest>& guests, const string& currentUser);
void recordManualPayment(const vector<Event>& events, const string& currentUser);
void listPayments();
void recordInvoicePayment(const vector<Event>& events, const vector<Guest>& guests, const string& currentUser);

// Storage helpers
void savePaymentsToFile(const vector<Payment>& payments);
void loadPaymentsFromFile(vector<Payment>& payments);

// Utility helpers
string generatePaymentId();
string currentTimestamp();
int parseAmountToCents(const string& input);
string formatCentsToAmount(int cents);
bool hasPaidForEvent(const string& eventName, const string& username);

#endif


