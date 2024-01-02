#include "regexp.hpp"
#include <iostream>
#include <check.h>
#include <cstdlib>
#include <cstdio>
#include <sstream>
START_TEST(one_char){
	Regexp* r = Regexp::parse("e");
	std::ostringstream buf;
	r->print(buf);
	ck_assert_int_eq(strcmp(buf.str().c_str(), "e"), 0);
	delete r;

} END_TEST

START_TEST(parenthesized_char){
	Regexp* r = Regexp::parse("(e)");
	std::ostringstream buf;
	r->print(buf);
	ck_assert_int_eq(strcmp(buf.str().c_str(), "e"), 0);
	delete r;
} END_TEST

START_TEST(one_plus){
	Regexp* r = Regexp::parse("e|d");
	std::ostringstream buf;
	r->print(buf);
	ck_assert_int_eq(strcmp(buf.str().c_str(), "(e|d)"), 0);
	delete r;
} END_TEST

START_TEST(one_dot){
	Regexp* r = Regexp::parse("ed");
	std::ostringstream buf;
	r->print(buf);
	ck_assert_int_eq(strcmp(buf.str().c_str(), "(e.d)"), 0);
	delete r;
} END_TEST

START_TEST(one_star){
	Regexp* r = Regexp::parse("e*");
	std::ostringstream buf;
	r->print(buf);
	ck_assert_int_eq(strcmp(buf.str().c_str(), "(e)*"), 0);
	delete r;
} END_TEST

START_TEST(numbers){
	Regexp* r = Regexp::parse("[0-9]+");
	std::ostringstream buf;
	r->print(buf);
	ck_assert_int_eq(strcmp(buf.str().c_str(), "([0123456789].([0123456789])*)"), 0);
	delete r;
} END_TEST

START_TEST(multi_star){
	Regexp* r = Regexp::parse("e*");
	std::ostringstream buf;
	r->print(buf);
	ck_assert_int_eq(strcmp(buf.str().c_str(), "(e)*"), 0);
	delete r;
} END_TEST


START_TEST(identifiers){
	Regexp* r = Regexp::parse("[a-zA-Z][a-zA-Z_]*");
	std::ostringstream buf;
	r->print(buf);
	ck_assert_int_eq(strcmp(buf.str().c_str(), "([ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz].([ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz])*)"), 0);
	delete r;
} END_TEST

START_TEST(plus_dot_star){
	Regexp* r = Regexp::parse("((a|b)(b|d))*");
	std::ostringstream buf;
	r->print(buf);
	ck_assert_int_eq(strcmp(buf.str().c_str(), "(((a|b).(b|d)))*"), 0);
	delete r;
} END_TEST

START_TEST(plus_dot_star2){
	Regexp* r = Regexp::parse("(a|bd)*");
	std::ostringstream buf;
	r->print(buf);
	ck_assert_int_eq(strcmp(buf.str().c_str(), "((a|(b.d)))*"), 0);
	delete r;
} END_TEST


Suite *regexp_suite(void){
	Suite* s;
	TCase *tc_core;
	s = suite_create("regexp");
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, one_char);
	tcase_add_test(tc_core, parenthesized_char);
	tcase_add_test(tc_core, one_plus);
	tcase_add_test(tc_core, one_dot);
	tcase_add_test(tc_core, one_star);
	tcase_add_test(tc_core, numbers);
	tcase_add_test(tc_core, identifiers);
	tcase_add_test(tc_core, multi_star);
	tcase_add_test(tc_core, plus_dot_star);
	tcase_add_test(tc_core, plus_dot_star2);
	
	suite_add_tcase(s, tc_core);
	return s;
}
int main(void){
	int no_failed = 0;
	Suite* s;
	SRunner* runner;
	s = regexp_suite();
	runner = srunner_create(s);

	srunner_run_all(runner, CK_NORMAL);
	no_failed = srunner_ntests_failed(runner);
	srunner_free(runner);
	if(no_failed == 0){
		std::cout << "✔️  Success!\n";
	}else{
		std::cout << "❌ " << no_failed << " tests failed!\n";
	}	
	return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


