#!/usr/bin/env qore

%requires sysconf


printf("%N\n", sysconf_constants());

printf("\nCount of CPUs: %N\n\n", sysconf(Sysconf::SC_NPROCESSORS_ONLN));

my hash $sc = sysconf_constants();

foreach my string $i in (keys $sc) {
    try {
        my int $val = sysconf($sc.$i);
        printf("%s -> %n\n", $i, $val == Sysconf::SC_NO_LIMIT ? "is not set (-1)" : $val);
    } catch ($ex) {
        printf("%s -> %s:%s\n", $i, $ex.err, $ex.desc);
    }
}


my hash $cs = confstr_constants();

printf("%N\n", $cs);
foreach my string $i in (keys $cs) {
    try {
        my string $val = confstr($cs.$i);
        printf("%s -> %n\n", $i, $val);
    } catch ($ex) {
        printf("%s -> %s:%s\n", $i, $ex.err, $ex.desc);
    }
}