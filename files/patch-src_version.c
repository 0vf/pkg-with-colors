--- src/version.c.orig	2023-08-01 08:25:14 UTC
+++ src/version.c
@@ -52,6 +52,13 @@
 
 #include "pkgcli.h"
 
+#define RESET "\033[0m"
+#define BOLD "\033[1m"
+#define COLOR_GREEN "\033[32m"
+#define COLOR_RED "\033[31m"
+#define COLOR_YELLOW "\033[33m"
+#define COLOR_BLUE "\033[34m"
+
 extern char **environ;
 
 struct index_entry {
@@ -69,11 +76,11 @@ pkghash *categories = NULL;
 void
 usage_version(void)
 {
-	fprintf(stderr, "Usage: pkg version [-IPR] [-hoqvU] [-l limchar] [-L limchar] [-Cegix pattern]\n");
-	fprintf(stderr, "		    [-r reponame] [-O origin|-n pkgname] [index]\n");
+	fprintf(stderr, COLOR_YELLOW BOLD "Usage: " RESET BOLD "pkg version [-IPR] [-hoqvU] [-l limchar] [-L limchar] [-Cegix pattern]\n" RESET);
+	fprintf(stderr, COLOR_BLUE "		    [-r reponame] [-O origin|-n pkgname] [index]\n");
 	fprintf(stderr, "	pkg version -t <version1> <version2>\n");
-	fprintf(stderr, "	pkg version -T <pkgname> <pattern>\n\n");
-	fprintf(stderr, "For more information see 'pkg help version'.\n");
+	fprintf(stderr, "	pkg version -T <pkgname> <pattern>\n\n" RESET);
+	fprintf(stderr, BOLD "For more information see 'pkg help version'.\n" RESET);
 }
 
 static void
@@ -114,31 +121,31 @@ print_version(struct pkg *pkg, const char *source, con
 		return;
 
 	if (opt & VERSION_ORIGIN)
-		pkg_printf("%-34o %S", pkg, key);
+		pkg_printf(COLOR_BLUE BOLD "%-34o %S" RESET, pkg, key);
 	else {
-		cout = pkg_printf("%n-%v", pkg, pkg);
+		cout = pkg_printf(COLOR_BLUE BOLD "%n-%v" RESET, pkg, pkg);
 		cout = 35 - cout;
 		if (cout < 1)
 			cout = 1;
-		printf("%*s%s", cout, " ", key);
+		printf(COLOR_BLUE BOLD "%*s%s" RESET, cout, " ", key);
 	}
 
 	if (opt & VERSION_VERBOSE) {
 		switch (*key) {
 		case '<':
-			printf("   needs updating (%s has %s)", source, ver);
+			printf(COLOR_YELLOW BOLD "   needs updating (" COLOR_BLUE BOLD "%s " COLOR_YELLOW "has " COLOR_BLUE "%s" COLOR_YELLOW ")", source, ver);
 			break;
 		case '=':
-			printf("   up-to-date with %s", source);
+			printf(COLOR_GREEN BOLD "   up-to-date with " COLOR_BLUE BOLD "%s", source);
 			break;
 		case '>':
-			printf("   succeeds %s (%s has %s)", source, source, ver);
+			printf(COLOR_GREEN BOLD "   succeeds " COLOR_BLUE "%s " COLOR_YELLOW "(" COLOR_BLUE "%s " COLOR_YELLOW "has " COLOR_BLUE "%s" COLOR_YELLOW ")" RESET, source, source, ver);
 			break;
 		case '?':
-			pkg_printf("   orphaned: %o", pkg);
+			pkg_printf(COLOR_YELLOW BOLD "   orphaned: " COLOR_BLUE "%o" RESET, pkg);
 			break;
 		case '!':
-			printf("   Comparison failed");
+			printf(COLOR_RED BOLD "   Comparison failed" RESET);
 			break;
 		}
 	}
