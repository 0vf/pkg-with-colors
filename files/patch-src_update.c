--- src/update.c.orig	2023-08-01 08:25:14 UTC
+++ src/update.c
@@ -41,6 +41,12 @@
 
 #include "pkgcli.h"
 
+#define RESET "\033[0m"
+#define BOLD "\033[1m"
+#define COLOR_GREEN "\033[32m"
+#define COLOR_RED "\033[31m"
+#define COLOR_YELLOW "\033[33m"
+
 /**
  * Fetch repository calalogues.
  */
@@ -56,7 +62,7 @@ pkgcli_update(bool force, bool strict, const char *rep
 		return (EPKG_OK);
 
 	if (pkg_repos_total_count() == 0) {
-		fprintf(stderr, "No active remote repositories configured.\n");
+		fprintf(stderr, COLOR_RED BOLD "No active remote repositories configured.\n" RESET);
 		return (EPKG_FATAL);
 	}
 
@@ -70,7 +76,7 @@ pkgcli_update(bool force, bool strict, const char *rep
 		}
 
 		if (!quiet)
-			printf("Updating %s repository catalogue...\n",
+			printf(BOLD "Updating " COLOR_GREEN "%s " RESET BOLD "repository catalogue...\n" RESET,
 			    pkg_repo_name(r));
 
 		retcode = pkg_update(r, force);
@@ -78,7 +84,7 @@ pkgcli_update(bool force, bool strict, const char *rep
 		if (retcode == EPKG_UPTODATE) {
 			retcode = EPKG_OK;
 			if (!quiet) {
-				printf("%s repository is up to date.\n",
+				printf(COLOR_GREEN BOLD "%s " RESET BOLD "repository is up to date.\n" RESET,
 				    pkg_repo_name(r));
 			}
 		}
@@ -95,22 +101,22 @@ pkgcli_update(bool force, bool strict, const char *rep
 	if (total_count == 0) {
 		retcode = EPKG_FATAL;
 		if (!quiet) {
-			printf("No repositories are enabled.\n");
+			printf(COLOR_RED BOLD "No repositories are enabled.\n" RESET);
 		}
 	}
 	else if (update_count == total_count) {
 		if (!quiet) {
-			printf("All repositories are up to date.\n");
+			printf(BOLD "All repositories are up to date.\n" RESET);
 		}
 	}
 	else if (total_count == 1) {
 		if (!quiet) {
-			printf("Error updating repositories!\n");
+			printf(COLOR_RED BOLD "Error updating repositories!\n" RESET);
 		}
 	}
 	else {
 		if (!quiet) {
-			printf("Error updating repositories!\n");
+			printf(COLOR_RED BOLD "Error updating repositories!\n" RESET);
 		}
 		if (strict) {
 			retcode = EPKG_FATAL;
@@ -124,8 +130,8 @@ pkgcli_update(bool force, bool strict, const char *rep
 void
 usage_update(void)
 {
-	fprintf(stderr, "Usage: pkg update [-fq] [-r reponame]\n\n");
-	fprintf(stderr, "For more information, see 'pkg help update'.\n");
+	fprintf(stderr, COLOR_YELLOW BOLD "Usage: " RESET BOLD "pkg update [-fq] [-r reponame]\n\n");
+	fprintf(stderr, BOLD "For more information, see 'pkg help update'.\n" RESET);
 }
 
 int
@@ -168,8 +174,8 @@ exec_update(int argc, char **argv)
 	ret = pkgdb_access(PKGDB_MODE_WRITE|PKGDB_MODE_CREATE,
 			   PKGDB_DB_REPO);
 	if (ret == EPKG_ENOACCESS) {
-		warnx("Insufficient privileges to update the repository "
-		      "catalogue.");
+		warnx(COLOR_RED BOLD "Insufficient privileges to update the repository "
+		      "catalogue." RESET);
 		return (EXIT_FAILURE);
 	} else if (ret != EPKG_OK)
 		return (EXIT_FAILURE);
