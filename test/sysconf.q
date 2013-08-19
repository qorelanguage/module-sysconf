#!/usr/bin/env qore

%requires sysconf


sub test(hash $c, string $function) {
    my HashIterator $it($c);
    my any $ret;
    while ($it.next()) {
        try {
            if (exists $argv)
                $ret = call_function_args($function, $argv + $it.getValue());
            else
                $ret = call_function($function, $it.getValue());
            printf("%s: %s = %n\n", $function, $it.getKey(), $ret);
        }
        catch ($ex) {
            printf("ERROR: %s: %s = %s:%s\n", $function, $it.getKey(), $ex.err, $ex.desc);
        }
    }
}

test(sysconf_constants(), "sysconf");
test(confstr_constants(), "confstr");
test(pathconf_constants(), "pathconf", "/etc/passwd");

printf("\nCount of CPUs: %N\n\n", sysconf(Sysconf::SC_NPROCESSORS_ONLN));
printf("%N\n", sysconf_constants());