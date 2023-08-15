--- src/config.c.orig	2023-08-01 08:25:14 UTC
+++ src/config.c
@@ -33,12 +33,20 @@
 
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
 usage_config(void)
 {
 	fprintf(stderr,
-            "Usage: pkg config <name>\n\n");
-	//fprintf(stderr, "For more information see 'pkg help config'.\n");
+            BOLD "Usage: pkg config <name>\n\n" RESET);
+	//fprintf(stderr, BOLD "For more information see 'pkg help config'.\n" RESET);
 }
 
 int
@@ -63,31 +71,31 @@ exec_config(int argc, char **argv)
 
 	conf = pkg_config_get(key);
 	if (conf == NULL) {
-		warnx("No such configuration options: %s", key);
+		warnx(COLOR_RED BOLD "No such configuration options: %s" RESET, key);
 		return (EXIT_FAILURE);
 	}
 
 	switch (pkg_object_type(conf)) {
 	case PKG_STRING:
 		buf = pkg_object_string(conf);
-		printf("%s\n", buf == NULL ? "" : buf);
+		printf(BOLD "%s\n" RESET, buf == NULL ? "" : buf);
 		break;
 	case PKG_BOOL:
 		b = pkg_object_bool(conf);
-		printf("%s\n", b ? "yes" : "no");
+		printf(BOLD "%s\n" RESET, b ? "yes" : "no");
 		break;
 	case PKG_INT:
 		integer = pkg_object_int(conf);
-		printf("%"PRId64"\n", integer);
+		printf(BOLD "%"PRId64"\n" RESET, integer);
 		break;
 	case PKG_OBJECT:
 		while ((o = pkg_object_iterate(conf, &it))) {
-			printf("%s: %s\n", pkg_object_key(o), pkg_object_string(o));
+			printf(BOLD "%s: %s\n" RESET, pkg_object_key(o), pkg_object_string(o));
 		}
 		break;
 	case PKG_ARRAY:
 		while ((o = pkg_object_iterate(conf, &it))) {
-			printf("%s\n", pkg_object_string(o));
+			printf(BOLD "%s\n" RESET, pkg_object_string(o));
 		}
 		break;
 	default:
