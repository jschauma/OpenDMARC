#include "../opendmarc_internal.h"
#include "../dmarc.h"

#define TESTFILE "testfiles/effective_tld_names.dat"
typedef struct {
	char *	subdomain;
	char *	tld;
	int	mode;
	int  	outcome;
} TEST_ALIGNMENT_T;

int
main(int argc, char **argv)
{
	TEST_ALIGNMENT_T *alignp;
	TEST_ALIGNMENT_T alignm_test[] = {
		/* 1 */ {"a.b.c.bcx.com",	"bcx.com",	DMARC_RECORD_A_RELAXED,	 0},
		/* 2 */ {"a.b.c.edu.ar",	"edr.au",	DMARC_RECORD_A_STRICT,	-1},
		/* 3 */ {"oooo.com",		"ooo.com",	DMARC_RECORD_A_STRICT,	-1},
		/* 4 */ {"a.foo.com",           "b.foo.com",	DMARC_RECORD_A_RELAXED,	 0},
		/* 5 */ {".mac.com.",           "mac.com",	DMARC_RECORD_A_STRICT,	 0},
		/* 6 */ {"....mac.com....",     "mac.com",	DMARC_RECORD_A_STRICT,	 0},
		/* 7 */ {"mac...com",           "..com",	DMARC_RECORD_A_STRICT,	-1},
			{NULL, NULL, 0},
	};
	int	outcome;
	int	pass, fails, count;
	
	if (opendmarc_tld_read_file(TESTFILE, "//", "*.", "!") != 0)
	{
		printf("\tTLD find test: %s: could not read. Skipping\n", TESTFILE);
		return 0;
	}
	pass = fails = count = 0;
	for (alignp = alignm_test; alignp != NULL && alignp->subdomain != NULL; ++alignp)
	{
		count += 1;
		outcome = opendmarc_policy_check_alignment(alignp->subdomain, alignp->tld, alignp->mode);
		if (outcome == alignp->outcome)
		{
			//printf("\tALIGNMENT find test: %d: PASS\n", count);
			pass += 1;
		}
		else
		{
			printf("\tALIGNMENT find test: %d: FAIL\n", count);
			fails += 1;
		}
	}
	printf("ALIGNMENT find test: pass=%d, fail=%d\n", pass, fails);
	return fails;
}