--- src/clean.c.orig	2023-08-01 08:25:14 UTC
+++ src/clean.c
@@ -66,6 +66,11 @@ typedef tll(char *) dl_list;
 #define CKSUM_MISMATCH	(1U<<2)
 #define SIZE_MISMATCH	(1U<<3)
 #define ALL		(1U<<4)
+#define RESET "\033[0m"
+#define BOLD "\033[1m"
+#define COLOR_GREEN "\033[32m"
+#define COLOR_RED "\033[31m"
+#define COLOR_YELLOW "\033[33m"
 
 static size_t
 add_to_dellist(int fd, dl_list *dl, const char *cachedir, const char *path)
@@ -83,8 +88,8 @@ add_to_dellist(int fd, dl_list *dl, const char *cached
 	if (!quiet) {
 		if (first_entry) {
 			first_entry = false;
-			printf("The following package files will be deleted:"
-			    "\n");
+			printf(BOLD "The following package files will be deleted:"
+			    "\n" RESET);
 		}
 		printf("\t%s\n", store_path);
 	}
@@ -133,9 +138,9 @@ delete_dellist(int fd, const char *cachedir,  dl_list 
 
 	if (!quiet) {
 		if (retcode == EXIT_SUCCESS)
-			printf("All done\n");
+			printf(COLOR_GREEN BOLD "All done\n");
 		else
-			printf("%d package%s could not be deleted\n",
+			printf(COLOR_YELLOW BOLD "%d package%s " COLOR_RED BOLD "could not be deleted\n",
 			      count, count > 1 ? "s" : "");
 	}
 	return (retcode);
@@ -274,8 +279,8 @@ recursive_analysis(int fd, struct pkgdb *db, const cha
 void
 usage_clean(void)
 {
-	fprintf(stderr, "Usage: pkg clean [-anqy]\n\n");
-	fprintf(stderr, "For more information see 'pkg help clean'.\n");
+	fprintf(stderr, BOLD "Usage: pkg clean [-anqy]\n\n");
+	fprintf(stderr, "For more information see 'pkg help clean'.\n" RESET);
 }
 
 int
@@ -333,15 +338,15 @@ exec_clean(int argc, char **argv)
 	retcode = pkgdb_access(PKGDB_MODE_READ, PKGDB_DB_REPO);
 
 	if (retcode == EPKG_ENOACCESS) {
-		warnx("Insufficient privileges to clean old packages");
+		warnx(COLOR_RED BOLD "Insufficient privileges to clean old packages" RESET);
 		close(cachefd);
 		return (EXIT_FAILURE);
 	} else if (retcode == EPKG_ENODB) {
-		warnx("No package database installed.  Nothing to do!");
+		warnx(COLOR_YELLOW BOLD "No package database installed.  Nothing to do!" RESET);
 		close(cachefd);
 		return (EXIT_SUCCESS);
 	} else if (retcode != EPKG_OK) {
-		warnx("Error accessing the package database");
+		warnx(COLOR_RED BOLD "Error accessing the package database" RESET);
 		close(cachefd);
 		return (EXIT_FAILURE);
 	}
@@ -356,8 +361,8 @@ exec_clean(int argc, char **argv)
 	if (pkgdb_obtain_lock(db, PKGDB_LOCK_READONLY) != EPKG_OK) {
 		pkgdb_close(db);
 		close(cachefd);
-		warnx("Cannot get a read lock on a database, it is locked by "
-		    "another process");
+		warnx(COLOR_RED BOLD "Cannot get a read lock on a database, it is locked by "
+		    "another process" RESET);
 		return (EXIT_FAILURE);
 	}
 
@@ -396,10 +401,10 @@ exec_clean(int argc, char **argv)
 	    HN_AUTOSCALE, HN_IEC_PREFIXES);
 
 	if (!quiet)
-		printf("The cleanup will free %s\n", size);
+		printf(BOLD "The cleanup will free %s\n" RESET, size);
 	if (!dry_run) {
 			if (query_yesno(false,
-			  "\nProceed with cleaning the cache? ")) {
+			  BOLD "\nProceed with cleaning the cache? " RESET)) {
 				retcode = delete_dellist(cachefd, cachedir, &dl, tll_length(dl));
 			}
 	} else {
