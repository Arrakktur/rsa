#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <cmath>
#include "big_integer.h"

using namespace std;

const big_integer maxp = 16777216;
const big_integer maxq = 4294967300;

big_integer p, q, n, e, f, d;
unsigned long long nkey;
unsigned long long hex_char[3] = { 0 };
int number_char = 0;
unsigned long long y;
bool hex_first = true;
unsigned long long signature_char[3] = { 0 };

//íîä
big_integer gcd(big_integer a, big_integer b) {
	if (b == 0)
		return a;
	return gcd(b, a%b);
}

//âîçâåäåíèå â ñòåïåíü
big_integer pows(big_integer a, big_integer b, big_integer m) {
	big_integer c = 1;
	while (b > 0) {
		if (b % 2 == 0)
			c *= c % m;
		if (b % 2 == 1)
			c *= a * c % m;
		b = b / 2;
	}
	return c;
}

//ïðîâåðêà íà ïðîñòîòó
bool prime(big_integer n) {
	for (big_integer i = 3; i*i < n; i += 2) {
		if (n % i == 0) return false;
	}
	return true;
}

//ïîëó÷åíèå ñëó÷àéíîãî ÷èñëà
big_integer getRandom(big_integer x) {
	big_integer c;
	c = int(pow(rand(), 2)) % x;
	while (prime(c) != true) {
		c = int(pow(rand(), 2)) % x;
	}
	return c;
}

big_integer xgcd(big_integer a, big_integer b, big_integer* x) {
	big_integer u1, u2, u3;
	big_integer v1, v2, v3;
	big_integer t1, t2, t3;
	big_integer q;
	u1 = a;	u2 = 1;	u3 = 0;
	v1 = b; v2 = 0; v3 = 1;
	while (v1 != 0) {
		q = u1 / v1;

		t1 = u1 % v1;
		t2 = u2 - q * v2;
		t3 = u3 - q * v3;

		u1 = v1; u2 = v2; u3 = v3;
		v1 = t1; v2 = t2; v3 = t3;
	}

	*x = u2;
	return u1;
}

big_integer getE(big_integer d, big_integer mod) {
	big_integer e;
	xgcd(d, mod, &e);
	return e % mod;
}

unsigned long long mul(unsigned long long a, unsigned long long b, unsigned long long m) {
	if (b == 1)
		return a;
	if (b % 2 == 0) {
		long long t = mul(a, b / 2, m);
		return (2 * t) % m;
	}
	return (mul(a, b - 1, m) + a) % m;
}

//âû÷èñëåíèå ñòåïåíè ïî ìîäóëþ
unsigned long long func(unsigned long long a, unsigned long long b, unsigned long long m) {
	if (a == 0) {
		return nkey;
	}
	if (a == nkey) {
		return 0;
	}
	if (b == 0)
		return 1;
	if (b % 2 == 0) {
		long long t = func(a, b / 2, m);
		return mul(t, t, m) % m;
	}
	return (mul(func(a, b - 1, m), a, m)) % m;
}

//õåø-ôóíêöèÿ
void hex(unsigned long long a) {
	if (number_char > 2) {
		number_char = 0;
		hex_first = false;
	}

	if (hex_first) {
		hex_char[number_char] = a;
	}
	else {
		hex_char[number_char] = hex_char[number_char] xor a;
	}
	number_char++;
}

//ïîëó÷àåì d
big_integer getD(big_integer f) {
	big_integer d = rand() % f;
	while (gcd(d, f) != 1 && d < f)
		d = rand() % f;
	return d;
}

void getKeys() {
	ofstream file, efile;
	file.open("keys.txt");
	efile.open("pkeys.txt");
	p = 1046527;
	//p = getRandom(maxp);
	q = 452401;
	//q = getRandom(maxq);
	n = p * q;
	f = (p - 1)*(q - 1);
	d = getD(f);
	e = getE(d, f);
	cout << "p: " << p << endl;
	cout << "q: " << q << endl;
	cout << "n: " << n << endl;
	cout << "f: " << f << endl;
	cout << "d: " << d << endl;
	cout << "e: " << e << endl;
	cout << prime(p) << " " << prime(q) << endl;
	file << n << " " <<  d;
	efile << e;
}

void shifr() {
	ifstream file, efile;
	file.open("keys.txt");
	efile.open("pkeys.txt");
	unsigned long long n, d, e;
	file >> n >> d;
	efile >> e;
	nkey = n;
	FILE * in = fopen("1.txt", "rb");
	ofstream of;
	of.open("2.txt", ios_base::out);
	big_integer c;
	unsigned char m;
	while (!feof(in))
	{
		m = fgetc(in);
		if (feof(in) == 1) break;
		c = func(m, d, n);
		if (c > n) c = func(m, d, n);
		of << c << " ";
	}
	fclose(in); of.close();
}

void deshifr() {
	ifstream file, efile;
	file.open("keys.txt");
	efile.open("pkeys.txt");
	unsigned long long c, n, d, e;
	file >> n >> d;
	nkey = n;
	efile >> e;
	ifstream in; in.open("2.txt", ios_base::in);
	FILE * out = fopen("3.txt", "wb");
	unsigned char m;
	while (!in.eof())
	{
		in >> c;
		if (in.eof() == 1) break;
		m = func(c, e, n);
		fputc((char)m, out);
	}
	in.close(); fclose(out);
}

string test() {
	unsigned long long int a = 64;
	a = a << 24;
	return to_string(a);
}

void create_signature() {
	ifstream file, efile;
	file.open("keys.txt");
	efile.open("pkeys.txt");
	unsigned long long n, e, y, s;
	file >> n;
	efile >> e;
	nkey = n;
	FILE * in = fopen("1.txt", "rb");
	ofstream of;
	of.open("signature.txt", ios_base::out);
	big_integer c;
	unsigned char m;
	while (!feof(in)) {
		m = fgetc(in); 
		if (feof(in) == 1) break;
		hex(m);
	}
	number_char = 0;
	hex_first = true;
	cout << hex_char[0] << " " << hex_char[1] << " " << hex_char[2] << " " << endl;
	y = (hex_char[0] << 16) + (hex_char[1] << 8) + hex_char[2];
	cout << endl << y << endl;
	s = func(y, e, n);
	of << s;
	fclose(in); of.close();
}

string verify_signature() {
	ifstream file, efile, signature;
	file.open("keys.txt");
	efile.open("pkeys.txt");
	signature.open("signature.txt");
	unsigned long long n, e, y, s, w, d;
	file >> n >> d;
	efile >> e;
	signature >> s;
	nkey = n;
	FILE * in = fopen("1.txt", "rb");
	big_integer c;
	unsigned char m;
	while (!feof(in)) {
		m = fgetc(in);
		if (feof(in) == 1) break;
		hex(m);
	}
	number_char = 0;
	hex_first = true;
	cout << hex_char[0] << " " << hex_char[1] << " " << hex_char[2] << " " << endl;
	y = (hex_char[0] << 16) + (hex_char[1] << 8) + hex_char[2];
	cout << "\n*" << s << endl << d << endl << n << "*\n" ;
	w = func(s, d, n);
	cout << y << endl << w << endl;
	fclose(in);
	if (y == w) {
		return "Подпись подтверждена";
	}
	else {
		return "Подпись не подтверждена";
	}
}

int main() {
	srand(time(NULL));
	setlocale(LC_ALL, "ru");
	short answer;
	bool exit = false;
	string message = "";

	while (!exit) {
		system("clear");
		if (message != "") {
			cout << endl << "***" << message << "***" << endl << endl;
		}
		cout << "Выберите действие: " << endl; 
		cout << "1 - Сгенерировать ключ" << endl; 
		cout << "2 - Зашифровать файл" << endl; 
		cout << "3 - Расшифровать файл" << endl;
		cout << "4 - Создать подпись" << endl;
		cout << "5 - Проверить подпись" << endl;
		cout << "0 - Выход" << endl;
		cout << "Ответ: ";
		cin >> answer;
		switch (answer)
		{
		case 1:
			getKeys();
			message = "Ключи сгенерированы!";
			break;
		case 2:
			shifr();
			message = "Сообщение зашифровано!";
			break;
		case 3:
			deshifr();
			message = "Сообщение расшифровано";
			break;
		case 4:
			create_signature();
			message = "Подпись создана";
			break;
		case 5:
			message = verify_signature();
			//verify_signature();
			break;
		case 9:
			message = test();
			break;
		case 0:
			exit = true;
			break;
		default:
			cout << "Неверное значение!";
			break;
		}
	}
	return 0;
}
