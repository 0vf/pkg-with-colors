--- src/info.c.orig	2023-08-01 08:25:14 UTC
+++ src/info.c
@@ -48,6 +48,14 @@
 
 #include "pkgcli.h"
 
+#define STALL_TIME 5
+#define RESET "\033[0m"
+#define BOLD "\033[1m"
+#define COLOR_GREEN "\033[32m"
+#define COLOR_RED "\033[31m"
+#define COLOR_YELLOW "\033[33m"
+#define COLOR_BLUE "\033[34m"
+
 enum sign {
 	LT,
 	LE,
@@ -59,11 +67,11 @@ enum sign {
 void
 usage_info(void)
 {
-	fprintf(stderr, "Usage: pkg info <pkg-name>\n");
-	fprintf(stderr, "       pkg info -a\n");
+	fprintf(stderr, COLOR_YELLOW BOLD "Usage: " RESET BOLD "pkg info <pkg-name>\n");
+	fprintf(stderr, RESET COLOR_BLUE "       pkg info -a\n");
 	fprintf(stderr, "       pkg info [-AbBDdefIklOqRrs] [-Cgix] <pkg-name>\n");
-	fprintf(stderr, "       pkg info [-AbBDdfIlqRrs] -F <pkg-file>\n\n");
-	fprintf(stderr, "For more information see 'pkg help info'.\n");
+	fprintf(stderr, "       pkg info [-AbBDdfIlqRrs] -F <pkg-file>\n\n" RESET BOLD);
+	fprintf(stderr, "For more information see 'pkg help info'.\n" RESET);
 }
 
 /*
@@ -213,7 +221,7 @@ exec_info(int argc, char **argv)
 			else if (strcasecmp(optarg, "ucl") == 0)
 				opt |= INFO_RAW_UCL;
 			else
-				errx(EXIT_FAILURE, "Invalid format '%s' for the "
+				errx(EXIT_FAILURE, COLOR_RED BOLD "Invalid format " COLOR_YELLOW "'%s'" COLOR_RED "for the "
 				    "raw output, expecting json, json-compact "
 				    "or yaml", optarg);
 			break;
@@ -263,7 +271,7 @@ exec_info(int argc, char **argv)
 
 	if (file != NULL) {
 		if ((fd = open(file, O_RDONLY)) == -1) {
-			warn("Unable to open %s", file);
+			warn(COLOR_RED BOLD "Unable to open " COLOR_YELLOW "%s" RESET, file);
 			return (EXIT_FAILURE);
 		}
 
@@ -271,14 +279,14 @@ exec_info(int argc, char **argv)
 #ifdef HAVE_CAPSICUM
 		cap_rights_init(&rights, CAP_READ, CAP_FSTAT);
 		if (cap_rights_limit(fd, &rights) < 0 && errno != ENOSYS ) {
-			warn("cap_rights_limit() failed");
+			warn(COLOR_RED BOLD "cap_rights_limit() failed" RESET);
 			close(fd);
 			return (EXIT_FAILURE);
 		}
 
 #ifndef PKG_COVERAGE
 		if (cap_enter() < 0 && errno != ENOSYS) {
-			warn("cap_enter() failed");
+			warn(COLOR_RED BOLD "cap_enter() failed" RESET);
 			close(fd);
 			return (EXIT_FAILURE);
 		}
@@ -303,7 +311,7 @@ exec_info(int argc, char **argv)
 
 	ret = pkgdb_access(PKGDB_MODE_READ, PKGDB_DB_LOCAL);
 	if (ret == EPKG_ENOACCESS) {
-		warnx("Insufficient privileges to query the package database");
+		warnx(COLOR_RED BOLD "Insufficient privileges to query the package database" RESET);
 		return (EXIT_FAILURE);
 	} else if (ret == EPKG_ENODB) {
 		if (match == MATCH_ALL)
@@ -311,7 +319,7 @@ exec_info(int argc, char **argv)
 		if (origin_search)
 			return (EXIT_SUCCESS);
 		if (!quiet)
-			warnx("No packages installed");
+			warnx(COLOR_YELLOW BOLD "No packages installed" RESET);
 		return (EXIT_FAILURE);
 	} else if (ret != EPKG_OK)
 		return (EXIT_FAILURE);
@@ -322,7 +330,7 @@ exec_info(int argc, char **argv)
 	drop_privileges();
 	if (pkgdb_obtain_lock(db, PKGDB_LOCK_READONLY) != EPKG_OK) {
 		pkgdb_close(db);
-		warnx("Cannot get a read lock on a database, it is locked by another process");
+		warnx(COLOR_RED BOLD "Cannot get a read lock on a database, it is locked by another process" RESET);
 		return (EXIT_FAILURE);
 	}
 
@@ -413,7 +421,7 @@ exec_info(int argc, char **argv)
 		}
 
 		if (match != MATCH_ALL && pkgname[0] == '\0') {
-			fprintf(stderr, "Pattern must not be empty.\n");
+			fprintf(stderr, COLOR_RED BOLD "Pattern must not be empty.\n" RESET);
 			i++;
 			continue;
 		}
@@ -508,7 +516,7 @@ exec_info(int argc, char **argv)
 
 		if (retcode == EXIT_SUCCESS && !gotone && match != MATCH_ALL) {
 			if (!quiet)
-				warnx("No package(s) matching %s", argv[i]);
+				warnx(COLOR_RED BOLD "No package(s) matching " COLOR_YELLOW "%s" RESET, argv[i]);
 			retcode = EXIT_FAILURE;
 		}
 
