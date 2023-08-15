--- src/install.c.orig	2023-08-01 08:25:14 UTC
+++ src/install.c
@@ -43,12 +43,18 @@
 
 #include "pkgcli.h"
 
+#define RESET "\033[0m"
+#define BOLD "\033[1m"
+#define COLOR_GREEN "\033[32m"
+#define COLOR_RED "\033[31m"
+#define COLOR_YELLOW "\033[33m"
+
 void
 usage_install(void)
 {
 	fprintf(stderr,
-	    "Usage: pkg install [-AfInFMqRUy] [-r reponame] [-Cgix] <pkg-name> ...\n\n");
-	fprintf(stderr, "For more information see 'pkg help install'.\n");
+	    COLOR_YELLOW BOLD "Usage: " RESET BOLD "pkg install [-AfInFMqRUy] [-r reponame] [-Cgix] <pkg-name> ...\n\n");
+	fprintf(stderr, "For more information see 'pkg help install'.\n" RESET);
 }
 
 int
@@ -185,7 +191,7 @@ exec_install(int argc, char **argv)
 	}
 
 	if (retcode == EPKG_ENOACCESS) {
-		warnx("Insufficient privileges to install packages");
+		warnx(COLOR_RED BOLD "Insufficient privileges to install packages" RESET);
 		return (EXIT_FAILURE);
 	} else if (retcode != EPKG_OK)
 		return (EXIT_FAILURE);
@@ -202,7 +208,7 @@ exec_install(int argc, char **argv)
 
 	if (pkgdb_obtain_lock(db, lock_type) != EPKG_OK) {
 		pkgdb_close(db);
-		warnx("Cannot get an advisory lock on a database, it is locked by another process");
+		warnx(COLOR_RED BOLD "Cannot get an advisory lock on a database, it is locked by another process" RESET);
 		return (EXIT_FAILURE);
 	}
 
@@ -228,7 +234,7 @@ exec_install(int argc, char **argv)
 		/* print a summary before applying the jobs */
 		if (!quiet || dry_run) {
 			print_jobs_summary(jobs,
-			    "The following %d package(s) will be affected (of %d checked):\n\n",
+			    BOLD "The following %d package(s) will be affected (of %d checked):\n\n" RESET,
 			    nbactions, pkg_jobs_total(jobs));
 		}
 		if (dry_run)
@@ -236,16 +242,16 @@ exec_install(int argc, char **argv)
 
 		if (!quiet) {
 			rc = query_yesno(false,
-			    "\nProceed with this action? ");
+			    BOLD "\nProceed with this action? " RESET);
 		}
 
 		if (rc) {
 			retcode = pkg_jobs_apply(jobs);
 			done = 1;
 			if (retcode == EPKG_CONFLICT) {
-				printf("Conflicts with the existing packages "
+				printf(COLOR_RED BOLD "Conflicts with the existing packages "
 				    "have been found.\nOne more solver "
-				    "iteration is needed to resolve them.\n");
+				    "iteration is needed to resolve them.\n" RESET);
 				continue;
 			}
 			else if (retcode != EPKG_OK) {
@@ -262,7 +268,7 @@ exec_install(int argc, char **argv)
 	}
 
 	if (done == 0 && rc)
-		printf("The most recent versions of packages are already installed\n");
+		printf(COLOR_YELLOW BOLD "The most recent versions of packages are already installed\n" RESET);
 
 	if (!rc)
 		status = EXIT_FAILURE;
