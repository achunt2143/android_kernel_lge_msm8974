
# This filter requires one command line option of form -vN=n
# where n must be a decimal number.
#
# Repeat each input line containing $$ n times, replacing $$ with 0...n-1.
# Replace each $# with n, and each $* with a single $.

BEGIN {
	n = N + 0
}
{
	if (/\$\$/) { rep = n } else { rep = 1 }
	for (i = 0; i < rep; ++i) {
		tmp = $0
		gsub(/\$\$/, i, tmp)
<<<<<<< HEAD
		gsub(/\$\#/, n, tmp)
=======
		gsub(/\$#/, n, tmp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		gsub(/\$\*/, "$", tmp)
		print tmp
	}
}
