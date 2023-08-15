--- src/add.c.orig	2023-08-01 08:25:14 UTC
+++ src/add.c
@@ -41,6 +41,14 @@
 
 #include "pkgcli.h"
 
+#define RESET "\033[0m"
+#define BOLD "\033[1m"
+#define COLOR_GREEN "\033[32m"
+#define COLOR_RED "\033[31m"
+#define COLOR_YELLOW "\033[33m"
+#define COLOR_BLUE "\033[34m"
+#define COLOR_PURPLE "\033[35m"
+
 static int
 is_url(const char * const pattern)
 {
@@ -55,9 +63,9 @@ is_url(const char * const pattern)
 void
 usage_add(void)
 {
-	fprintf(stderr, "Usage: pkg add [-IAfqM] <pkg-name> ...\n");
+	fprintf(stderr, BOLD "Usage: pkg add [-IAfqM] <pkg-name> ...\n");
 	fprintf(stderr, "       pkg add [-IAfqM] <protocol>://<path>/<pkg-name> ...\n\n");
-	fprintf(stderr, "For more information see 'pkg help add'.\n");
+	fprintf(stderr, "For more information see 'pkg help add'.\n" RESET);
 }
 
 int
@@ -124,7 +132,7 @@ exec_add(int argc, char **argv)
 			       PKGDB_MODE_CREATE,
 			       PKGDB_DB_LOCAL);
 	if (retcode == EPKG_ENOACCESS) {
-		warnx("Insufficient privileges to add packages");
+		warnx(COLOR_RED BOLD "Insufficient privileges to add packages" RESET);
 		return (EXIT_FAILURE);
 	} else if (retcode != EPKG_OK)
 		return (EXIT_FAILURE);
@@ -134,7 +142,7 @@ exec_add(int argc, char **argv)
 
 	if (pkgdb_obtain_lock(db, PKGDB_LOCK_EXCLUSIVE) != EPKG_OK) {
 		pkgdb_close(db);
-		warnx("Cannot get an exclusive lock on a database, it is locked by another process");
+		warnx(COLOR_RED BOLD "Cannot get an exclusive lock on a database, it is locked by another process" RESET);
 		return (EXIT_FAILURE);
 	}
 
@@ -164,7 +172,7 @@ exec_add(int argc, char **argv)
 			if (strcmp(file, "-") != 0 && access(file, F_OK) != 0) {
 				warn("%s", file);
 				if (errno == ENOENT)
-					warnx("Was 'pkg install %s' meant?", file);
+					warnx(COLOR_YELLOW "Was 'pkg install %s' meant?" RESET, file);
 				fprintf(failedpkgs->fp, "%s", argv[i]);
 				if (i != argc - 1)
 					fprintf(failedpkgs->fp, ", ");
@@ -190,7 +198,7 @@ exec_add(int argc, char **argv)
 	
 	if(failedpkgcount > 0) {
 		fflush(failedpkgs->fp);
-		printf("\nFailed to install the following %d package(s): %s\n", failedpkgcount, failedpkgs->buf);
+		printf(COLOR_RED BOLD "\nFailed to install the following %d package(s): " COLOR_YELLOW BOLD "%s\n" RESET, failedpkgcount, failedpkgs->buf);
 		retcode = EPKG_FATAL;
 	}
 	xstring_free(failedpkgs);
