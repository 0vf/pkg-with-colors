--- src/create.c.orig	2023-08-01 08:25:14 UTC
+++ src/create.c
@@ -51,6 +51,14 @@
 
 #include "pkgcli.h"
 
+#define RESET "\033[0m"
+#define BOLD "\033[1m"
+#define COLOR_GREEN "\033[32m"
+#define COLOR_RED "\033[31m"
+#define COLOR_YELLOW "\033[33m"
+#define COLOR_BLUE "\033[34m"
+#define COLOR_PURPLE "\033[35m"
+
 tll(struct pkg *) pkg_head = tll_init();
 
 void
@@ -89,13 +97,13 @@ pkg_create_matches(int argc, char **argv, match_t matc
 	/* XXX: get rid of hardcoded timeouts */
 	if (pkgdb_obtain_lock(db, PKGDB_LOCK_READONLY) != EPKG_OK) {
 		pkgdb_close(db);
-		warnx("Cannot get a read lock on a database, it is locked by another process");
+		warnx(COLOR_RED BOLD "Cannot get a read lock on a database, it is locked by another process" RESET);
 		return (EXIT_FAILURE);
 	}
 
 	for (i = 0; i < argc || match == MATCH_ALL; i++) {
 		if (match == MATCH_ALL) {
-			printf("Loading the package list...\n");
+			printf(BOLD"Loading the package list...\n" RESET);
 			if ((it = pkgdb_query(db, NULL, match)) == NULL)
 				goto cleanup;
 			match = !MATCH_ALL;
@@ -110,7 +118,7 @@ pkg_create_matches(int argc, char **argv, match_t matc
 			foundone = true;
 		}
 		if (!foundone) {
-			warnx("No installed package matching \"%s\" found\n",
+			warnx(COLOR_YELLOW BOLD "No installed package matching \"%s\" found\n" RESET,
 			    argv[i]);
 			retcode = EXIT_FAILURE;
 		}
@@ -124,7 +132,7 @@ pkg_create_matches(int argc, char **argv, match_t matc
 		pkg_printf("Creating package for %n-%v\n", el->item, el->item);
 		ret = pkg_create_i(pc, el->item, false);
 		if (ret == EPKG_EXIST) {
-			pkg_printf("%n-%v already packaged, skipping...\n",
+			pkg_printf(COLOR_BLUE BOLD "%n-%v " COLOR_YELLOW BOLD "already packaged, skipping...\n" RESET,
 			  el->item, el->item);
 		}
 		if (ret != EPKG_OK && ret != EPKG_EXIST)
@@ -235,7 +243,7 @@ exec_create(int argc, char **argv)
 				level = INT_MIN;
 				break;
 			}
-			warnx("Invalid compression level %s", optarg);
+			warnx(COLOR_RED BOLD "Invalid compression level " COLOR_YELLOW BOLD "%s" RESET, optarg);
 			return (EXIT_FAILURE);
 			}
 		case 'm':
@@ -263,7 +271,7 @@ exec_create(int argc, char **argv)
 			endptr = NULL;
 			ts = (time_t)strtoimax(optarg, &endptr, 10);
 			if (*endptr != '\0') {
-				warnx("Invalid timestamp %s", optarg);
+				warnx(COLOR_RED BOLD "Invalid timestamp " COLOR_YELLOW BOLD "%s" RESET, optarg);
 				return (EXIT_FAILURE);
 			}
 			break;
@@ -288,8 +296,8 @@ exec_create(int argc, char **argv)
 	}
 
 	if (metadatadir == NULL && manifest == NULL && rootdir != NULL) {
-		warnx("Do not specify a rootdir without also specifying "
-		    "either a metadatadir or manifest");
+		warnx(COLOR_RED BOLD "Do not specify a rootdir without also specifying "
+		    "either a metadatadir or manifest" RESET);
 		usage_create();
 		return (EXIT_FAILURE);
 	}
@@ -302,7 +310,7 @@ exec_create(int argc, char **argv)
 		if (format[0] == '.')
 			++format;
 		if (!pkg_create_set_format(pc, format))
-			warnx("unknown format %s, using the default", format);
+			warnx(COLOR_YELLOW BOLD "unknown format " COLOR_BLUE "%s " RESET BOLD ", using the default" RESET, format);
 	}
 	if (level_is_set)
 	    pkg_create_set_compression_level(pc, level);
