--- src/stats.c.orig	2023-08-01 08:25:14 UTC
+++ src/stats.c
@@ -44,11 +44,19 @@
 
 #include "pkgcli.h"
 
+#define STALL_TIME 5
+#define RESET "\033[0m"
+#define BOLD "\033[1m"
+#define COLOR_GREEN "\033[32m"
+#define COLOR_RED "\033[31m"
+#define COLOR_YELLOW "\033[33m"
+#define COLOR_BLUE "\033[34m"
+
 void
 usage_stats(void)
 {
-	fprintf(stderr, "Usage: pkg stats [-qlrb]\n\n");
-	fprintf(stderr, "For more information see 'pkg help stats'.\n");
+	fprintf(stderr, BOLD "Usage: pkg stats [-qlrb]\n\n");
+	fprintf(stderr, "For more information see 'pkg help stats'.\n" RESET);
 }
 
 int
@@ -100,39 +108,39 @@ exec_stats(int argc, char **argv)
 
 	if (pkgdb_obtain_lock(db, PKGDB_LOCK_READONLY) != EPKG_OK) {
 		pkgdb_close(db);
-		warnx("Cannot get a read lock on a database, it is locked by another process");
+		warnx(COLOR_RED BOLD "Cannot get a read lock on a database, it is locked by another process" RESET);
 		return (EXIT_FAILURE);
 	}
 
 	if (opt & STATS_LOCAL) {
-		printf("Local package database:\n");
-		printf("\tInstalled packages: %" PRId64 "\n", pkgdb_stats(db, PKG_STATS_LOCAL_COUNT));
+		printf(BOLD COLOR_YELLOW "Local package database:\n" RESET);
+		printf(BOLD "\tInstalled packages: " COLOR_BLUE "%" PRId64 "\n" RESET, pkgdb_stats(db, PKG_STATS_LOCAL_COUNT));
 
 		flatsize = pkgdb_stats(db, PKG_STATS_LOCAL_SIZE);
 
 		if (show_bytes)
-			printf("\tDisk space occupied: %" PRId64 "\n\n", flatsize);
+			printf(BOLD "\tDisk space occupied: " COLOR_BLUE "%" PRId64 "\n\n" RESET, flatsize);
 		else {
 			humanize_number(size, sizeof(size), flatsize, "B",
 			    HN_AUTOSCALE, HN_IEC_PREFIXES);
-			printf("\tDisk space occupied: %s\n\n", size);
+			printf(BOLD "\tDisk space occupied: " COLOR_BLUE "%s\n\n" RESET, size);
 		}
 	}
 
 	if ((opt & STATS_REMOTE) && pkg_repos_total_count() > 0) {
-		printf("Remote package database(s):\n");
-		printf("\tNumber of repositories: %" PRId64 "\n", pkgdb_stats(db, PKG_STATS_REMOTE_REPOS));
-		printf("\tPackages available: %" PRId64 "\n", pkgdb_stats(db, PKG_STATS_REMOTE_COUNT));
-		printf("\tUnique packages: %" PRId64 "\n", pkgdb_stats(db, PKG_STATS_REMOTE_UNIQUE));
+		printf(COLOR_YELLOW BOLD "Remote package database(s):\n" RESET);
+		printf(BOLD "\tNumber of repositories: " COLOR_BLUE "%" PRId64 "\n" RESET, pkgdb_stats(db, PKG_STATS_REMOTE_REPOS));
+		printf(BOLD "\tPackages available: " COLOR_BLUE "%" PRId64 "\n" RESET, pkgdb_stats(db, PKG_STATS_REMOTE_COUNT));
+		printf(BOLD "\tUnique packages: " COLOR_BLUE "%" PRId64 "\n" RESET, pkgdb_stats(db, PKG_STATS_REMOTE_UNIQUE));
 
 		flatsize = pkgdb_stats(db, PKG_STATS_REMOTE_SIZE);
 
 		if (show_bytes)
-			printf("\tTotal size of packages: %" PRId64 "\n", flatsize);
+			printf(BOLD "\tTotal size of packages: " COLOR_BLUE "%" PRId64 "\n" RESET, flatsize);
 		else {
 			humanize_number(size, sizeof(size), flatsize, "B",
 			    HN_AUTOSCALE, HN_IEC_PREFIXES);
-			printf("\tTotal size of packages: %s\n", size);
+			printf(BOLD "\tTotal size of packages: " COLOR_BLUE "%s\n" RESET, size);
 		}
 	}
 
