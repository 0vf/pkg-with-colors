--- src/alias.c.orig	2023-08-01 08:25:14 UTC
+++ src/alias.c
@@ -38,11 +38,19 @@
 
 #include "pkgcli.h"
 
+#define RESET "\033[0m"
+#define BOLD "\033[1m"
+#define COLOR_GREEN "\033[32m"
+#define COLOR_RED "\033[31m"
+#define COLOR_YELLOW "\033[33m"
+#define COLOR_BLUE "\033[34m"
+#define COLOR_PURPLE "\033[35m"
+
 void
 usage_alias(void)
 {
-	fprintf(stderr, "Usage: pkg alias [-ql] [alias]\n\n");
-	fprintf(stderr, "For more information see 'pkg help alias'.\n");
+	fprintf(stderr, BOLD "Usage: pkg alias [-ql] [alias]\n\n");
+	fprintf(stderr, "For more information see 'pkg help alias'.\n" RESET);
 }
 
 int
@@ -106,7 +114,7 @@ exec_alias(int argc, char **argv)
 			else
 				printf("%-20s '%s'\n", argv[i], pkg_object_string(alias));
 		} else {
-			warnx("No such alias: '%s'", argv[i]);
+			warnx(COLOR_YELLOW BOLD "No such alias: " COLOR_RED "'%s'" RESET, argv[i]);
 			ret = EXIT_FAILURE;
 		}
 	}
