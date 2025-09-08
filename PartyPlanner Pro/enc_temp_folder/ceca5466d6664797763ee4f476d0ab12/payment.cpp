#include "payment.h"

static const regex kAmountRegex("^\\d+(?:[.,]\\d{1,2})?$");

static void printPayment(const Payment& p) {
	cout << left << setw(14) << "Payment ID" << ": " << p.paymentId << "\n";
	cout << left << setw(14) << "Event" << ": " << p.eventName << "\n";
	cout << left << setw(14) << "Payer" << ": " << p.payerName << "\n";
	cout << left << setw(14) << "Amount" << ": " << formatCentsToAmount(p.amountCents) << " " << p.currency << "\n";
	cout << left << setw(14) << "Method" << ": " << p.method << "\n";
	cout << left << setw(14) << "Reference" << ": " << p.reference << "\n";
	cout << left << setw(14) << "Status" << ": " << p.status << "\n";
	cout << left << setw(14) << "Created At" << ": " << p.createdAt << "\n";
}

void paymentMenu(const vector<Event>& events) {
	while (true) {
		cout << "\n=== Payment Menu ===\n";
		cout << "1. Record Manual Payment\n";
		cout << "2. List Payments\n";
		cout << "3. Create Invoice (RM100 + RM10/guest)\n";
		cout << "0. Back\n";
		cout << "Choose an option: ";
		string choice; getline(cin, choice);
		if (choice == "1") {
			recordManualPayment(events);
		} else if (choice == "2") {
			listPayments();
		} else if (choice == "3") {
			recordInvoicePayment(events);
		} else if (choice == "0") {
			break;
		} else {
			cout << "Invalid choice. Try again.\n";
		}
	}
}

void recordManualPayment(const vector<Event>& events) {
	if (events.empty()) {
		cout << "No events available. Create an event first.\n";
		return;
	}

	cout << "Select event by number:\n";
	for (size_t i = 0; i < events.size(); ++i) {
		cout << (i + 1) << ") " << events[i].eventName << " - " << events[i].date << " " << events[i].time << " @ " << events[i].location << "\n";
	}
	cout << "Choice: ";
	string idxStr; getline(cin, idxStr);
	int idx = 0;
	try { idx = stoi(idxStr); } catch (...) { idx = 0; }
	if (idx <= 0 || static_cast<size_t>(idx) > events.size()) {
		cout << "Invalid selection.\n";
		return;
	}
	const Event& selectedEvent = events[idx - 1];

	cout << "Payer name (optional, press Enter to skip): ";
	string payer; getline(cin, payer);

	string amountInput;
	while (true) {
		cout << "Amount (e.g., 100 or 100.50): ";
		getline(cin, amountInput);
		if (regex_match(amountInput, kAmountRegex)) break;
		cout << "Invalid amount format.\n";
	}
	int amountCents = parseAmountToCents(amountInput);

	cout << "Currency (default USD): ";
	string currency; getline(cin, currency);
	if (currency.empty()) currency = "USD";

	cout << "Method (cash/bank_transfer/other): ";
	string method; getline(cin, method);
	if (method.empty()) method = "cash";

	cout << "Reference/Receipt (optional): ";
	string reference; getline(cin, reference);

	Payment p;
	p.paymentId = generatePaymentId();
	p.eventName = selectedEvent.eventName;
	p.payerName = payer;
	p.amountCents = amountCents;
	p.currency = currency;
	p.method = method;
	p.reference = reference;
	p.status = "succeeded";
	p.createdAt = currentTimestamp();

	vector<Payment> payments;
	loadPaymentsFromFile(payments);
	payments.push_back(p);
	savePaymentsToFile(payments);

	cout << "Payment recorded successfully.\n\n";
	printPayment(p);
}

void listPayments() {
	vector<Payment> payments;
	loadPaymentsFromFile(payments);
	if (payments.empty()) {
		cout << "No payments recorded yet.\n";
		return;
	}
	cout << "\n=== Payments ===\n";
	for (const auto& p : payments) {
		printPayment(p);
		cout << "-----------------------------\n";
	}
}

void recordInvoicePayment(const vector<Event>& events) {
	if (events.empty()) {
		cout << "No events available. Create an event first.\n";
		return;
	}

	cout << "Select event by number to invoice:\n";
	for (size_t i = 0; i < events.size(); ++i) {
		cout << (i + 1) << ") " << events[i].eventName << " - " << events[i].date << " " << events[i].time << " @ " << events[i].location << "\n";
	}
	cout << "Choice: ";
	string idxStr; getline(cin, idxStr);
	int idx = 0;
	try { idx = stoi(idxStr); } catch (...) { idx = 0; }
	if (idx <= 0 || static_cast<size_t>(idx) > events.size()) {
		cout << "Invalid selection.\n";
		return;
	}
	const Event& selectedEvent = events[idx - 1];

	// Ask how many guests to include in the invoice
	cout << "Number of guests to include: ";
	string guestsStr; getline(cin, guestsStr);
	int numGuests = 0;
	try { numGuests = stoi(guestsStr); } catch (...) { numGuests = 0; }
	if (numGuests < 0) numGuests = 0;

	int eventFee = DEFAULT_EVENT_FEE_CENTS;
	int guestFee = DEFAULT_PER_GUEST_FEE_CENTS * numGuests;
	int total = eventFee + guestFee;

	cout << "\n=== Invoice Preview (MYR) ===\n";
	cout << left << setw(20) << "Event fee" << ": RM" << formatCentsToAmount(eventFee) << "\n";
	cout << left << setw(20) << (string("Guests (" ) + to_string(numGuests) + ")") << ": RM" << formatCentsToAmount(guestFee) << "\n";
	cout << left << setw(20) << "Total" << ": RM" << formatCentsToAmount(total) << "\n";

	cout << "\nProceed to mark as paid? (y/n): ";
	string yn; getline(cin, yn);
	if (yn != "y" && yn != "Y") {
		cout << "Invoice not recorded.\n";
		return;
	}

	cout << "Payment method (cash/bank_transfer/other): ";
	string method; getline(cin, method);
	if (method.empty()) method = "cash";

	cout << "Reference/Receipt (optional): ";
	string reference; getline(cin, reference);

	Payment p;
	p.paymentId = generatePaymentId();
	p.eventName = selectedEvent.eventName;
	p.payerName = "";
	p.amountCents = total;
	p.currency = "MYR";
	p.method = method;
	p.reference = reference;
	p.status = "succeeded";
	p.createdAt = currentTimestamp();

	vector<Payment> payments;
	loadPaymentsFromFile(payments);
	payments.push_back(p);
	savePaymentsToFile(payments);

	cout << "Payment recorded. Receipt:\n\n";
	printPayment(p);
}

void savePaymentsToFile(const vector<Payment>& payments) {
	ofstream out(PAYMENTS_FILE);
	if (!out) {
		cout << "Failed to write payments to file." << "\n";
		return;
	}
	for (const auto& p : payments) {
		// pipe-delimited to align with simple text storage approach used elsewhere
		out << p.paymentId << "|" << p.eventName << "|" << p.payerName << "|"
			<< p.amountCents << "|" << p.currency << "|" << p.method << "|"
			<< p.reference << "|" << p.status << "|" << p.createdAt << "\n";
	}
}

void loadPaymentsFromFile(vector<Payment>& payments) {
	payments.clear();
	ifstream in(PAYMENTS_FILE);
	if (!in) {
		return; // No file yet
	}
	string line;
	while (getline(in, line)) {
		if (line.empty()) continue;
		stringstream ss(line);
		string field;
		Payment p;
		getline(ss, p.paymentId, '|');
		getline(ss, p.eventName, '|');
		getline(ss, p.payerName, '|');
		getline(ss, field, '|'); p.amountCents = stoi(field);
		getline(ss, p.currency, '|');
		getline(ss, p.method, '|');
		getline(ss, p.reference, '|');
		getline(ss, p.status, '|');
		getline(ss, p.createdAt, '|');
		payments.push_back(p);
	}
}

string generatePaymentId() {
	static int counter = 0;
	stringstream ss;
	ss << "PMT" << time(nullptr) << "_" << (++counter);
	return ss.str();
}

string currentTimestamp() {
	time_t now = time(nullptr);
	struct tm tmBuf;
#ifdef _WIN32
	localtime_s(&tmBuf, &now);
#else
	localtime_r(&now, &tmBuf);
#endif
	char buf[20];
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tmBuf);
	return string(buf);
}

int parseAmountToCents(const string& input) {
	string s = input;
	replace(s.begin(), s.end(), ',', '.');
	size_t dot = s.find('.');
	if (dot == string::npos) {
		long long dollars = stoll(s);
		return static_cast<int>(dollars * 100);
	}
	string dollars = s.substr(0, dot);
	string cents = s.substr(dot + 1);
	if (cents.size() == 1) cents += "0";
	if (cents.size() > 2) cents = cents.substr(0, 2);
	long long d = dollars.empty() ? 0 : stoll(dollars);
	long long c = cents.empty() ? 0 : stoll(cents);
	return static_cast<int>(d * 100 + c);
}

string formatCentsToAmount(int cents) {
	stringstream ss;
	bool negative = cents < 0;
	int absCents = negative ? -cents : cents;
	ss << (negative ? "-" : "") << (absCents / 100) << "." << setw(2) << setfill('0') << (absCents % 100);
	return ss.str();
}


