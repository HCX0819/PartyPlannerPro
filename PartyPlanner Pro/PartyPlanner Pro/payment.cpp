#include "payment.h"

static const regex kAmountRegex("^\\d+(?:[.,]\\d{1,2})?$");

static void printPayment(const Payment& p) {
	cout << left << setw(14) << "Payment ID" << ": " << p.paymentId << "\n";
	cout << left << setw(14) << "Event" << ": " << p.eventName << "\n";
	cout << left << setw(14) << "Payer" << ": " << p.payerName << "\n";
	cout << left << setw(14) << "Amount" << ": " << formatCentsToAmount(p.amountCents) << " " << p.currency << "\n";
	cout << left << setw(14) << "Method" << ": " << p.method << "\n";
	cout << left << setw(14) << "Status" << ": " << p.status << "\n";
	cout << left << setw(14) << "Created At" << ": " << p.createdAt << "\n";
}

void paymentMenu(const vector<Event>& events, const vector<Guest>& guests, const string& currentUser) {
	while (true) {
		cout << "\n=== Payment Menu ===\n";
		cout << "1. Make Payment (RM100/event + RM10/guest)\n";
		cout << "2. Payment History\n";
		cout << "0. Back\n";
		cout << "Choose an option: ";
		string choice; getline(cin, choice);
		if (choice == "1") {
			recordInvoicePayment(events, guests, currentUser);
		} else if (choice == "2") {
			listPayments();
		} else if (choice == "0") {
			break;
		} else {
			cout << "Invalid choice. Try again.\n";
		}
	}
}

void recordManualPayment(const vector<Event>& events, const string& currentUser) {
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

	//avoid pay two times for the same event
	if (hasPaidForEvent(selectedEvent.eventName, currentUser)) {
		cout << "A payment for this event has already been recorded for user '" << currentUser << "'.\n";
		cout << "Use Payment History to review or choose a different event.\n";
		return;
	}

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

	Payment p;
	p.paymentId = generatePaymentId();
	p.eventName = selectedEvent.eventName;
	p.payerName = payer;
	p.amountCents = amountCents;
	p.currency = currency;
	p.method = method;
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

void recordInvoicePayment(const vector<Event>& events, const vector<Guest>& guests, const string& currentUser) {
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

	// Prevent duplicate payments for the same event by the same user (invoice flow)
	if (hasPaidForEvent(selectedEvent.eventName, currentUser)) {
		cout << "A payment for this event has already been recorded for user '" << currentUser << "'.\n";
		cout << "Use Payment History to review or choose a different event.\n";
		return;
	}

	// Auto-calculate from provided guest list
	int numGuests = static_cast<int>(guests.size());

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

	cout << "Payment Method :\n";
	cout << "1) E-Wallet\n";
	cout << "2) Debit Card\n";
	cout << "3) Credit Card\n";
	cout << "4) Cash\n";
	cout << "Choose option: ";
	string methodChoice; getline(cin, methodChoice);
	string method;
	if (methodChoice == "1") method = "E-Wallet";
	else if (methodChoice == "2") method = "Debit Card";
	else if (methodChoice == "3") method = "Credit Card";
	else method = "Cash";

	Payment p;
	p.paymentId = generatePaymentId();
	p.eventName = selectedEvent.eventName;
	p.payerName = currentUser;
	p.amountCents = total;
	p.currency = "MYR";
	p.method = method;
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
			<< p.status << "|" << p.createdAt << "\n";
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
		getline(ss, field, '|');
		try { p.amountCents = stoi(field); } catch (...) { p.amountCents = 0; }
		getline(ss, p.currency, '|');
		getline(ss, p.method, '|');
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

bool hasPaidForEvent(const string& eventName, const string& /*username*/) {
	ifstream in(PAYMENTS_FILE);
	if (!in) return false;
	string line;
	auto trim = [](const string& s){ size_t a=s.find_first_not_of(" \t\r\n"); if(a==string::npos) return string(""); size_t b=s.find_last_not_of(" \t\r\n"); return s.substr(a,b-a+1); };
	auto lower = [](string s){ transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return (char)tolower(c); }); return s; };
	string target = lower(trim(eventName));
	while (getline(in, line)) {
		if (line.empty()) continue;
		stringstream ss(line);
		string id, event, payer, amount, currency, method, status, createdAt;
		getline(ss, id, '|');
		getline(ss, event, '|');
		getline(ss, payer, '|');
		getline(ss, amount, '|');
		getline(ss, currency, '|');
		getline(ss, method, '|');
		getline(ss, status, '|');
		getline(ss, createdAt, '|');
		if (lower(trim(event)) == target && lower(trim(status)) == "succeeded") {
			return true;
		}
	}
	return false;
}


