--- src/annotate.c.orig	2023-08-01 08:25:14 UTC
+++ src/annotate.c
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
 enum action {
 	NONE,
 	ADD,
@@ -53,7 +61,7 @@ void
 usage_annotate(void)
 {
 	fprintf(stderr,
-            "Usage: pkg annotate [-Cgiqxy] [-A|M] <pkg-name> <tag> [<value>]\n");
+            BOLD "Usage: pkg annotate [-Cgiqxy] [-A|M] <pkg-name> <tag> [<value>]\n");
 	fprintf(stderr,
             "       pkg annotate [-Cgiqxy] [-S|D] <pkg-name> <tag>\n");
 	fprintf(stderr,
@@ -61,7 +69,7 @@ usage_annotate(void)
 	fprintf(stderr,
             "       pkg annotate [-qy] -a [-S|D] <tag>\n\n");
 	fprintf(stderr,
-            "For more information see 'pkg help annotate'.\n");
+            "For more information see 'pkg help annotate'.\n" RESET);
 }
 
 static int
@@ -70,18 +78,18 @@ do_add(struct pkgdb *db, struct pkg *pkg, const char *
 	int	ret = EPKG_OK;
 
 
-	if (yes || query_tty_yesno(false, "%n-%v: Add annotation tagged: %S with "
-	               "value: %S? ", pkg, pkg, tag, value)) {
+	if (yes || query_tty_yesno(false, BOLD "%n-%v: Add annotation tagged: " COLOR_BLUE "%S " RESET BOLD "with "
+	               "value: " COLOR_BLUE "%S? " RESET, pkg, pkg, tag, value)) {
 
 		ret = pkgdb_add_annotation(db, pkg, tag, value);
 		if (ret == EPKG_OK) {
 			if (!quiet)
-				pkg_printf("%n-%v: added annotation tagged:"
-				    " %S\n", pkg, pkg, tag);
+				pkg_printf(COLOR_GREEN BOLD "%n-%v: added annotation tagged:"
+				    COLOR_BLUE " %S\n", pkg, pkg, tag);
 		} else if (ret == EPKG_WARN) {
 			if (!quiet) {
-				pkg_warnx("%n-%v: Cannot add annotation tagged:"
-				    " %S\n", pkg, pkg, tag);
+				pkg_warnx(COLOR_RED BOLD "%n-%v: Cannot add annotation tagged:"
+				    COLOR_YELLOW " %S\n" RESET, pkg, pkg, tag);
 			}
 		} else {
 			pkg_warnx("%n-%v: Failed to add annotation tagged:"
@@ -97,16 +105,16 @@ do_modify(struct pkgdb *db, struct pkg *pkg, const cha
 	int	ret = EPKG_OK;
 
 
-	if (yes || query_tty_yesno(false, "%n-%v: Change annotation tagged: %S to "
-		         "new value: %S? ", pkg, pkg, tag, value)) {
+	if (yes || query_tty_yesno(false, COLOR_GREEN BOLD "%n-%v: Change annotation tagged: %S to "
+		         COLOR_BLUE "new value: %S? " RESET, pkg, pkg, tag, value)) {
 		ret = pkgdb_modify_annotation(db, pkg, tag, value);
 		if (ret == EPKG_OK || ret == EPKG_WARN) {
 			if (!quiet)
-				pkg_printf("%n-%v: Modified annotation "
-				       "tagged: %S\n", pkg, pkg, tag);
+				pkg_printf(COLOR_GREEN BOLD "%n-%v: Modified annotation "
+				       COLOR_BLUE "tagged: %S\n" RESET, pkg, pkg, tag);
 		} else {
-			pkg_warnx("%n-%v: Failed to modify annotation tagged:"
-			    " %S", pkg, pkg, tag);
+			pkg_warnx(COLOR_RED BOLD "%n-%v: Failed to modify annotation tagged:"
+			    " %S" RESET, pkg, pkg, tag);
 		}
 	}
 	return (ret);
@@ -117,21 +125,21 @@ do_delete(struct pkgdb *db, struct pkg *pkg, const cha
 {
 	int	ret = EPKG_OK;
 
-	if (yes || query_tty_yesno(false, "%n-%v: Delete annotation tagged: %S? ",
+	if (yes || query_tty_yesno(false, COLOR_GREEN BOLD "%n-%v: Delete annotation tagged: %S? " RESET,
 			 pkg, pkg, tag)) {
 		ret = pkgdb_delete_annotation(db, pkg, tag);
 		if (ret == EPKG_OK) {
 			if (!quiet)
-				pkg_printf("%n-%v: Deleted annotation "
-				       "tagged: %S\n", pkg, pkg, tag);
+				pkg_printf(COLOR_GREEN BOLD "%n-%v: Deleted annotation "
+				       "tagged: %S\n" RESET, pkg, pkg, tag);
 		} else if (ret == EPKG_WARN) {
 			if (!quiet) {
-				pkg_warnx("%n-%v: Cannot delete annotation "
-				     "tagged: %S -- because there is none\n",
+				pkg_warnx(COLOR_YELLOW BOLD "%n-%v: Cannot delete annotation "
+				     COLOR_BLUE "tagged: %S -- because there is none\n" RESET,
 				     pkg, pkg, tag);
 			}
 		} else {
-			pkg_warnx("%n-%v: Failed to delete annotation tagged: %S\n",
+			pkg_warnx(COLOR_RED BOLD "%n-%v: Failed to delete annotation tagged: %S\n" RESET,
 			     pkg, pkg, tag);
 		}
 	}
@@ -153,7 +161,7 @@ do_show(struct pkg *pkg, const char *tag)
 			if (quiet)
 				printf("%s\n", note->value);
 			else
-				pkg_printf("%n-%v: Tag: %S Value: %S\n",
+				pkg_printf(BOLD "%n-%v: Tag: %S Value: %S\n" RESET,
 				    pkg, pkg, note->key, note->value);
 			return (EPKG_OK);
 		}
@@ -303,15 +311,15 @@ exec_annotate(int argc, char **argv)
 			goto cleanup;
 		}
 		if (!quiet)
-			warnx("No packages installed.  Nothing to do!");
+			warnx(COLOR_YELLOW BOLD "No packages installed.  Nothing to do!" RESET);
 		exitcode = EXIT_SUCCESS;
 		goto cleanup;
 	} else if (retcode == EPKG_ENOACCESS) {
-		warnx("Insufficient privileges to modify the package database");
+		warnx(COLOR_RED BOLD "Insufficient privileges to modify the package database" RESET);
 		exitcode = EXIT_FAILURE;
 		goto cleanup;
 	} else if (retcode != EPKG_OK) {
-		warnx("Error accessing the package database");
+		warnx(COLOR_RED BOLD "Error accessing the package database" RESET);
 		exitcode = EXIT_FAILURE;
 		goto cleanup;
 	}
@@ -324,7 +332,7 @@ exec_annotate(int argc, char **argv)
 
 	if (pkgdb_obtain_lock(db, lock_type) != EPKG_OK) {
 		pkgdb_close(db);
-		warnx("Cannot get an exclusive lock on a database, it is locked by another process");
+		warnx(COLOR_RED BOLD "Cannot get an exclusive lock on a database, it is locked by another process" RESET);
 		return (EXIT_FAILURE);
 	}
 
