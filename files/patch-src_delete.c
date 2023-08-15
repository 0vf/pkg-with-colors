--- src/delete.c.orig	2023-08-01 08:25:14 UTC
+++ src/delete.c
@@ -38,12 +38,18 @@
 
 #include "pkgcli.h"
 
+#define RESET "\033[0m"
+#define BOLD "\033[1m"
+#define COLOR_GREEN "\033[32m"
+#define COLOR_RED "\033[31m"
+#define COLOR_YELLOW "\033[33m"
+
 void
 usage_delete(void)
 {
-	fprintf(stderr, "Usage: pkg delete [-DfnqRy] [-Cgix] <pkg-name> ...\n");
+	fprintf(stderr, BOLD "Usage: pkg delete [-DfnqRy] [-Cgix] <pkg-name> ...\n");
 	fprintf(stderr, "       pkg delete [-Dnqy] -a\n\n");
-	fprintf(stderr, "For more information see 'pkg help delete'.\n");
+	fprintf(stderr, "For more information see 'pkg help delete'.\n" RESET);
 }
 
 int
@@ -136,13 +142,13 @@ exec_delete(int argc, char **argv)
 				       PKGDB_DB_LOCAL);
 
 	if (retcode == EPKG_ENODB) {
-		warnx("No packages installed.  Nothing to do!");
+		warnx(COLOR_YELLOW BOLD "No packages installed.  Nothing to do!" RESET);
 		return (EXIT_SUCCESS);
 	} else if (retcode == EPKG_ENOACCESS) {
-		warnx("Insufficient privileges to delete packages");
+		warnx(COLOR_RED BOLD "Insufficient privileges to delete packages" RESET);
 		return (EXIT_FAILURE);
 	} else if (retcode != EPKG_OK) {
-		warnx("Error accessing the package database");
+		warnx(COLOR_RED BOLD "Error accessing the package database" RESET);
 		return (EXIT_FAILURE);
 	}
 
@@ -151,7 +157,7 @@ exec_delete(int argc, char **argv)
 
 	if (pkgdb_obtain_lock(db, lock_type) != EPKG_OK) {
 		pkgdb_close(db);
-		warnx("Cannot get an advisory lock on a database, it is locked by another process");
+		warnx(COLOR_RED BOLD "Cannot get an advisory lock on a database, it is locked by another process" RESET);
 		return (EXIT_FAILURE);
 	}
 
@@ -185,14 +191,14 @@ exec_delete(int argc, char **argv)
 		goto cleanup;
 
 	if (pkg_jobs_solve(jobs) != EPKG_OK) {
-		fprintf(stderr, "Cannot perform request\n");
+		fprintf(stderr, COLOR_RED BOLD "Cannot perform request\n" RESET);
 		retcode = EXIT_FAILURE;
 		goto cleanup;
 	}
 
 	if (pkg_jobs_has_lockedpkgs(jobs)) {
-		printf("The following package(s) are locked and may not ");
-		printf("be removed:\n\n");
+		printf(COLOR_YELLOW BOLD "The following package(s) are locked and may not ");
+		printf("be removed:\n\n" RESET);
 		pkg_jobs_iter_lockedpkgs(jobs, print_pkg, &locked_pkgs);
 		printf("\n");
 	}
@@ -201,14 +207,14 @@ exec_delete(int argc, char **argv)
 	if ((nbactions = pkg_jobs_count(jobs)) == 0) {
 		if (argc == 0) {
 			if (!quiet)
-				printf("Nothing to do.\n");
+				printf(COLOR_GREEN BOLD "Nothing to do.\n" RESET);
 
 			retcode = EXIT_SUCCESS;
 			goto cleanup;
 		}
 		if (!quiet) {
-			printf("%d packages requested for removal: "
-			    "%d locked, %d missing\n",
+			printf(BOLD "%d packages requested for removal: "
+			    "%d " COLOR_YELLOW BOLD "locked," "%d " COLOR_RED BOLD "missing\n",
 			    argc, locked_pkgs, argc - locked_pkgs);
 		}
 		if (locked_pkgs > 0) {
@@ -222,8 +228,8 @@ exec_delete(int argc, char **argv)
 	if (!quiet || dry_run) {
 		if (!quiet) {
 			print_jobs_summary(jobs,
-				"Deinstallation has been requested for the following %d packages "
-				"(of %d packages in the universe):\n\n", nbactions,
+				BOLD "Deinstallation has been requested for the following %d packages "
+				"(of %d packages in the universe):\n\n" RESET, nbactions,
 				pkg_jobs_total(jobs));
 		}
 		if (dry_run) {
@@ -231,7 +237,7 @@ exec_delete(int argc, char **argv)
 			goto cleanup;
 		}
 		rc = query_yesno(false,
-		            "\nProceed with deinstalling packages? ");
+		            BOLD "\nProceed with deinstalling packages? " RESET);
 	}
 	else
 		rc = yes;
