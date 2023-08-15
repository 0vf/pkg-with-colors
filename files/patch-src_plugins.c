--- src/plugins.c.orig	2023-08-01 08:25:14 UTC
+++ src/plugins.c
@@ -34,6 +34,13 @@
 
 #include "pkgcli.h"
 
+#define RESET "\033[0m"
+#define BOLD "\033[1m"
+#define COLOR_GREEN "\033[32m"
+#define COLOR_RED "\033[31m"
+#define COLOR_YELLOW "\033[33m"
+#define COLOR_BLUE "\033[34m"
+
 void
 usage_plugins(void)
 {
@@ -69,9 +76,9 @@ exec_plugins(int argc, char **argv)
 	 * @todo: implement enabling, disabling and configuring of plugins
 	 */
 
-	printf("%-10s %-45s %-10s\n", "NAME", "DESC", "VERSION");
+	printf(COLOR_YELLOW BOLD "%-10s %-45s %-10s\n", "NAME", "DESC", "VERSION" RESET);
 	while (pkg_plugins(&p) != EPKG_END)
-		printf("%-10s %-45s %-10s\n",
+		printf(COLOR_BLUE "%-10s %-45s %-10s\n RESET",
 		       pkg_plugin_get(p, PKG_PLUGIN_NAME),
 		       pkg_plugin_get(p, PKG_PLUGIN_DESC),
 		       pkg_plugin_get(p, PKG_PLUGIN_VERSION));
