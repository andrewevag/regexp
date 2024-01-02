#include <iostream>
#include <check.h>
#include <cstdlib>
#include <cstdio>
#include "regexp.hpp"
#include "nfae.hpp"
#include "dfa.hpp"

START_TEST(dfamatching_initial_test){
	Regexp* r = Regexp::parse("[0-9]+");
	Dfa d = r->toNfae().toDfa();
	ck_assert_int_eq(d.match("1230438259347320"), 1);
	ck_assert_int_eq(d.match("12a30438259347320"), 0);
	ck_assert_int_eq(d.match("1230438259347 320"), 0);
	ck_assert_int_eq(d.match("00123"), 1);

} END_TEST

START_TEST(identifiers){
	Regexp* r = Regexp::parse("[a-zA-Z][a-zA-Z0-9_]+");
	Dfa d = r->toNfae().toDfa();
	ck_assert_int_eq(d.match("x_1"), 1);
	ck_assert_int_eq(d.match("camelCaseTest"), 1);
	ck_assert_int_eq(d.match("lower_bar_test"), 1);
	ck_assert_int_eq(d.match("1x"), 0);
	ck_assert_int_eq(d.match("x "), 0);

} END_TEST

Suite *dfamatching_suite(void){
	Suite* s;
	TCase *tc_core;
	s = suite_create("dfamatching");
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, dfamatching_initial_test);
	tcase_add_test(tc_core, identifiers);


	suite_add_tcase(s, tc_core);
	return s;
}
int main(void){
	int no_failed = 0;
	Suite* s;
	SRunner* runner;
	s = dfamatching_suite();
	runner = srunner_create(s);

	srunner_run_all(runner, CK_NORMAL);
	no_failed = srunner_ntests_failed(runner);
	srunner_free(runner);
	if(no_failed == 0){
		printf("✔️  Success!\n");
	}else{
		printf("❌ %d tests failed!\n", no_failed);
	}	
	return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


