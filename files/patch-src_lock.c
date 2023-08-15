--- src/lock.c.orig	2023-08-01 08:25:14 UTC
+++ src/lock.c
@@ -36,6 +36,13 @@
 
 #include "pkgcli.h"
 
+#define RESET "\033[0m"
+#define BOLD "\033[1m"
+#define COLOR_GREEN "\033[32m"
+#define COLOR_RED "\033[31m"
+#define COLOR_YELLOW "\033[33m"
+#define COLOR_BLUE "\033[34m"
+
 enum action {
 	LOCK,
 	UNLOCK,
@@ -48,10 +55,10 @@ static int do_unlock(struct pkgdb *db, struct pkg *pkg
 void
 usage_lock(void)
 {
-	fprintf(stderr, "Usage: pkg lock [-lqy] [-a|[-Cgix] <pkg-name>]\n");
-	fprintf(stderr, "       pkg lock --has-locked-packages\n");
-	fprintf(stderr, "       pkg unlock [-lqy] [-a|[-Cgix] <pkg-name>]\n");
-	fprintf(stderr, "For more information see 'pkg help lock'.\n");
+	fprintf(stderr, COLOR_YELLOW BOLD "Usage: " RESET BOLD "pkg lock [-lqy] [-a|[-Cgix] <pkg-name>]\n" RESET);
+	fprintf(stderr, COLOR_BLUE "       pkg lock --has-locked-packages\n");
+	fprintf(stderr, "       pkg unlock [-lqy] [-a|[-Cgix] <pkg-name>]\n" RESET);
+	fprintf(stderr, BOLD "For more information see 'pkg help lock'.\n" RESET);
 }
 
 static int
@@ -59,17 +66,17 @@ do_lock(struct pkgdb *db, struct pkg *pkg)
 {
 	if (pkg_is_locked(pkg)) {
 		if (!quiet)
-			pkg_printf("%n-%v: already locked\n",
+			pkg_printf(COLOR_BLUE BOLD "%n-%v" COLOR_YELLOW ": already locked\n" RESET,
 			       pkg, pkg);
 		return (EPKG_OK);
 	}
 
-	if (!query_yesno(false, "%n-%v: lock this package? ",
+	if (!query_yesno(false, COLOR_BLUE BOLD "%n-%v" COLOR_YELLOW ": lock this package? " RESET,
 				 pkg, pkg))
 		return (EPKG_OK);
 
 	if (!quiet)
-		pkg_printf("Locking %n-%v\n", pkg, pkg);
+		pkg_printf(COLOR_GREEN BOLD "Locking " COLOR_BLUE "%n-%v\n" RESET, pkg, pkg);
 
 	return (pkgdb_set(db, pkg, PKG_SET_LOCKED, (int)true));
 }
@@ -80,16 +87,16 @@ do_unlock(struct pkgdb *db, struct pkg *pkg)
 {
 	if (!pkg_is_locked(pkg)) {
 		if (!quiet)
-			pkg_printf("%n-%v: already unlocked\n", pkg, pkg);
+			pkg_printf(COLOR_BLUE BOLD "%n-%v: " COLOR_YELLOW "already unlocked\n" RESET, pkg, pkg);
 		return (EPKG_OK);
 	}
 
-	if (!query_yesno(false, "%n-%v: unlock this package? ",
+	if (!query_yesno(false, COLOR_BLUE BOLD "%n-%v: " COLOR_YELLOW "unlock this package? " RESET,
 				 pkg, pkg))
 		return (EPKG_OK);
 
 	if (!quiet)
-		pkg_printf("Unlocking %n-%v\n", pkg, pkg);
+		pkg_printf(COLOR_GREEN BOLD "Unlocking " COLOR_BLUE "%n-%v\n" RESET, pkg, pkg);
 
 	return (pkgdb_set(db, pkg, PKG_SET_LOCKED, (int)false));
 }
@@ -106,8 +113,8 @@ do_lock_unlock(struct pkgdb *db, int match, const char
 
 	if (pkgdb_obtain_lock(db, PKGDB_LOCK_EXCLUSIVE) != EPKG_OK) {
 		pkgdb_close(db);
-		warnx("Cannot get an exclusive lock on database. "
-		      "It is locked by another process");
+		warnx(COLOR_RED BOLD "Cannot get an exclusive lock on database. "
+		      "It is locked by another process" RESET);
 		return (EXIT_FAILURE);
 	}
 
@@ -166,13 +173,13 @@ list_locked(struct pkgdb *db, bool has_locked)
 	}
 
 	if (!quiet && !has_locked)
-		printf("Currently locked packages:\n");
+		printf(COLOR_YELLOW BOLD "Currently locked packages:\n" RESET);
 
 	while (pkgdb_it_next(it, &pkg, PKG_LOAD_BASIC) == EPKG_OK) {
 		gotone = true;
 		if (has_locked)
 			break;
-		pkg_printf("%n-%v\n", pkg, pkg);
+		pkg_printf(COLOR_BLUE "%n-%v\n" RESET, pkg, pkg);
 	}
 
 	pkg_free(pkg);
@@ -268,13 +275,13 @@ exec_lock_unlock(int argc, char **argv, enum action ac
 		if (match == MATCH_ALL)
 			return (EXIT_SUCCESS);
 		if (!quiet)
-			warnx("No packages installed.  Nothing to do!");
+			warnx(COLOR_YELLOW BOLD "No packages installed.  Nothing to do!" RESET);
 		return (EXIT_SUCCESS);
 	} else if (retcode == EPKG_ENOACCESS) {
-		warnx("Insufficient privileges to modify the package database");
+		warnx(COLOR_RED BOLD "Insufficient privileges to modify the package database" RESET);
 		return (EXIT_FAILURE);
 	} else if (retcode != EPKG_OK) {
-		warnx("Error accessing the package database");
+		warnx(COLOR_RED BOLD "Error accessing the package database" RESET);
 		return (EXIT_FAILURE);
 	}
 
