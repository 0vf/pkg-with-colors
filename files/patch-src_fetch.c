--- src/fetch.c.orig	2023-08-01 08:25:14 UTC
+++ src/fetch.c
@@ -41,14 +41,21 @@
 
 #include "pkgcli.h"
 
+#define RESET "\033[0m"
+#define BOLD "\033[1m"
+#define COLOR_GREEN "\033[32m"
+#define COLOR_RED "\033[31m"
+#define COLOR_YELLOW "\033[33m"
+#define COLOR_BLUE "\033[34m"
+
 void
 usage_fetch(void)
 {
-	fprintf(stderr, "Usage: pkg fetch [-r reponame] [-o destdir] [-dqUy] "
+	fprintf(stderr, COLOR_YELLOW BOLD "Usage: " RESET BOLD "pkg fetch [-r reponame] [-o destdir] [-dqUy] "
 					"[-Cgix] <pkg-name> <...>\n");
-	fprintf(stderr, "       pkg fetch [-r reponame] [-dqUy] -a\n");
-	fprintf(stderr, "       pkg fetch [-r reponame] [-dqUy] -u\n\n");
-	fprintf(stderr, "For more information see 'pkg help fetch'.\n");
+	fprintf(stderr, RESET COLOR_BLUE "       pkg fetch [-r reponame] [-dqUy] -a\n");
+	fprintf(stderr, "       pkg fetch [-r reponame] [-dqUy] -u\n\n" RESET BOLD);
+	fprintf(stderr, "For more information see 'pkg help fetch'.\n" RESET);
 }
 
 int
@@ -148,7 +155,7 @@ exec_fetch(int argc, char **argv)
 	retcode = pkgdb_access(mode, PKGDB_DB_REPO);
 
 	if (retcode == EPKG_ENOACCESS) {
-		warnx("Insufficient privileges to access repo catalogue");
+		warnx(BOLD COLOR_RED "Insufficient privileges to access repo catalogue" RESET);
 		return (EXIT_FAILURE);
 	} else if (retcode != EPKG_OK)
 		return (EXIT_FAILURE);
@@ -157,7 +164,7 @@ exec_fetch(int argc, char **argv)
 		retcode = pkgdb_access(PKGDB_MODE_READ, PKGDB_DB_LOCAL);
 
 		if (retcode == EPKG_ENOACCESS) {
-			warnx("Insufficient privileges to access the package database");
+			warnx(BOLD COLOR_RED "Insufficient privileges to access the package database" RESET);
 			return (EXIT_FAILURE);
 		} else if (retcode != EPKG_OK)
 			return (EXIT_FAILURE);
@@ -173,7 +180,7 @@ exec_fetch(int argc, char **argv)
 
 	if (pkgdb_obtain_lock(db, PKGDB_LOCK_READONLY) != EPKG_OK) {
 		pkgdb_close(db);
-		warnx("Cannot get a read lock on a database, it is locked by another process");
+		warnx(BOLD COLOR_YELLOW "Cannot get a read lock on a database, it is locked by another process" RESET);
 		return (EXIT_FAILURE);
 	}
 
@@ -202,15 +209,15 @@ exec_fetch(int argc, char **argv)
 	if (!quiet) {
 
 		rc = print_jobs_summary(jobs,
-		    "The following packages will be fetched:\n\n");
+		    BOLD "The following packages will be fetched:\n\n" RESET);
 
 		if (rc != 0) {
-			rc = query_yesno(false, "\nProceed with fetching "
-					"packages? ");
+			rc = query_yesno(false, BOLD "\nProceed with fetching "
+					"packages? " RESET);
 		} else {
-			printf("No packages are required to be fetched.\n");
-			rc = query_yesno(false, "Check the integrity of packages "
-							"downloaded? ");
+			printf(BOLD "No packages are required to be fetched.\n" RESET);
+			rc = query_yesno(false, BOLD "Check the integrity of packages "
+							"downloaded? " RESET);
 			csum_only = true;
 		}
 	}
@@ -222,7 +229,7 @@ exec_fetch(int argc, char **argv)
 		goto cleanup;
 
 	if (csum_only && !quiet)
-		printf("Integrity check was successful.\n");
+		printf(BOLD COLOR_GREEN "Integrity check was successful.\n" RESET);
 
 	status = EXIT_SUCCESS;
 
