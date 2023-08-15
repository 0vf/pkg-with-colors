--- src/autoremove.c.orig	2023-08-01 08:25:14 UTC
+++ src/autoremove.c
@@ -36,11 +36,19 @@
 
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
 usage_autoremove(void)
 {
-	fprintf(stderr, "Usage: pkg autoremove [-Dynq]\n\n");
-	fprintf(stderr, "For more information see 'pkg help autoremove'.\n");
+	fprintf(stderr, BOLD "Usage: pkg autoremove [-Dynq]\n\n");
+	fprintf(stderr, "For more information see 'pkg help autoremove'.\n" RESET);
 }
 
 int
@@ -98,13 +106,13 @@ exec_autoremove(int argc, char **argv)
 				       PKGDB_DB_LOCAL);
 
 	if (retcode == EPKG_ENOACCESS) {
-		warnx("Insufficient privileges to autoremove packages");
+		warnx(COLOR_RED BOLD "Insufficient privileges to autoremove packages" RESET);
 		return (EXIT_FAILURE);
 	} else if (retcode == EPKG_ENODB) {
-		warnx("No packages installed.  Nothing to do!");
+		warnx(COLOR_YELLOW BOLD "No packages installed.  Nothing to do!" RESET);
 		return (EXIT_SUCCESS);
 	} else if (retcode != EPKG_OK) {
-		warnx("Error accessing the package database");
+		warnx(COLOR_RED BOLD "Error accessing the package database" RESET);
 		return (EXIT_FAILURE);
 	}
 
@@ -138,10 +146,10 @@ exec_autoremove(int argc, char **argv)
 
 	if (!quiet || dry_run) {
 		print_jobs_summary(jobs,
-				"Deinstallation has been requested for the following %d packages:\n\n", nbactions);
+				BOLD "Deinstallation has been requested for the following " COLOR_RED BOLD "%d " RESET BOLD "packages:\n\n" RESET, nbactions);
 		if (!dry_run)
 			rc = query_yesno(false,
-		            "\nProceed with deinstalling packages? ");
+		            BOLD "\nProceed with deinstalling packages? " RESET);
 	}
 	if ((yes || rc ) && !dry_run && ((retcode = pkg_jobs_apply(jobs)) != EPKG_OK)) {
 		goto cleanup;
