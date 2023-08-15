--- src/utils.c.orig	2023-08-01 08:25:14 UTC
+++ src/utils.c
@@ -57,6 +57,13 @@
 #include <bsd_compat.h>
 
 #include "pkgcli.h"
+#define RESET "\033[0m"
+#define BOLD "\033[1m"
+#define COLOR_GREEN "\033[32m"
+#define COLOR_RED "\033[31m"
+#define COLOR_YELLOW "\033[33m"
+#define COLOR_BLUE "\033[34m"
+#define COLOR_PURPLE "\033[35m"
 
 struct jobs_sum_number {
 	int install;
@@ -905,13 +912,13 @@ display_summary_item(struct pkg_solved_display *it, in
 
 
 static const char* pkg_display_messages[PKG_DISPLAY_MAX + 1] = {
-	[PKG_DISPLAY_LOCKED] = "Installed packages LOCKED",
-	[PKG_DISPLAY_DELETE] = "Installed packages to be REMOVED",
-	[PKG_DISPLAY_INSTALL] = "New packages to be INSTALLED",
-	[PKG_DISPLAY_UPGRADE] = "Installed packages to be UPGRADED",
-	[PKG_DISPLAY_DOWNGRADE] = "Installed packages to be DOWNGRADED",
-	[PKG_DISPLAY_REINSTALL] = "Installed packages to be REINSTALLED",
-	[PKG_DISPLAY_FETCH] = "New packages to be FETCHED",
+	[PKG_DISPLAY_LOCKED] = BOLD "Installed packages " COLOR_RED "LOCKED" RESET,
+	[PKG_DISPLAY_DELETE] = BOLD "Installed packages to be " COLOR_RED "REMOVED" RESET,
+	[PKG_DISPLAY_INSTALL] = BOLD "New packages to be " COLOR_GREEN "INSTALLED" RESET,
+	[PKG_DISPLAY_UPGRADE] = BOLD "Installed packages to be " COLOR_GREEN "UPGRADED" RESET,
+	[PKG_DISPLAY_DOWNGRADE] = BOLD "Installed packages to be " COLOR_YELLOW "DOWNGRADED" RESET,
+	[PKG_DISPLAY_REINSTALL] = BOLD "Installed packages to be " COLOR_PURPLE "REINSTALLED" RESET,
+	[PKG_DISPLAY_FETCH] = BOLD "New packages to be " COLOR_BLUE "FETCHED" RESET,
 	[PKG_DISPLAY_MAX] = NULL
 };
 
@@ -985,24 +992,24 @@ print_jobs_summary(struct pkg_jobs *jobs, const char *
 
 	puts("");
 	if (sum.delete > 0) {
-		printf("Number of packages to be removed: %d\n", sum.delete);
+		printf(BOLD "Number of packages to be " COLOR_RED "removed" RESET ": " BOLD COLOR_YELLOW "%d\n" RESET, sum.delete);
 	}
 	if (sum.install > 0) {
-		printf("Number of packages to be installed: %d\n", sum.install);
+		printf(BOLD "Number of packages to be " COLOR_GREEN "installed" RESET ": " BOLD COLOR_YELLOW "%d\n" RESET, sum.install);
 	}
 	if (sum.upgrade > 0) {
-		printf("Number of packages to be upgraded: %d\n", sum.upgrade);
+		printf(BOLD "Number of packages to be " COLOR_BLUE "upgraded" RESET ": " BOLD COLOR_YELLOW "%d\n" RESET, sum.upgrade);
 	}
 	if (sum.reinstall > 0) {
-		printf("Number of packages to be reinstalled: %d\n",
+		printf(BOLD "Number of packages to be " COLOR_PURPLE "reinstalled" RESET ": " BOLD COLOR_YELLOW "%d\n" RESET,
 		    sum.reinstall);
 	}
 	if (sum.downgrade > 0) {
-		printf("Number of packages to be downgraded: %d\n",
+		printf(BOLD "Number of packages to be " COLOR_YELLOW "downgraded" RESET ": " BOLD COLOR_YELLOW "%d\n" RESET,
 		    sum.downgrade);
 	}
 	if (sum.fetch > 0) {
-		printf("Number of packages to be fetched: %d\n", sum.fetch);
+		printf(BOLD "Number of packages to be " COLOR_BLUE "fetched" RESET ": " BOLD COLOR_YELLOW "%d\n", sum.fetch);
 	}
 	/* Add an extra line before the size output. */
 	if (bytes_change > limbytes || dlsize)
@@ -1012,18 +1019,18 @@ print_jobs_summary(struct pkg_jobs *jobs, const char *
 		if (oldsize > newsize) {
 			humanize_number(size, sizeof(size), oldsize - newsize, "B",
 			    HN_AUTOSCALE, HN_IEC_PREFIXES);
-			printf("The operation will free %s.\n", size);
+			printf(BOLD "The operation will free " COLOR_YELLOW "%s" RESET BOLD ".\n" RESET, size);
 		} else if (newsize > oldsize) {
 			humanize_number(size, sizeof(size), newsize - oldsize, "B",
 			    HN_AUTOSCALE, HN_IEC_PREFIXES);
-			printf("The process will require %s more space.\n", size);
+			printf(BOLD "The process will require " COLOR_YELLOW BOLD "%s " RESET BOLD "more space.\n" RESET, size);
 		}
 	}
 
 	if (dlsize > 0) {
 		humanize_number(size, sizeof(size), dlsize, "B",
 		    HN_AUTOSCALE, HN_IEC_PREFIXES);
-		printf("%s to be downloaded.\n", size);
+		printf(COLOR_YELLOW BOLD "%s " RESET BOLD "to be downloaded.\n" RESET, size);
 	}
 
 	return (displayed);
