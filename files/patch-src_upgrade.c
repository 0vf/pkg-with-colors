--- src/upgrade.c.orig	2023-08-01 08:25:14 UTC
+++ src/upgrade.c
@@ -51,14 +51,19 @@
 #endif
 #include "pkgcli.h"
 #include <pkg/audit.h>
+#define RESET "\033[0m"
+#define BOLD "\033[1m"
+#define COLOR_GREEN "\033[32m"
+#define COLOR_RED "\033[31m"
+#define COLOR_YELLOW "\033[33m"
 
 static const char vuln_end_lit[] = "**END**";
 
 void
 usage_upgrade(void)
 {
-	fprintf(stderr, "Usage: pkg upgrade [-fInFqUy] [-r reponame] [-Cgix] <pkg-name> ...\n\n");
-	fprintf(stderr, "For more information see 'pkg help upgrade'.\n");
+	fprintf(stderr, BOLD "Usage: pkg upgrade [-fInFqUy] [-r reponame] [-Cgix] <pkg-name> ...\n\n");
+	fprintf(stderr, "For more information see 'pkg help upgrade'.\n" RESET);
 }
 
 static void
@@ -88,7 +93,7 @@ check_vulnerable(struct pkg_audit *audit, struct pkgdb
 	}
 
 	if ((it = pkgdb_query(db, NULL, MATCH_ALL)) == NULL) {
-		warnx("Error accessing the package database");
+		warnx(COLOR_RED BOLD "Error accessing the package database" RESET);
 		pkg_audit_free(audit);
 		fclose(out);
 		return;
@@ -114,7 +119,7 @@ check_vulnerable(struct pkg_audit *audit, struct pkgdb
 
 
 	if (pkg_audit_load(audit, NULL) != EPKG_OK) {
-		warn("unable to open vulnxml file");
+		warn(COLOR_YELLOW BOLD "unable to open vulnxml file" RESET);
 		fclose(out);
 		pkg_audit_free(audit);
 		pkghash_destroy(check);
@@ -126,7 +131,7 @@ check_vulnerable(struct pkg_audit *audit, struct pkgdb
 #ifdef HAVE_CAPSICUM
 #ifndef PKG_COVERAGE
 	if (cap_enter() < 0 && errno != ENOSYS) {
-		warn("cap_enter() failed");
+		warn(COLOR_YELLOW BOLD "cap_enter() failed" RESET);
 		pkg_audit_free(audit);
 		pkghash_destroy(check);
 		fclose(out);
@@ -142,7 +147,7 @@ check_vulnerable(struct pkg_audit *audit, struct pkgdb
 				pkg = (struct pkg *)hit.value;
 				if (pkg_audit_is_vulnerable(audit, pkg, &issues, true)) {
 					pkg_get(pkg, PKG_ATTR_UNIQUEID, &uid);
-					fprintf(out, "%s\n", uid);
+					fprintf(out, BOLD "%s\n" RESET, uid);
 					fflush(out);
 				}
 				pkg_audit_issues_free(issues);
@@ -152,7 +157,7 @@ check_vulnerable(struct pkg_audit *audit, struct pkgdb
 		fprintf(out, "%s\n", vuln_end_lit);
 		fflush(out);
 	} else {
-		warnx("cannot process vulnxml");
+		warnx(COLOR_YELLOW BOLD "cannot process vulnxml" RESET);
 	}
 
 	pkg_audit_free(audit);
@@ -225,7 +230,7 @@ add_vulnerable_upgrades(struct pkg_jobs	*jobs, struct 
 		}
 
 		if (pkg_jobs_add(jobs, MATCH_EXACT, &line, 1) == EPKG_FATAL) {
-			warnx("Cannot update %s which is vulnerable", line);
+			warnx(COLOR_YELLOW BOLD "Cannot update %s which is vulnerable" RESET, line);
 			/* TODO: assume it non-fatal for now */
 		}
 	}
@@ -237,14 +242,14 @@ add_vulnerable_upgrades(struct pkg_jobs	*jobs, struct 
 			continue;
 		}
 		else {
-			warnx("Cannot wait");
+			warnx(COLOR_RED BOLD "Cannot wait" RESET);
 
 			return (EPKG_FATAL);
 		}
 	}
 
 	if (ret != EPKG_OK) {
-		warn("Cannot get the complete list of vulnerable packages");
+		warn(COLOR_YELLOW BOLD "Cannot get the complete list of vulnerable packages" RESET);
 	}
 
 	return (ret);
@@ -352,7 +357,7 @@ exec_upgrade(int argc, char **argv)
 	}
 
 	if (retcode == EPKG_ENOACCESS) {
-		warnx("Insufficient privilege to upgrade packages");
+		warnx(COLOR_RED BOLD "Insufficient privilege to upgrade packages" RESET);
 		return (EXIT_FAILURE);
 	} else if (retcode != EPKG_OK)
 		return (EXIT_FAILURE);
@@ -369,7 +374,7 @@ exec_upgrade(int argc, char **argv)
 
 	if (pkgdb_obtain_lock(db, lock_type) != EPKG_OK) {
 		pkgdb_close(db);
-		warnx("Cannot get an advisory lock on a database, it is locked by another process");
+		warnx(COLOR_RED BOLD "Cannot get an advisory lock on a database, it is locked by another process" RESET);
 		return (EXIT_FAILURE);
 	}
 
@@ -400,12 +405,12 @@ exec_upgrade(int argc, char **argv)
 		rc = yes;
 		if (!quiet || dry_run) {
 			print_jobs_summary(jobs,
-				"The following %d package(s) will be affected (of %d checked):\n\n",
+				BOLD "The following %d package(s) will be affected (of %d checked):\n\n" RESET,
 				nbactions, pkg_jobs_total(jobs));
 
 			if (!dry_run) {
-				rc = query_yesno(false, "\nProceed with this "
-						"action? ");
+				rc = query_yesno(false, BOLD "\nProceed with this "
+						"action? " RESET);
 			} else {
 				rc = false;
 			}
@@ -415,9 +420,9 @@ exec_upgrade(int argc, char **argv)
 			retcode = pkg_jobs_apply(jobs);
 			done = 1;
 			if (retcode == EPKG_CONFLICT) {
-				printf("Conflicts with the existing packages "
+				printf(BOLD COLOR_RED "Conflicts with the existing packages "
 				    "have been found.\nOne more solver "
-				    "iteration is needed to resolve them.\n");
+				    "iteration is needed to resolve them.\n" RESET);
 				continue;
 			}
 			else if (retcode != EPKG_OK)
@@ -432,7 +437,7 @@ exec_upgrade(int argc, char **argv)
 	}
 
 	if (done == 0 && rc && !quiet)
-		printf("Your packages are up to date.\n");
+		printf(COLOR_GREEN BOLD "Your packages are up to date.\n" RESET);
 
 	if (rc)
 		retcode = EXIT_SUCCESS;
