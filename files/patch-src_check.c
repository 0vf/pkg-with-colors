--- src/check.c.orig	2023-08-01 08:25:14 UTC
+++ src/check.c
@@ -44,6 +44,12 @@
 
 #include "pkgcli.h"
 
+#define RESET "\033[0m"
+#define BOLD "\033[1m"
+#define COLOR_GREEN "\033[32m"
+#define COLOR_RED "\033[31m"
+#define COLOR_YELLOW "\033[33m"
+
 typedef tll(char *) deps_entries;
 
 static int check_deps(struct pkgdb *db, struct pkg *pkg, deps_entries *dh,
@@ -71,7 +77,7 @@ check_deps(struct pkgdb *db, struct pkg *p, deps_entri
 			if (quiet)
 				pkg_fprintf(out->fp, "%n\t%dn\n", p, dep);
 			else
-				pkg_fprintf(out->fp, "%n has a missing dependency: %dn\n",
+				pkg_fprintf(out->fp, COLOR_YELLOW BOLD "%n has a missing dependency: " COLOR_RED BOLD "%dn\n" RESET,
 				    p, dep);
 			if (!noinstall)
 				add_missing_dep(dep, dh, &nbpkgs);
@@ -91,7 +97,7 @@ check_deps(struct pkgdb *db, struct pkg *p, deps_entri
 		if (quiet)
 			pkg_fprintf(out->fp, "%n\t%S\n", p, buf);
 		else
-			pkg_fprintf(out->fp, "%n is missing a required shared library: %S\n",
+			pkg_fprintf(out->fp, COLOR_YELLOW BOLD "%n is missing a required shared library: " COLOR_RED BOLD "%S\n" RESET,
 			    p, buf);
 	}
 	free(slit);
@@ -111,7 +117,7 @@ check_deps(struct pkgdb *db, struct pkg *p, deps_entri
 		if (quiet)
 			pkg_fprintf(out->fp, "%n\tS\n", p, buf);
 		else
-			pkg_fprintf(out->fp, "%n has a missing requirement: %S\n",
+			pkg_fprintf(out->fp, COLOR_YELLOW BOLD "%n has a missing requirement: " COLOR_RED BOLD "%S\n" RESET,
 			    p, buf);
 	}
 	free(slit);
@@ -177,21 +183,21 @@ fix_deps(struct pkgdb *db, deps_entries *dh, int nbpkg
 	}
 
 	if (pkg_jobs_count(jobs) == 0) {
-		printf("\nUnable to find packages for installation.\n\n");
+		printf(COLOR_RED BOLD "\nUnable to find packages for installation.\n\n" RESET);
 		goto cleanup;
 	}
 
 	/* print a summary before applying the jobs */
 	print_jobs_summary(jobs,
-			"The following packages will be installed:\n\n");
+			BOLD "The following packages will be installed:\n\n" RESET);
 
-	rc = query_yesno(false, "\n>>> Try to fix the missing dependencies? ");
+	rc = query_yesno(false, BOLD "\n>>> " COLOR_YELLOW BOLD "Try to fix the missing dependencies? " RESET);
 
 	if (rc) {
 		if (pkgdb_access(PKGDB_MODE_WRITE, PKGDB_DB_LOCAL) ==
 		    EPKG_ENOACCESS) {
-			warnx("Insufficient privileges to modify the package "
-			      "database");
+			warnx(COLOR_RED BOLD "Insufficient privileges to modify the package "
+			      "database" RESET);
 
 			goto cleanup;
 		}
@@ -224,19 +230,19 @@ check_summary(struct pkgdb *db, deps_entries *dh)
 
 		if (pkgdb_it_next(it, &pkg, PKG_LOAD_BASIC) != EPKG_OK) {
 			fixed = false;
-			printf("%s dependency failed to be fixed\n", e->item);
+			printf(COLOR_YELLOW BOLD "%s " COLOR_RED BOLD "dependency failed to be fixed\n" RESET, e->item);
 		} else
-			printf("%s dependency has been fixed\n", e->item);
+			printf(COLOR_YELLOW BOLD "%s " RESET BOLD "dependency has been fixed\n", e->item);
 
 		pkgdb_it_free(it);
 	}
 
 	if (fixed) {
-		printf("\n>>> Missing dependencies were fixed successfully.\n");
+		printf(BOLD "\n>>> " COLOR_GREEN BOLD "Missing dependencies were fixed successfully.\n");
 	} else {
-		printf("\n>>> There are still missing dependencies.\n");
-		printf(">>> Try fixing them manually.\n");
-		printf("\n>>> Also make sure to check 'pkg updating' for known issues.\n");
+		printf(BOLD "\n>>> " COLOR_YELLOW BOLD "There are still missing dependencies.\n" RESET);
+		printf(BOLD ">>> " COLOR_YELLOW BOLD "Try fixing them manually.\n" RESET);
+		printf(BOLD "\n>>> " COLOR_YELLOW BOLD "Also make sure to check 'pkg updating' for known issues.\n" RESET);
 	}
 
 	pkg_free(pkg);
@@ -246,10 +252,10 @@ void
 usage_check(void)
 {
 	fprintf(stderr,
-	    "Usage: pkg check -B|-d|-s|-r [-qvy] -a\n");
+	    BOLD "Usage: pkg check -B|-d|-s|-r [-qvy] -a\n");
 	fprintf(stderr,
 	    "       pkg check -B|-d|-s|-r [-qvy] [-Cgix] <pattern>\n\n");
-	fprintf(stderr, "For more information see 'pkg help check'.\n");
+	fprintf(stderr, "For more information see 'pkg help check'.\n" RESET);
 }
 
 int
@@ -362,18 +368,18 @@ exec_check(int argc, char **argv)
 
 	if (ret == EPKG_ENODB) {
 		if (!quiet)
-			warnx("No packages installed.  Nothing to do!");
+			warnx(COLOR_YELLOW BOLD "No packages installed.  Nothing to do!" RESET);
 		return (EXIT_SUCCESS);
 	} else if (ret == EPKG_ENOACCESS) {
-		warnx("Insufficient privileges to access the package database");
+		warnx(COLOR_RED BOLD "Insufficient privileges to access the package database" RESET);
 		return (EXIT_FAILURE);
 	} else if (ret != EPKG_OK) {
-		warnx("Error accessing the package database");
+		warnx(COLOR_RED BOLD "Error accessing the package database" RESET);
 		return (EXIT_FAILURE);
 	}
 
 	if (pkgdb_access(PKGDB_MODE_WRITE, PKGDB_DB_LOCAL) == EPKG_ENOACCESS) {
-		warnx("Insufficient privileges");
+		warnx(COLOR_RED BOLD "Insufficient privileges" RESET);
 		return (EXIT_FAILURE);
 	}
 
@@ -383,7 +389,7 @@ exec_check(int argc, char **argv)
 
 	if (pkgdb_obtain_lock(db, PKGDB_LOCK_ADVISORY) != EPKG_OK) {
 		pkgdb_close(db);
-		warnx("Cannot get an advisory lock on a database, it is locked by another process");
+		warnx(COLOR_RED BOLD "Cannot get an advisory lock on a database, it is locked by another process" RESET);
 		return (EXIT_FAILURE);
 	}
 
@@ -398,7 +404,7 @@ exec_check(int argc, char **argv)
 		}
 		nbactions = pkgdb_it_count(it);
 		if (nbactions == 0 && match != MATCH_ALL) {
-			warnx("No packages matching: %s", argv[i]);
+			warnx(COLOR_YELLOW BOLD "No packages matching: " COLOR_RED BOLD "%s", argv[i]);
 			rc = EXIT_FAILURE;
 			goto cleanup;
 		}
@@ -408,9 +414,9 @@ exec_check(int argc, char **argv)
 		if (!verbose) {
 			if (!quiet) {
 				if (match == MATCH_ALL)
-					progressbar_start("Checking all packages");
+					progressbar_start(BOLD "Checking all packages" RESET);
 				else {
-					fprintf(msg->fp, "Checking %s", argv[i]);
+					fprintf(msg->fp, BOLD "Checking %s" RESET, argv[i]);
 					fflush(msg->fp);
 					progressbar_start(msg->buf);
 				}
@@ -427,7 +433,7 @@ exec_check(int argc, char **argv)
 				else {
 					++nbdone;
 					job_status_begin(msg);
-					pkg_fprintf(msg->fp, "Checking %n-%v:",
+					pkg_fprintf(msg->fp, BOLD "Checking %n-%v:" RESET,
 					    pkg, pkg);
 					fflush(msg->fp);
 					printf("%s", msg->buf);
@@ -438,7 +444,7 @@ exec_check(int argc, char **argv)
 			/* check for missing dependencies */
 			if (dcheck) {
 				if (!quiet && verbose)
-					printf(" dependencies...");
+					printf(BOLD " dependencies..." RESET);
 				nbpkgs += check_deps(db, pkg, &dh, noinstall, out);
 				if (noinstall && nbpkgs > 0) {
 					rc = EXIT_FAILURE;
@@ -446,7 +452,7 @@ exec_check(int argc, char **argv)
 			}
 			if (checksums) {
 				if (!quiet && verbose)
-					printf(" checksums...");
+					printf(BOLD " checksums..." RESET);
 				if (pkg_test_filesum(pkg) != EPKG_OK) {
 					rc = EXIT_FAILURE;
 				}
@@ -455,7 +461,7 @@ exec_check(int argc, char **argv)
 				if (pkgdb_upgrade_lock(db, PKGDB_LOCK_ADVISORY,
 						PKGDB_LOCK_EXCLUSIVE) == EPKG_OK) {
 					if (!quiet && verbose)
-						printf(" recomputing...");
+						printf(BOLD " recomputing..." RESET);
 					if (pkg_recompute(db, pkg) != EPKG_OK) {
 						rc = EXIT_FAILURE;
 					}
@@ -471,11 +477,11 @@ exec_check(int argc, char **argv)
 				if (pkgdb_upgrade_lock(db, PKGDB_LOCK_ADVISORY,
 						PKGDB_LOCK_EXCLUSIVE) == EPKG_OK) {
 					if (!quiet && verbose)
-						printf(" shared libraries...");
+						printf(BOLD " shared libraries..." RESET);
 					if (pkgdb_reanalyse_shlibs(db, pkg) != EPKG_OK) {
-						pkg_fprintf(stderr, "Failed to "
+						pkg_fprintf(stderr, COLOR_RED BOLD "Failed to "
 						    "reanalyse for shlibs: "
-						    "%n-%v\n", pkg, pkg);
+						    COLOR_YELLOW BOLD "%n-%v\n" RESET, pkg, pkg);
 						rc = EXIT_FAILURE;
 					}
 					pkgdb_downgrade_lock(db,
@@ -505,8 +511,8 @@ exec_check(int argc, char **argv)
 		msg = NULL;
 
 		if (dcheck && nbpkgs > 0 && !noinstall) {
-			printf("\n>>> Missing package dependencies were detected.\n");
-			printf(">>> Found %d issue(s) in the package database.\n\n", nbpkgs);
+			printf(BOLD "\n>>> " COLOR_YELLOW BOLD "Missing package dependencies were detected.\n");
+			printf(BOLD ">>> " COLOR_YELLOW BOLD "Found " COLOR_RED BOLD "%d " COLOR_YELLOW BOLD "issue(s) in the package database.\n\n", nbpkgs);
 			if (pkgdb_upgrade_lock(db, PKGDB_LOCK_ADVISORY,
 					PKGDB_LOCK_EXCLUSIVE) == EPKG_OK) {
 				ret = fix_deps(db, &dh, nbpkgs);
